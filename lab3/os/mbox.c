#include "ostraps.h"
#include "dlxos.h"
#include "process.h"
#include "synch.h"
#include "queue.h"
#include "mbox.h"

static mbox mboxes[MBOX_NUM_MBOXES];
static mbox_message msgs[MBOX_NUM_BUFFERS];


//-------------------------------------------------------
//
// void MboxModuleInit();
//
// Initialize all mailboxes.  This process does not need
// to worry about synchronization as it is called at boot
// time.  Only initialize necessary items here: you can
// initialize others in MboxCreate.  In other words, 
// don't waste system resources like locks and semaphores
// on unused mailboxes.
//
//-------------------------------------------------------

void MboxModuleInit() {
	int i;

	for(i = 0; i < MBOX_NUM_MBOXES; i++)
	{
		if(AQueueInit(&(mboxes[i].ready_msgs)) == QUEUE_FAIL)
		{
			printf("Could not initialize ready_msgs queue for mailbox %d", i);
			exitsim();	
		}

		mboxes[i].inuse = 0;
		mboxes[i].num_procs_open = 0;
		mboxes[i].lock = INVALID_LOCK;
		mboxes[i].boxNotEmpty = INVALID_COND;
		mboxes[i].boxNotFull = INVALID_COND;

	}
	for(i = 0; i < MBOX_NUM_BUFFERS; i++)
	{
		msgs[i].len = 0;
		msgs[i].inuse = 0;
	}
}

//-------------------------------------------------------
//
// mbox_t MboxCreate();
//
// Allocate an available mailbox structure for use. 
//
// Returns the mailbox handle on success
// Returns MBOX_FAIL on error.
//
//-------------------------------------------------------
mbox_t MboxCreate() {
	int i;
	mbox_t box = MBOX_FAIL;
	uint32 key;

	key = DisableIntrs();

	for(i = 0; i < MBOX_NUM_MBOXES; i++)
	{
		if(mboxes[i].inuse == 0)
		{
			mboxes[i].inuse = 1;
			box = i;

			mboxes[i].lock = LockCreate();
			if (mboxes[i].lock == INVALID_LOCK)
			{
				printf("Could not initialize lock for mailbox %d", i);
				exitsim();
			}

			mboxes[i].boxNotFull = CondCreate(mboxes[i].lock);
			if(mboxes[i].boxNotFull == INVALID_LOCK)
			{
				printf("Could not initialize cond var for mailbox %d", i);
				exitsim();
			}

			mboxes[i].boxNotEmpty = CondCreate(mboxes[i].lock);
			if(mboxes[i].boxNotEmpty == INVALID_LOCK)
			{
				printf("Could not initialize cond var for mailbox %d", i);
				exitsim();
			}
			break;
		}
	}
	RestoreIntrs(key);

	return box;	
}

//-------------------------------------------------------
// 
// void MboxOpen(mbox_t);
//
// Open the mailbox for use by the current process.  Note
// that it is assumed that the internal lock/mutex handle 
// of the mailbox and the inuse flag will not be changed 
// during execution.  This allows us to get the a valid 
// lock handle without a need for synchronization.
//
// Returns MBOX_FAIL on failure.
// Returns MBOX_SUCCESS on success.
//
//-------------------------------------------------------
int MboxOpen(mbox_t handle) {

	uint32 key;
	key = DisableIntrs();

	(mboxes[handle].num_procs_open)++;

	if(mboxes[handle].num_procs_open <= 0)
	{
		printf("Fatal error: attempting to open mailbox %d with negative number of processes open.\n", handle);
		exitsim();
	}

	RestoreIntrs(key);
	
	return MBOX_SUCCESS;
}

//-------------------------------------------------------
//
// int MboxClose(mbox_t);
//
// Close the mailbox for use to the current process.
// If the number of processes using the given mailbox
// is zero, then disable the mailbox structure and
// return it to the set of available mboxes.
//
// Returns MBOX_FAIL on failure.
// Returns MBOX_SUCCESS on success.
//
//-------------------------------------------------------
int MboxClose(mbox_t handle) {
	mbox* box;
	mbox_message* msg;
	int key;

	key = DisableIntrs();

	box = &mboxes[handle];
	(box->num_procs_open)--;
	if(box->num_procs_open == 0)
	{
		while(AQueueLength(&(box->ready_msgs)))
		{
			msg = AQueueFirst(&(box->ready_msgs));
			if(AQueueRemove(&(msg)) == QUEUE_FAIL){
				printf("Fatal error: Could not remove messages from queue when attempting to dealocate mailbox %d\n", handle);
				exitsim();
			}
		}
		box->inuse = 0;
		box->num_procs_open = 0;
		box->lock = INVALID_LOCK;
		box->boxNotEmpty = INVALID_COND;
		box->boxNotFull = INVALID_COND;
	}
	else if (box->num_procs_open < 0)
	{
		printf("Fatal error: Attempting to close mailbox, mailbox now has negative number with it open %d\n", handle);
		exitsim();
	}


	RestoreIntrs(key);

	return MBOX_SUCCESS;
}

