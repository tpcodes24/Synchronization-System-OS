#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>

#include "serializer-api.h"
#include "serializer.h"

serializer_t *create_serializer() {
    serializer_t *temp = (serializer_t *)malloc(sizeof(serializer_t));
    if (temp == NULL) {
        printf("Serializer - Memory allocation failed.\n");
        return NULL;
    }

    // Initialize the mutex
    if (pthread_mutex_init(&temp->serializer_lock, NULL) != 0) {
        printf("Serializer Mutex Lock: Failed.");
        free(temp);  // Free memory on failure
        return NULL;
    }
	
    // Initializing Linked List of Queues (To hold the different queues)
    temp->ll_queue_head = NULL;             // No queues at initialization
    temp->ll_queue_count = 0;               // Count of queues

    printf("Serializer created successfully!\n");  // Success message
    return temp; 
}

void destroy_serializer(serializer_t *serial) {

    // Free memory...
	if (serial == NULL) {
        return; 
    }

    // Free the linked list of queues if it exists
    //while (serial->ll_queue_head != NULL) {
        // Call the function to destroy each queue
        //free(serial->ll_queue_head);  // Free the head queue
        //serial->ll_queue_head = serial->ll_queue_head->next;  // Move to the next queue
    //}

    // Destroy the mutex
    pthread_mutex_destroy(&serial->serializer_lock);

    // Free the serializer itself
    free(serial);

    printf("Serializer destroyed successfully!\n");  // Success message
    
}

void enter_serializer(serializer_t *serial) {

    // Gain possession of the serializer
	if (serial == NULL) {
        printf("Serializer is not initialized. Cannot enter.\n");
        return;
    }

    // Attempt to lock the mutex
    int result = pthread_mutex_lock(&serial->serializer_lock);
    if (result != 0) {
        printf("Lock Failed.\n");
        return;  // Handle error if the lock cannot be acquired
    }

    printf("Successfully entered serializer.\n %lu", (unsigned long)pthread_self());	// Success message
    
}

void exit_serializer(serializer_t *serial) {

    // Release
	if (serial == NULL) {
        printf("Exit - Serializer is NULL. Cannot exit.\n");
        return;
    }

    // Release the serializer and signal any waiting threads
        
    // Use a for loop to iterate through the queues
    for (queue_t *temp = serial->ll_queue_head; temp != NULL; temp = temp->next) {
        printf("Checking the Exit Conditions");
        // printf()
        if (temp->head!=NULL && temp->head->cond(NULL)) {
            printf("Condition satisfied, Head Signalled: %d\n %lu", temp->head->t_id, (unsigned long)pthread_self());
            pthread_cond_signal(&temp->head->nqcond);  // Signal the condition variable
            break;  // Exit the loop after signaling
        }
    }

    pthread_mutex_unlock(&serial->serializer_lock);  // Release the lock
    printf("Released serializer\n");

}

crowd_t *create_crowd(serializer_t *crowd) {
    //crowd_t *temp = NULL;

    // Allocate necessary structures using malloc...
	crowd_t *temp = (crowd_t *)malloc(sizeof(crowd_t)); 
	
	if (!temp) return NULL;

    // Check if the memory allocation was successful
    if (temp != NULL) {
        temp->t_count = 0;  // Initialize thread count to 0
        temp->next = NULL;       // No next crowd yet.
    }
	printf("Crowd created successfully!\n");
    return temp;
}

void destroy_crowd(serializer_t *serial, crowd_t *crowd) {

    // Free memory...
	
	if (crowd == NULL) {
        printf("Crowd Destroy - NULL.\n");
        return; 
    }

    // Free the crowd structure itself
	if(crowd) {
		free(crowd);
	}
    printf("Crowd destroyed successfully.\n");

}

void join_crowd(serializer_t *serial, crowd_t *crowd, void (*body)(void *), void *body_args) {
    // Put the executing thread into crowd
	
	crowd->t_count++;
    printf("Thread %lu joining crowd. Total: %d\n", (unsigned long)pthread_self(), crowd->t_count);

    // Release possession of the serializer
	
	exit_serializer(serial);

    // Execute body
    body(body_args);
	
	// Perform leave_crowd operation
	
	// Re-enter the serializer after executing the body
    enter_serializer(serial);
	printf("Entering serilaizer in join crowd %lu", (unsigned long)pthread_self());
	// Decrement the thread count as the thread is leaving the crowd
    crowd->t_count--;
    printf("Thread %lu leaving crowd. Total: %d\n", (unsigned long)pthread_self(), crowd->t_count);

}

