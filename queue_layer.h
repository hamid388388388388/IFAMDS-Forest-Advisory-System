#ifndef QUEUE_LAYER_H
#define QUEUE_LAYER_H

#include <iostream>
#include "linklist_layer.h"
using namespace std;

//-------------------- Queue Layer--------------------
// Queues store events in First-In-First-Out (FIFO) order
// Used for processing events in the order they arrive

class queue_layer
{
    event_node* front;  // Front of the queue (oldest event)
    event_node* rear;   // Back of the queue (newest event)

public:
    queue_layer();      // Constructor - creates empty queue
    bool isempty();     // Check if queue has no events
    void enqueue(event_node* node);  // Add event to back of queue
    event_node* dequeue();           // Remove and return event from front
    event_node* peek();              // Look at front event without removing
    void build_queue(linklist_layer &global_list, queue_layer &q1, queue_layer &q2, queue_layer &q3, queue_layer &q4);  // Create multiple queues from global list
    void display_queue();            // Show all events in queue
    void priority_switching(queue_layer &q1, queue_layer &q2, queue_layer &q3);  // Move high-priority events to front
    int getsize();                   // Get number of events in queue
    void load_balancing(queue_layer &q1, queue_layer &q2);  // Distribute events evenly between queues
};

#endif