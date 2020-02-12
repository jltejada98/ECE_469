#define BUFFER_SIZE 20

#define PRODUCER_FILENAME 
#define CONSUMER_FILENAME 

typedef struct buffer{
	char data[BUFFER_SIZE];
	int start;
	int end;
}buffer;
