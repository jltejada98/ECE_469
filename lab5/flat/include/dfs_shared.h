#ifndef __DFS_SHARED__
#define __DFS_SHARED__

typedef struct dfs_superblock {
  // STUDENT: put superblock internals here
  char valid; //a valid indicator for the file system
  uint32 block_size; //The file system block size 
  uint32 num_fs_blocks; //The total number of file system blocks
  uint32 start_block_num; //The starting file system block number for the array of inodes
  uint32 num_inodes; //The number of inodes in the inodes array
  uint32 start_free_block_num; //he starting file system block number for the free block vector.

} dfs_superblock;

#define DFS_BLOCKSIZE 1024  // Must be an integer multiple of the disk blocksize

typedef struct dfs_block {
  char data[DFS_BLOCKSIZE];
} dfs_block;

//Student Definitions:
#define DFS_MAX_FILENAME_LENGTH 44 //Adjusted filename length (96-(4*3+4*10))
#define DFS_NUM_INODES 192
#define DFS_NUM_BLOCKS (DFS_MAX_FILESYSTEM_SIZE / DFS_BLOCKSIZE)
#define DFS_FBV_MAX_NUM_WORDS (DFS_NUM_BLOCKS / 32)

typedef struct dfs_inode {
  // STUDENT: put inode structure internals here
  // IMPORTANT: sizeof(dfs_inode) MUST return 128 in order to fit in enough
  // inodes in the filesystem (and to make your life easier).  To do this, 
  // adjust the maximumm length of the filename until the size of the overall inode 
  // is 128 bytes.
	int inuse; //an in use indicator to tell if an inode is free or in use.
	uint32 file_size; //the size of the file this inode represents (i.e. the maximum byte that has been written to this file)
	char filename[DFS_MAX_FILENAME_LENGTH];
	uint32 virtual_blocks[10]; //a table of direct address translations for the first 10 virtual blocks.
	unint32 indirect_blocks; //A block number of a file system block on the disk which holds a table of indirect address translations for the virtual blocks beyond the first 10.
} dfs_inode;

#define DFS_MAX_FILESYSTEM_SIZE 0x1000000  // 16MB

#define DFS_FAIL -1
#define DFS_SUCCESS 1



#endif
