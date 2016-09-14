/*
 *
 * queue.h - public interface to queue.c, the queue module
 *
 */

typedef struct queue {
    struct node *root;
    struct node *tailPointer;
    int length;
    
} queue;

extern void enqueue(queue* q, node* newNode);
extern value *dequeue(queue* q);
extern queue *newQueue(void);
