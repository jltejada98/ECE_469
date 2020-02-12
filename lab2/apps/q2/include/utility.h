#define BUFFER_SIZE 20

typedef struct buffer{
	char data[BUFFER_SIZE];
	int start;
	int end;
}buffer;