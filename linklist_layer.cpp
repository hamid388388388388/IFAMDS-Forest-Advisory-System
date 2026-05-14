#include "linklist_layer.h"

// SINGLY LINKED LIST LAYER - L1, L2, L3, L4 (forward chains)
// Used for: raw events, verified events, anomaly streams,
//           forward correction chains

// Constructor - initialize empty list
// Time Complexity: O(1)
linklist_layer::linklist_layer()
{
    head = NULL;
}

// gethead() - Return pointer to first node
// Time Complexity: O(1)
event_node *linklist_layer::gethead()
{
    return head;
}

// inserting_event() - Append a classified event node to end of singly linked list
// Used for raw event streams (L1) and anomaly streams (L3)
// Time Complexity: O(n) - traversal to find tail before inserting
void linklist_layer::inserting_event(string e, int v, int t, int r, int c)
{
    event_node *newnode = new event_node(e, v, t, r, c);
    if (head == NULL)
    {
        head = newnode;
    }
    else
    {
        event_node *temp = head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newnode;
    }
}

// inserting_verified_event_stream() - Append a sensor-value event to end of list
// Used for verified event streams (L2) and forward correction chains (L4)
// Time Complexity: O(n) - traversal to find tail before inserting
void linklist_layer::inserting_verified_event_stream(int t, int h, int s, int i, int r, int c)
{
    event_node *newnode = new event_node(t, h, s, i, r, c);
    if (head == NULL)
    {
        head = newnode;
    }
    else
    {
        event_node *temp = head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newnode;
    }
}

// display_event_list() - Print all events with type, value, time, and zone info
// Used to display raw event stream (L1) and anomaly stream (L3)
// Time Complexity: O(n) - single traversal through n nodes
void linklist_layer::display_event_list()
{
    event_node *temp = head;
    cout << "\n  +============================================+" << endl;
    cout << "  |           EVENT LIST (Singly)              |" << endl;
    cout << "  +============================================+" << endl;
    if (temp == NULL)
    {
        cout << "  |   [EMPTY] No events currently recorded.    |" << endl;
        cout << "  +============================================+" << endl;
        return;
    }
    while (temp != NULL)
    {
        cout << "  | Type  : " << temp->type
             << string(max(0,(int)(33 - temp->type.length())), ' ') << "|" << endl;
        cout << "  | Value : " << temp->val
             << string(33 - (int)to_string(temp->val).length(), ' ') << "|" << endl;
        cout << "  | Time  : " << temp->time
             << string(33 - (int)to_string(temp->time).length(), ' ') << "|" << endl;
        cout << "  | Zone  : (" << temp->row << "," << temp->col << ")"
             << string(29, ' ') << "|" << endl;
        cout << "  +--------------------------------------------+" << endl;
        temp = temp->next;
    }
}

// display_event_list_global() - Print all events showing sensor values (temp, humidity, smoke)
// Used to display the global synchronized state list (L6)
// Time Complexity: O(n) - single traversal through n nodes
void linklist_layer::display_event_list_global()
{
    event_node *temp = head;
    cout << "\n  +============================================+" << endl;
    cout << "  |        GLOBAL SYNCHRONIZED EVENT LIST      |" << endl;
    cout << "  +============================================+" << endl;
    if (temp == NULL)
    {
        cout << "  |   [EMPTY] No synced events available.      |" << endl;
        cout << "  +============================================+" << endl;
        return;
    }
    while (temp != NULL)
    {
        cout << "  |  Temp     : " << temp->temperature
             << " C" << string(29 - (int)to_string(temp->temperature).length(), ' ') << "|" << endl;
        cout << "  |  Humidity : " << temp->humidity
             << " %" << string(29 - (int)to_string(temp->humidity).length(), ' ') << "|" << endl;
        cout << "  |  Smoke    : " << temp->smoke_lvl
             << string(31 - (int)to_string(temp->smoke_lvl).length(), ' ') << "|" << endl;
        cout << "  |  Time     : " << temp->time
             << string(31 - (int)to_string(temp->time).length(), ' ') << "|" << endl;
        cout << "  |  Zone     : (" << temp->row << "," << temp->col << ")"
             << string(27, ' ') << "|" << endl;
        cout << "  +--------------------------------------------+" << endl;
        temp = temp->next;
    }
}