int crowd_count(serializer_t *serial, crowd_t *crowd) {
    // int ret = 0;
	//printf("Crowdcount %d", crowd->t_count);
    return crowd->t_count;
}

queue_t *create_queue(serializer_t *serial) {
    //queue_t *temp = NULL;

    // Allocate necessary structures using malloc...
	
	queue_t *temp = (queue_t *)malloc(sizeof(queue_t));
    if (temp == NULL) {
        printf("Queue - Memory allocation failed.\n");
        return NULL;
    }
	
	// Initialize the queue fields
    temp->head = NULL;  // No elements in the queue yet
    temp->tail = NULL;
    temp->next = NULL;  // No next queue in the linkedlist yet
	
	// Add the new queue to the linked list of queues in the serializer
    if (serial->ll_queue_count == 0) {
        // If there are no queues, this is the first one
        serial->ll_queue_head = temp;
    } else {
        // Otherwise, find the last queue using the queue count and add the new one
        queue_t *last_queue = serial->ll_queue_head;
        for (int i = 1; i < serial->ll_queue_count; i++) {
            last_queue = last_queue->next;
        }
        last_queue->next = temp;  // Append the new queue at the end
    }

    // Increment the queue count in the serializer
    serial->ll_queue_count++;

    printf("Queue created and added to the list. Total queues: %d\n", serial->ll_queue_count);

    return temp;
}

void destroy_queue(serializer_t *serial, queue_t *queue) {
	
	 // Free memory...
	 
	 
    if (queue) {
        // Free all nodes in the queue
        queue_node *current = queue->head;
        while (current) {
            queue_node *next_node = current->next;
            free(current); // Free each node
            current = next_node;
        }

        // Remove the queue from the linked list of queues
	//if (serial->ll_queue_head == queue) {
		// If it's the head, update the head to the next queue
	//	serial->ll_queue_head = queue->next;
	//} else {
		// Otherwise, find the previous queue
	//	queue_t *prev = serial->ll_queue_head;
	//	while (prev && prev->next != queue) {
	//		prev = prev->next;
	//	}
	//	if (prev) {
		//    prev->next = queue->next; // Link previous to next
	  //  }
	//}

        // Decrement the queue count and free the queue itself
        serial->ll_queue_count--;
		//pthread_cond_destroy();
        free(queue); // Free the queue
        printf("Queue destroyed. Remaining queues: %d\n", serial->ll_queue_count);
    }
}

void enqueue(serializer_t *serial, queue_t *queue, bool (*cond)(void *)) {
    // Get the current thread's ID
    pthread_t thread_id = pthread_self();
     printf("Entering enqueue function %lu\n",(unsigned long)pthread_self());
    // Allocate memory for a new node in the queue
    queue_node *new_node = (queue_node *)malloc(sizeof(queue_node));
    if (new_node == NULL) {
        printf("Memory allocation for new_node failed.\n");
        return;
    }

    // Store the thread ID and the condition function
    new_node->t_id = (int)(uintptr_t)thread_id;
    new_node->next = NULL;
    new_node->cond = cond;

    // Add the new node to the end of the queue
    if (queue->tail == NULL) {
        queue->head = new_node;
        queue->tail = new_node;
    } else {
        queue->tail->next = new_node;
        queue->tail = new_node;
    }

    // Wait until this node reaches the head of the queue and the condition is met
    while (queue->head->t_id != new_node->t_id || !cond(queue)) {
        printf("Thread %lu is waiting. Mmy ID: %d\n", 
               (unsigned long)thread_id, new_node->t_id);
        
        // Wait for the condition variable to be signaled
        pthread_cond_wait(&new_node->nqcond, &serial->serializer_lock);
    }

    // The current thread is at the head of the queue and the condition is satisfied
    // Remove the node from the queue
    queue_node *dequeue_node = queue->head;
    queue->head = queue->head->next;
    
    printf("Dequeue with ID: %d\n %lu", dequeue_node->t_id, (unsigned long)pthread_self());
    if (queue->head == NULL) {
        queue->tail = NULL;
    }
    free(dequeue_node);
}