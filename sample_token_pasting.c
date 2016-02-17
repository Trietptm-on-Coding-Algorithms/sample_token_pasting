#include <stdio.h>
#include <assert.h>

typedef enum {
	PEEK_REQUEST_ITEM,
	PEEK_REPLY_ITEM,
	MOD_REQUEST_ITEM,
	MOD_REPLY_ITEM
}ITEM_TYPE;

struct queue_item {
	ITEM_TYPE type;
	char payload[32];
};

struct handler {
	int identifier;
	int (*process_data) (void* data);
};

#define PRINT_FUNC_NAME \
do { \
	printf("In function: %s \n", __func__); \
} while (0);

static inline int process_peek_request(const struct queue_item *const
				      peek_req,
				      struct handler *p)
{
	/** Algorithm A ...  */
	PRINT_FUNC_NAME
	return 0;
}

static inline int process_peek_reply(const struct queue_item *const
				    peek_rep,
				    struct handler *p)
{
	/** Algorithm B ...  */
	PRINT_FUNC_NAME
	return 0;
}

static inline int process_modification_request(const struct queue_item *const
				      modification_req,
				      struct handler *p)
{
	/** TODO: Invalidate cached items taking part in the MOD transaction **/

	/** TODO: Enqueue the MOD request to egress_port_output_queue */

	/** TODO: Notify egress_port thread to consume the MOD request */

	PRINT_FUNC_NAME

	return 0;/** Success */

 error:
	return -1;/** Failed */
}

static inline int process_modification_reply(const struct queue_item *const
				    modification_rep,
				    struct handler *p)
{
	/** TODO: Enqueue the MOD reply to ingress_port_output_queue */

	/** TODO: Notify ingress_port thread to consume the MOD reply */

	PRINT_FUNC_NAME

	return 0;/** Success */

 error:
	return -1;/** Failed */
}

#define PROCESS_DEQUEUED_ITEM(MESSAGE, TYPE) \
static inline int process_dequeued_##MESSAGE(const struct queue_item *const MESSAGE,\
					    struct handler *p) \
{ \
	assert((MESSAGE != NULL) && (p != NULL)); \
	\
	assert((MESSAGE->type == PEEK_##TYPE) || \
	       (MESSAGE->type == MOD_##TYPE)); \
	\
	PRINT_FUNC_NAME \
	\
	if (MESSAGE->type == PEEK_##TYPE) { \
		printf("Processing PEEK " #MESSAGE "\n"); \
		return process_peek_##MESSAGE(MESSAGE, p); \
	\
	} else if (MESSAGE->type == MOD_##TYPE) { \
		printf("Processing MOD " #MESSAGE "\n"); \
		return process_modification_##MESSAGE(MESSAGE, p); \
	\
	} else { \
		printf("Warning: Unknown " #MESSAGE " type!\n"); \
		return -1; /** Failed */ \
	} \
}

/** Token-pasted function instance to handle request message */
PROCESS_DEQUEUED_ITEM(request, REQUEST_ITEM)

/** Token-pasted function instance to handle reply message */
PROCESS_DEQUEUED_ITEM(reply, REPLY_ITEM)

int main (int argc, char * argv[])
{
	int i; 

	struct queue_item req_item[2], rep_item[2];
	struct handler h;

	req_item[0].type = PEEK_REQUEST_ITEM;
	req_item[1].type = MOD_REQUEST_ITEM;

	rep_item[0].type = PEEK_REPLY_ITEM;
	rep_item[1].type = MOD_REPLY_ITEM;

	for (i = 0; i < 2; i++) {
		process_dequeued_request(&req_item[i], &h);
	}

	for (i = 0; i < 2; i++) {
		process_dequeued_reply(&rep_item[i], &h);
	}

	return 0;
}