//-------------------------------------------------------
//
// int MboxSend(mbox_t handle,int length, void* message);
//
// Send a message (pointed to by "message") of length
// "length" bytes to the specified mailbox.  Messages of
// length 0 are allowed.  The call 
// blocks when there is not enough space in the mailbox.
// Messages cannot be longer than MBOX_MAX_MESSAGE_LENGTH.
// Note that the calling process must have opened the 
// mailbox via MboxOpen.
//
// Returns MBOX_FAIL on failure.
// Returns MBOX_SUCCESS on success.
//
//-------------------------------------------------------
int MboxSend(mbox_t handle, int length, void* message) {
	int msg;
	mbox* box;
	mbox_message *mail;
	Link *l;


	box = &mboxes[handle];

	if(length > MBOX_MAX_MESSAGE_LENGTH)
	{
		return MBOX_FAIL;
	}

	if(LockHandleAcquire(box->lock) == SYNC_FAIL)
	{
		printf("Fatal error: Unable to acquire lock in MboxSend\n");
		exitsim();
	}

	while(AQueueLength(&(box->ready_msgs)) >= MBOX_MAX_BUFFERS_PER_MBOX)
	{
		if(CondHandleWait((box->boxNotFull)) == SYNC_FAIL){
            printf("Fatal error: Unable to wait on condition variable in MboxSend\n");
            exitsim();   
        }
	}

	//Critical Section, putting in messages
	msg = InitMessage(length);
	if (msg == -1)
	{
		printf("Fatal error: Could not initialize message in mailbox %d\n", handle);
		exitsim();
	}
	//Copy message to buffer
	mail = &msgs[msg];
	bcopy(message, &(mail->buffer[0]), length);
	l = AQueueAllocLink ((void *)mail);
	if (l == NULL)
	{
		printf("FATAL ERROR: could not allocate link for message in MboxSend!\n");
      	exitsim();
	}
	if (AQueueInsertLast(&(box->ready_msgs),l) != QUEUE_SUCCESS )
	{
		printf("FATAL ERROR: could not insert new link into lock waiting queue in MboxSend!\n");
      	exitsim();
	}

    if(CondHandleSignal((box->boxNotEmpty)) == SYNC_FAIL){
            printf("Fatal error: Unable to signal on condition variable in MboxSend\n");
            exitsim();   
    }
	
	if(LockHandleRelease(box->lock)== SYNC_FAIL)
	{
		printf("Fatal error: Unable to acquire lock in MboxSend\n");
		exitsim();
	}

	return MBOX_SUCCESS;
}

int InitMessage(int length){
	int key;
	int message;
	int i;

	key = DisableIntrs();
	message = -1;
	for (i = 0; i < MBOX_NUM_BUFFERS; ++i)
	{
		if (msgs[i].inuse == 0)
		{
			msgs[i].inuse = 1;
			msgs[i].len = length;
			message = i ;
			break;
		}
	}
	RestoreIntrs(key);
	return message;
}


//-------------------------------------------------------
//
// int MboxRecv(mbox_t handle, int maxlength, void* message);
//
// Receive a message from the specified mailbox.  The call 
// blocks when there is no message in the buffer.  Maxlength
// should indicate the maximum number of bytes that can be
// copied from the buffer into the address of "message".  
// An error occurs if the message is larger than maxlength.
// Note that the calling process must have opened the mailbox 
// via MboxOpen.
//   
// Returns MBOX_FAIL on failure.
// Returns number of bytes written into message on success.
//
//-------------------------------------------------------
int MboxRecv(mbox_t handle, int maxlength, void* message) {
	mbox* box;
	mbox_message *mail;
	Link *l;

	box = &mboxes[handle];

	//Get lock
	if(LockHandleAcquire(box->lock) == SYNC_FAIL)
	{
		printf("Fatal error: Unable to acquire lock in MboxRecv\n");
		exitsim();
	}

	//Wait while no messages in mailbox
	while(AQueueEmpty(&(box->ready_msgs)))
	{
		if(CondHandleWait((box->boxNotEmpty)) == SYNC_FAIL){
            printf("Fatal error: Unable to wait on condition variable in MboxRecv\n");
            exitsim();   
        }
	}

	//TODO: Obtain message.
	l = AQueueFirst(&(box->ready_msgs));
	if (l == NULL)
	{
		printf("FATAL ERROR: could not obtain link for message in MboxRecv!\n");
      	exitsim();
	}

	mail = l->object;
	if (AQueueRemove(&l) == SYNC_FAIL)
	{
		printf("FATAL ERROR: could remove link into messagebox  waiting queue in MboxRecv!\n");
      	exitsim();
	}

	bcopy(&(mail->buffer[0]), message, message->len);

	//Message not in use
	mail->inuse = 0;

	//Signal Cond var, box not full
	if(CondHandleSignal((box->boxNotFull)) == SYNC_FAIL){
        printf("Fatal error: Unable to signal on condition variable in MboxSend\n");
        exitsim();   
    }


	//Release Lock
	if(LockHandleRelease(box->lock)== SYNC_FAIL)
	{
		printf("Fatal error: Unable to acquire lock in MboxSend\n");
		exitsim();
	}
    
  return message->len;
}

//--------------------------------------------------------------------------------
// 
// int MboxCloseAllByPid(int pid);
//
// Scans through all mailboxes and removes this pid from their "open procs" list.
// If this was the only open process, then it makes the mailbox available.  Call
// this function in ProcessFreeResources in process.c.
//
// Returns MBOX_FAIL on failure.
// Returns MBOX_SUCCESS on success.
//
//--------------------------------------------------------------------------------
int MboxCloseAllByPid(int pid) {
  return MBOX_FAIL;
}
