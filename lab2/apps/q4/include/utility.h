#define BUFFER_SIZE 5

#define PRODUCER_FILENAME "producer.dlx.obj"
#define CONSUMER_FILENAME "consumer.dlx.obj"

typedef struct buffer{
	char data[BUFFER_SIZE];
	int start;
	int end;
	int empty;
}buffer;
