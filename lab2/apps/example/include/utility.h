#define BUFFER_SIZE 20

typdef struct buffer{
	char data[BUFFER_SIZE];
	int start;
	int end;
};