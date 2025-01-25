#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>



struct queue_node {
    int t_id;                         // Thread ID
    struct queue_node *next;          // Pointer to the next node in the queue
    bool (*cond)(void *);        // Condition function pointer
    pthread_cond_t nqcond;
 
};
typedef struct queue_node queue_node; // Forward declare queue_node

struct psu_queue {
    queue_node *head;                 // Head of the queue
    queue_node *tail;                 // Tail of the queue
    struct psu_queue *next;           // Pointer to the next queue in the linked list
};
typedef struct psu_queue queue_t;

struct psu_serializer {
    pthread_mutex_t serializer_lock;  // Mutex for thread synchronization
    queue_t *ll_queue_head;           // Head of the linked list of queues
    int ll_queue_count;               // Number of queues in the linked list
};
typedef struct psu_serializer serializer_t;

struct psu_crowd {
    int t_count;                      // Number of threads in the crowd
    struct psu_crowd *next;           // Pointer to the next crowd in the linked list
};
typedef struct psu_crowd crowd_t;



serializer_t *create_serializer();
void destroy_serializer(serializer_t *serial);
void enter_serializer(serializer_t *serial);
void exit_serializer(serializer_t *serial);

crowd_t *create_crowd(serializer_t *crowd);
void destroy_crowd(serializer_t *serial, crowd_t *crowd);
void join_crowd(serializer_t *serial, crowd_t *crowd, void (*body)(void *), void *body_args);
int crowd_count(serializer_t *serial, crowd_t *crowd);

queue_t *create_queue(serializer_t *serial);
void destroy_queue(serializer_t *serial, queue_t *queue);
void enqueue(serializer_t *serial, queue_t *queue, bool (*cond)(void *));