// display_verified_list() - Print verified sensor values with zone and time info
// Used to display verified event stream (L2)
// Time Complexity: O(n) - single traversal through n nodes
void linklist_layer::display_verified_list()
{
    event_node *temp = head;
    cout << "\n  +============================================+" << endl;
    cout << "  |         VERIFIED EVENT STREAM (L2)         |" << endl;
    cout << "  +============================================+" << endl;
    while (temp != NULL)
    {
        cout << "  |  Temp     : " << temp->temperature
             << " C" << string(29 - (int)to_string(temp->temperature).length(), ' ') << "|" << endl;
        cout << "  |  Humidity : " << temp->humidity
             << " %" << string(29 - (int)to_string(temp->humidity).length(), ' ') << "|" << endl;
        cout << "  |  Smoke    : " << temp->smoke_lvl
             << string(31 - (int)to_string(temp->smoke_lvl).length(), ' ') << "|" << endl;
        cout << "  |  Time     : " << temp->time
             << string(31 - (int)to_string(temp->time).length(), ' ') << "|" << endl;
        cout << "  |  Zone     : (" << temp->row << "," << temp->col << ")"
             << string(27, ' ') << "|" << endl;
        cout << "  +--------------------------------------------+" << endl;
        temp = temp->next;
    }
}

// display_forward_correction_chain() - Print the smoothed/corrected event sequence
// Used to display forward correction chain (L4) after anomaly smoothing
// Time Complexity: O(n) - single traversal through n nodes
void linklist_layer::display_forward_correction_chain()
{
    event_node *temp = head;
    cout << "\n  +============================================+" << endl;
    cout << "  |       FORWARD CORRECTION CHAIN (L4)        |" << endl;
    cout << "  +============================================+" << endl;
    while (temp != NULL)
    {
        cout << "  |  Temp     : " << temp->temperature
             << " C" << string(29 - (int)to_string(temp->temperature).length(), ' ') << "|" << endl;
        cout << "  |  Humidity : " << temp->humidity
             << " %" << string(29 - (int)to_string(temp->humidity).length(), ' ') << "|" << endl;
        cout << "  |  Smoke    : " << temp->smoke_lvl
             << string(31 - (int)to_string(temp->smoke_lvl).length(), ' ') << "|" << endl;
        cout << "  |  Time     : " << temp->time
             << string(31 - (int)to_string(temp->time).length(), ' ') << "|" << endl;
        cout << "  |  Zone     : (" << temp->row << "," << temp->col << ")"
             << string(27, ' ') << "|" << endl;
        cout << "  +--------------------------------------------+" << endl;
        temp = temp->next;
    }
}

// DOUBLY LINKED LIST LAYER - L4, L5, L6
// Used for: forward correction, backward correction, state sync
// Each node has both next and prev pointers for bidirectional traversal

// Constructor - initialize empty doubly list
// Time Complexity: O(1)
doubly_linklist_layer::doubly_linklist_layer()
{
    head = NULL;
    tail = NULL;
}

// gethead() - Return pointer to first node
// Time Complexity: O(1)
event_node *doubly_linklist_layer::gethead()
{
    return head;
}

// getTail() - Return pointer to last node
// Required for backward traversal (backward correction chain L5)
// Time Complexity: O(1)
event_node *doubly_linklist_layer::getTail()
{
    return tail;
}

// inserting_event() - Append classified event to end of doubly linked list
// Used for L4 (forward correction) and L5 (backward correction) chains
// Time Complexity: O(1) - tail pointer maintained so no traversal needed
void doubly_linklist_layer::inserting_event(string e, int v, int t, int r, int c)
{
    event_node *newnode = new event_node(e, v, t, r, c);
    if (head == NULL)
    {
        head = tail = newnode;
    }
    else
    {
        tail->next = newnode;
        newnode->prev = tail;
        tail = newnode;
    }
}

// inserting_event() (overload) - Append sensor-value event to doubly linked list
// Used when converting verified singly list to doubly list for backward correction
// Time Complexity: O(1) - tail pointer maintained so no traversal needed
void doubly_linklist_layer::inserting_event(int t, int h, int s, int i, int r, int c)
{
    event_node *newnode = new event_node(t, h, s, i, r, c);
    if (head == NULL)
    {
        head = tail = newnode;
    }
    else
    {
        tail->next = newnode;
        newnode->prev = tail;
        tail = newnode;
    }
}

