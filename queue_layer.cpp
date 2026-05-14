#include "queue_layer.h"

// QUEUE LAYER - Task Scheduling Engine (Department 4)
// Q1: Routine Monitoring Queue
// Q2: Continuous Surveillance Queue
// Q3: Emergency Response Queue
// Q4: Multi-Factor Decision Queue
// Principle: FIFO - First In First Out

// Constructor - initialize empty queue (front and rear both NULL)
// Time Complexity: O(1)
queue_layer::queue_layer(){
    front = rear = NULL;
}

// isempty() - Check if queue has no elements
// Time Complexity: O(1)
bool queue_layer::isempty(){
    return front == NULL;
}

// enqueue() - Add a new event node to the REAR of the queue
// FIFO rule: new tasks always enter from the back
// Time Complexity: O(1) - rear pointer maintained, no traversal needed
void queue_layer::enqueue(event_node* node)
{
    node->next = NULL;

    if(rear == NULL){
        // queue was empty, both front and rear point to new node
        front = rear = node;
    }
    else{
        rear->next = node;
        rear = node;
    }
}

// dequeue() - Remove and return event node from the FRONT of the queue
// FIFO rule: oldest task is processed first
// Time Complexity: O(1) - front pointer directly accessed
event_node* queue_layer::dequeue(){
    if(isempty()){
        cout << "queue is empty" << endl;
        return NULL;
    }
    event_node* temp = front;
    front = front->next;
    if(front == NULL){
        rear = NULL;  // queue became empty, reset rear too
    }
    return temp;
}

// peek() - View the FRONT element without removing it
// Used to check next task before deciding to process
// Time Complexity: O(1)
event_node* queue_layer::peek()
{
    return front;
}

// getsize() - Count number of elements currently in the queue
// Time Complexity: O(n) - must traverse all n nodes to count
int queue_layer::getsize()
{
    int count = 0;
    event_node* temp = front;

    while(temp != NULL)
    {
        count++;
        temp = temp->next;
    }

    return count;
}

// build_queue() - Classify events from global list into Q1/Q2/Q3/Q4 based on urgency
// Classification rules:
//   Q3 (Emergency): temp > 45 OR smoke > 70
//   Q2 (Surveillance): humidity < 25 OR smoke 40-70 OR temp 35-45
//   Q1 (Routine): all other normal readings
//   Q4 (Decision): ALL events regardless of type
// Time Complexity: O(n) - single pass through n nodes of global_list
void queue_layer::build_queue(linklist_layer &global_list,
                              queue_layer &q1,
                              queue_layer &q2,
                              queue_layer &q3,
                              queue_layer &q4)
{
    // Clear all queues before rebuilding to prevent duplicate accumulation
    while (!q1.isempty()) { event_node* e = q1.dequeue(); delete e; }
    while (!q2.isempty()) { event_node* e = q2.dequeue(); delete e; }
    while (!q3.isempty()) { event_node* e = q3.dequeue(); delete e; }
    while (!q4.isempty()) { event_node* e = q4.dequeue(); delete e; }

    event_node* temp = global_list.gethead();

    while(temp != NULL)
    {
        int t = temp->temperature;
        int h = temp->humidity;
        int s = temp->smoke_lvl;
        int r = temp->row;
        int c = temp->col;

        // Q3: Emergency - highest priority
        if(t > 45 || s > 70)
        {
            q3.enqueue(new event_node(t, h, s, temp->time, r, c));
        }
        // Q2: Surveillance - medium priority
        else if(
            (h < 25) ||
            (s > 40 && s <= 70) ||
            (t > 35 && t <= 45)
        )
        {
            q2.enqueue(new event_node(t, h, s, temp->time, r, c));
        }
        // Q1: Routine - normal monitoring
        else
        {
            q1.enqueue(new event_node(t, h, s, temp->time, r, c));
        }

        // Q4: All events go here for multi-factor decision processing
        q4.enqueue(new event_node(t, h, s, temp->time, r, c));

        temp = temp->next;
    }
}

// display_queue() - Print all events in queue from front to rear
// Time Complexity: O(n) - single traversal through n nodes
void queue_layer::display_queue(){
    event_node* temp = front;
    if (temp == NULL) return;
    while(temp != NULL)
    {
        string status = (temp->temperature > 45 || temp->smoke_lvl > 70) ? "[!! EMERGENCY !!]" :
                        (temp->humidity < 25 || temp->smoke_lvl > 40)    ? "[SURVEILLANCE]   " :
                                                                            "[ROUTINE]        ";
        cout << "  | Temp=" << temp->temperature
             << " Hum=" << temp->humidity
             << " Smoke=" << temp->smoke_lvl
             << "  Zone(" << temp->row << "," << temp->col << ")"
             << "  " << status << endl;
        cout << "  +--------------------------------------------+" << endl;
        temp = temp->next;
    }
}

// priority_switching() - Process queues in priority order: Q3 first, then Q2, then Q1
// Ensures emergency tasks are never delayed behind routine ones
// Time Complexity: O(n1 + n2 + n3) where n1,n2,n3 are sizes of each queue
void queue_layer::priority_switching(queue_layer &q1,
                                     queue_layer &q2,
                                     queue_layer &q3)
{
    cout << "\n  +============================================+" << endl;
    cout << "  |       PRIORITY TASK PROCESSING             |" << endl;
    cout << "  +============================================+" << endl;

    // Emergency tasks first (highest priority)
    while(!q3.isempty())
    {
        event_node* e = q3.dequeue();
        if(e != NULL){
            cout << "  | [Q3 EMERGENCY]  Temp=" << e->temperature
                 << "  Zone(" << e->row << "," << e->col << ")"
                 << string(max(0, 14 - (int)to_string(e->temperature).length()), ' ') << "|" << endl;
            cout << "  +--------------------------------------------+" << endl;
            delete e;
        }
    }

    // Surveillance tasks second
    while(!q2.isempty())
    {
        event_node* e = q2.dequeue();
        if(e != NULL){
            cout << "  | [Q2 SURVEILL.]  Temp=" << e->temperature
                 << "  Zone(" << e->row << "," << e->col << ")"
                 << string(max(0, 14 - (int)to_string(e->temperature).length()), ' ') << "|" << endl;
            cout << "  +--------------------------------------------+" << endl;
            delete e;
        }
    }

    // Routine tasks last
    while(!q1.isempty())
    {
        event_node* e = q1.dequeue();
        if(e != NULL){
            cout << "  | [Q1 ROUTINE]    Temp=" << e->temperature
                 << "  Zone(" << e->row << "," << e->col << ")"
                 << string(max(0, 14 - (int)to_string(e->temperature).length()), ' ') << "|" << endl;
            cout << "  +--------------------------------------------+" << endl;
            delete e;
        }
    }
}

// load_balancing() - Move excess events from q2 to q1 when q2 is overloaded
// Threshold = 5: if q2 has more than 5 events, redistribute to q1
// BUG FIX: parameters passed by REFERENCE so changes affect actual queues
// Time Complexity: O(n) where n = number of events moved from q2 to q1
void queue_layer::load_balancing(queue_layer &q1, queue_layer &q2)
{
    int threshold = 5;
    cout << "checking load balancing" << endl;
    while (q2.getsize() > threshold)
    {
        event_node* e = q2.dequeue();
        if(e != NULL){
            cout << "moving event from q2 to q1" << endl;
            q1.enqueue(e);
        }
    }
}