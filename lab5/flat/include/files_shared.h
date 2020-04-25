#ifndef __FILES_SHARED__
#define __FILES_SHARED__

#define FILE_SEEK_SET 1
#define FILE_SEEK_END 2
#define FILE_SEEK_CUR 3

#define FILE_MAX_FILENAME_LENGTH 44

#define FILE_MAX_READWRITE_BYTES 4096

typedef struct file_descriptor {
  // STUDENT: put file descriptor info here
	char inuse;
	char filename[FILE_MAX_FILENAME_LENGTH]; 
	unit32 inode_handle; 
	unit32 curr_pos; //the current position in the file
	char eof_flag; //end-of-file flag
	char mode; //0 is undefined, 1 = read, 2 = write, 3 = read/write

} file_descriptor;

#define FILE_FAIL -1
#define FILE_EOF -1
#define FILE_SUCCESS 1

#endif