// display_forward() - Traverse doubly list from head to tail and print each event
// Forward traversal: head -> node1 -> node2 -> ... -> tail
// Time Complexity: O(n) - single forward pass through n nodes
void doubly_linklist_layer::display_forward()
{
    cout << "\n  +============================================+" << endl;
    cout << "  |    DOUBLY LIST - FORWARD TRAVERSAL         |" << endl;
    cout << "  +============================================+" << endl;
    event_node *temp = head;
    if (temp == NULL)
    {
        cout << "  |   [EMPTY] Doubly list is empty.            |" << endl;
        cout << "  +============================================+" << endl;
        return;
    }
    while (temp != NULL)
    {
        cout << "  | Type  : " << temp->type
             << string(max(0,(int)(33 - temp->type.length())), ' ') << "|" << endl;
        cout << "  | Value : " << temp->val
             << string(33 - (int)to_string(temp->val).length(), ' ') << "|" << endl;
        cout << "  | Time  : " << temp->time
             << string(33 - (int)to_string(temp->time).length(), ' ') << "|" << endl;
        cout << "  | Zone  : (" << temp->row << "," << temp->col << ")"
             << string(29, ' ') << "|" << endl;
        cout << "  +--------------------------------------------+" << endl;
        temp = temp->next;
    }
}

void doubly_linklist_layer::display_backward()
{
    cout << "\n  +============================================+" << endl;
    cout << "  |    DOUBLY LIST - BACKWARD TRAVERSAL        |" << endl;
    cout << "  +============================================+" << endl;
    event_node *temp = tail;
    if (temp == NULL)
    {
        cout << "  |   [EMPTY] Doubly list is empty.            |" << endl;
        cout << "  +============================================+" << endl;
        return;
    }
    while (temp != NULL)
    {
        cout << "  | Type  : " << temp->type
             << string(max(0,(int)(33 - temp->type.length())), ' ') << "|" << endl;
        cout << "  | Value : " << temp->val
             << string(33 - (int)to_string(temp->val).length(), ' ') << "|" << endl;
        cout << "  | Time  : " << temp->time
             << string(33 - (int)to_string(temp->time).length(), ' ') << "|" << endl;
        cout << "  | Zone  : (" << temp->row << "," << temp->col << ")"
             << string(29, ' ') << "|" << endl;
        cout << "  +--------------------------------------------+" << endl;
        temp = temp->prev;
    }
}

// CIRCULAR LINKED LIST LAYER - L7, L8, L9, L10
// Last node points back to head - used for continuous monitoring loops
// Loop rule: tail->next = head (never NULL, loop never stops)

// Constructor - initialize empty circular list
// Time Complexity: O(1)
circular_linklist_layer::circular_linklist_layer()
{
    head = tail = NULL;
}

// inserting_event() - Append classified event and maintain circular linkage
// After every insert, tail->next = head to preserve the loop
// Time Complexity: O(1) - tail pointer maintained
void circular_linklist_layer::inserting_event(string e, int v, int t, int r, int c)
{
    event_node *newnode = new event_node(e, v, t, r, c);
    if (head == NULL)
    {
        head = tail = newnode;
        tail->next = head;
    }
    else
    {
        tail->next = newnode;
        newnode->next = head;
        tail = newnode;
    }
}

// inserting_event() (overload) - Append sensor-value event with circular linkage
// Used for global monitoring loop (L8) and synchronized state loop (L6)
// Time Complexity: O(1) - tail pointer maintained
void circular_linklist_layer::inserting_event(string e, int t, int h, int s, int i, int r, int c)
{
    event_node *newnode = new event_node(e, t, h, s, i, r, c);
    if (head == NULL)
    {
        head = tail = newnode;
        tail->next = head;
    }
    else
    {
        tail->next = newnode;
        newnode->next = head;
        tail = newnode;
    }
}

