#include "usertraps.h"
#include "misc.h"

#include "fdisk.h"

dfs_superblock sb;
dfs_inode inodes[DFS_INODE_MAX_NUM];
uint32 fbv[DFS_FBV_MAX_NUM_WORDS];

int diskblocksize = 0; // These are global in order to speed things up
int disksize = 0;      // (i.e. fewer traps to OS to get the same number)

int FdiskWriteBlock(uint32 blocknum, dfs_block *b); //You can use your own function. This function 
//calls disk_write_block() to write physical blocks to disk

void setFbvBit(int bit, int val);
static inline uint32 invert(uint val);

static int negOne = 0xFFFFFFFF;
static inline uint32 invert(uint val) {
  return (n ^ negOne);
}

void setFbvBit(int bit, int val)
{
  uint32 idx = bit / 32;
  uint32 bit_idx = bit % 32;

  uint32 mask = fbv[idx] & invert(1 << bit_idx);
  fbv[idx] = mask | (b << val);
}

void main (int argc, char *argv[])
{
	// STUDENT: put your code here. Follow the guidelines below. They are just the main steps. 
	// You need to think of the finer details. You can use bzero() to zero out bytes in memory
  int i;
  disk_block new_block;


  //Initializations and argc check
  if (argc != 1)
  {
    Printf("Incorrect Argument Number for fdisk.c/main\n");
    GracefulExit();
  }

  // Need to invalidate filesystem before writing to it to make sure that the OS
  // doesn't wipe out what we do here with the old version in memory
  // You can use dfs_invalidate(); but it will be implemented in Problem 2. You can just do 
  // sb.valid = 0

  //Initialize superblock
  sb.valid = 0; //a valid indicator for the file system
  sb.block_size = FDISK_BLOCKSIZE; //The file system block size 
  sb.num_fs_blocks = FDISK_NUM_BLOCKS; //The total number of file system blocks
  sb.start_block_num = FDISK_FBV_BLOCK_START + (DFS_FBV_MAX_NUM_WORDS * 4) / DFS_BLOCKSIZE; //The starting file system block number for the array of inodes
  sb.num_inodes = FDISK_NUM_INODES; //The number of inodes in the inodes array
  sb.start_free_block_num = FDISK_FBV_BLOCK_START; //

  //Initialize sizes/blocks
  disksize = DiskSize();
  diskblocksize = disk_blocksize();
  num_filesystem_blocks = FDISK_INODE_NUM_BLOCKS; //Number of file system blocks to use for inodes

  // Make sure the disk exists before doing anything else
  if (disk_create() == DISK_FAIL)
  {
    Printf("Unable to create disk in fdisk.c/main\n ");
    GracefulExit();
  }

  // Write all inodes as not in use and empty (all zeros)
  bzero(new_block.data, sb.block_size);
  for (i = sb.start_block_num; i < sb.start_free_block_num; ++i)
  {
    FdiskWriteBlock(i, &new_block);
  }

  // Next, setup free block vector (fbv)
  for (i = 0; i < DFS_FBV_MAX_NUM_WORDS; ++i)
  {
    fbv[i] = 0;
  }
  //write free block vector to the disk.
  for (i = sb.start_block_num; i < num_fs_blocks; ++i)
  {
    //Insert function Here Josh.
  }
  for (i = sb.start_free_block_num; i < count; ++i)
  {
    /* code */
  }


  // Finally, setup superblock as valid filesystem and write superblock and boot record to disk: 
  sb.valid = 1;


  // boot record is all zeros in the first physical block, and superblock structure goes into the second physical block
  
  Printf("fdisk (%d): Formatted DFS disk for %d bytes.\n", getpid(), disksize);
}

int FdiskWriteBlock(uint32 blocknum, dfs_block *b) {
  // STUDENT: put your code here


}