// monitor_single_zone() - Continuously monitor one specific zone for given number of cycles
// Traverses circular list and prints data only for matching (r,c) zone each cycle
// Implements L7: Local Monitoring Loop
// Time Complexity: O(cycles * n) where n = number of nodes in circular list
void circular_linklist_layer::monitor_single_zone(int r, int c, int cycles)
{
    cout << "\n  +============================================+" << endl;
    cout << "  |    LOCAL ZONE MONITOR  Zone(" << r << "," << c << ")           |" << endl;
    cout << "  +============================================+" << endl;
    event_node *temp = head;
    if (temp == NULL)
    {
        cout << "  |   [EMPTY] Circular list is empty.          |" << endl;
        cout << "  +============================================+" << endl;
        return;
    }
    for (int i = 0; i < cycles; i++)
    {
        cout << "  |  --- Cycle " << i+1 << " ---                           |" << endl;
        for (int j = 0; j < cycles; j++)
        {
            do
            {
                if (temp->row == r && temp->col == c)
                {
                    cout << "  |  Zone   : (" << r << "," << c << ")"
                         << string(29, ' ') << "|" << endl;
                    cout << "  |  Risk   : " << temp->val
                         << string(33 - (int)to_string(temp->val).length(), ' ') << "|" << endl;
                    cout << "  +--------------------------------------------+" << endl;
                }
                temp = temp->next;
            } while (temp != head);
        }
    }
}

// monitor_all_zones() - Scan ALL zones in the circular list for given number of cycles
// Implements L8: System-wide Monitoring Loop
// Time Complexity: O(cycles * n) where n = number of nodes in circular list
void circular_linklist_layer::monitor_all_zones(int cycles)
{
    if (head == NULL)
    {
        cout << "  +============================================+" << endl;
        cout << "  |   [EMPTY] Circular list is empty.          |" << endl;
        cout << "  +============================================+" << endl;
        return;
    }

    event_node *temp;

    for (int i = 0; i < cycles; i++)
    {
        cout << "\n  +============================================+" << endl;
        cout << "  |       ALL-ZONE MONITOR  -  Cycle " << i+1;
        cout << string(12 - (int)to_string(i+1).length(), ' ') << "|" << endl;
        cout << "  +============================================+" << endl;

        temp = head;

        do
        {
            string status = (temp->val >= 100) ? "[!! CRITICAL !!]" :
                            (temp->val >= 70)  ? "[HIGH RISK]     " :
                            (temp->val >= 40)  ? "[MODERATE]      " : "[NORMAL]        ";
            cout << "  | Zone  : (" << temp->row << "," << temp->col << ")"
                 << string(29, ' ') << "|" << endl;
            cout << "  | Event : " << temp->type
                 << string(max(0,(int)(33 - temp->type.length())), ' ') << "|" << endl;
            cout << "  | Risk  : " << temp->val << "  " << status
                 << string(max(0, 14 - (int)to_string(temp->val).length()), ' ') << "|" << endl;
            cout << "  +--------------------------------------------+" << endl;
            temp = temp->next;
        } while (temp != head);
    }
}

// emergency_monitoring_loop() - Activated when dangerous conditions detected
// Scans all zones each cycle, triggers alert if risk value exceeds threshold
// Implements L9: Emergency Monitoring Loop
// Time Complexity: O(cycles * n) where n = number of nodes in circular list
void circular_linklist_layer::emergency_monitoring_loop(int cycles)
{
    int threshold = 20;

    for (int i = 0; i < cycles; i++)
    {
        event_node *temp = head;
        do
        {
            if (temp->val > threshold)
            {
                cout << "\n  +============================================+" << endl;
                cout << "  |        !! EMERGENCY SYSTEM ACTIVATED !!    |" << endl;
                cout << "  +============================================+" << endl;
                cout << "  |  Zone     : (" << temp->row << " " << temp->col << ")"
                     << string(28, ' ') << "|" << endl;
                cout << "  |  Event    : " << temp->type
                     << string(max(0,(int)(29 - temp->type.length())), ' ') << "|" << endl;
                cout << "  |  Risk Val : " << temp->val
                     << string(29 - (int)to_string(temp->val).length(), ' ') << "|" << endl;
                cout << "  +============================================+" << endl;
            }
            temp = temp->next;
        } while (temp != head);
    }
}

// stability_monitoring_loop() - Monitor long-term stability of forest conditions
// Checks if |current - previous| < small_limit (10) to classify zones as STABLE/UNSTABLE
// Implements L10: Stability Monitoring Loop
// Time Complexity: O(cycles * n) where n = number of nodes in circular list
void circular_linklist_layer::stability_monitoring_loop(int cycles)
{
    if (head == NULL)
    {
        cout << "  +============================================+" << endl;
        cout << "  |  No data for stability monitoring.         |" << endl;
        cout << "  +============================================+" << endl;
        return;
    }

    int small_limit = 10;

    for (int i = 0; i < cycles; i++)
    {
        cout << "\n  +============================================+" << endl;
        cout << "  |    STABILITY MONITOR  -  Cycle " << i+1;
        cout << string(14 - (int)to_string(i+1).length(), ' ') << "|" << endl;
        cout << "  +============================================+" << endl;

        event_node *temp = head;
        event_node *prev = head;

        do
        {
            if (abs(temp->val - prev->val) < small_limit)
            {
                cout << "  |  [STABLE]   Zone (" << temp->row << "," << temp->col << ")"
                     << string(24, ' ') << "|" << endl;
                cout << "  |  Risk  : " << temp->val
                     << "    Event: " << temp->type
                     << string(max(0, 12 - (int)to_string(temp->val).length() - (int)temp->type.length()), ' ')
                     << "|" << endl;
            }
            else
            {
                cout << "  |  [UNSTABLE] Zone (" << temp->row << "," << temp->col << ")"
                     << string(23, ' ') << "|" << endl;
                cout << "  |  Change: " << abs(temp->val - prev->val)
                     << string(33 - (int)to_string(abs(temp->val - prev->val)).length(), ' ') << "|" << endl;
            }
            cout << "  +--------------------------------------------+" << endl;

            prev = temp;
            temp = temp->next;

        } while (temp != head);
    }
}

// display_circular_linklist() - Print all nodes in circular list (stops at head)
// Time Complexity: O(n) - single pass through n nodes
void circular_linklist_layer::display_circular_linklist()
{
    cout << "\n  +============================================+" << endl;
    cout << "  |         CIRCULAR LINKED LIST               |" << endl;
    cout << "  +============================================+" << endl;
    event_node *temp = head;
    if (temp == NULL)
    {
        cout << "  |   [EMPTY] Circular list is empty.          |" << endl;
        cout << "  +============================================+" << endl;
        return;
    }
    do
    {
        cout << "  | Type  : " << temp->type
             << string(max(0,(int)(33 - temp->type.length())), ' ') << "|" << endl;
        cout << "  | Value : " << temp->val
             << string(33 - (int)to_string(temp->val).length(), ' ') << "|" << endl;
        cout << "  | Time  : " << temp->time
             << string(33 - (int)to_string(temp->time).length(), ' ') << "|" << endl;
        cout << "  | Zone  : (" << temp->row << "," << temp->col << ")"
             << string(29, ' ') << "|" << endl;
        cout << "  +--------------------------------------------+" << endl;
        temp = temp->next;
    } while (temp != head);
}

// display_circular_global() - Print full sensor data from circular global loop
// Time Complexity: O(n) - single pass through n nodes
void circular_linklist_layer::display_circular_global()
{
    cout << "\n  +============================================+" << endl;
    cout << "  |    CIRCULAR GLOBAL SYNC LOOP (L8)          |" << endl;
    cout << "  +============================================+" << endl;
    event_node *temp = head;
    if (temp == NULL)
    {
        cout << "  |   [EMPTY] Circular list is empty.          |" << endl;
        cout << "  +============================================+" << endl;
        return;
    }
    do
    {
        cout << "  | Type  : " << temp->type
             << string(max(0,(int)(33 - temp->type.length())), ' ') << "|" << endl;
        cout << "  | Temp  : " << temp->temperature
             << " C" << string(29 - (int)to_string(temp->temperature).length(), ' ') << "|" << endl;
        cout << "  | Humid : " << temp->humidity
             << " %" << string(29 - (int)to_string(temp->humidity).length(), ' ') << "|" << endl;
        cout << "  | Smoke : " << temp->smoke_lvl
             << string(31 - (int)to_string(temp->smoke_lvl).length(), ' ') << "|" << endl;
        cout << "  | Time  : " << temp->time
             << string(31 - (int)to_string(temp->time).length(), ' ') << "|" << endl;
        cout << "  | Zone  : (" << temp->row << "," << temp->col << ")"
             << string(27, ' ') << "|" << endl;
        cout << "  +--------------------------------------------+" << endl;
        temp = temp->next;
    } while (temp != head);
}