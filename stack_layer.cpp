#include "stack_layer.h"
#include <iomanip>

// stack_layer Implementation
// Linked-node based stack for system state management

// Constructor - initialize empty stack
// Time Complexity: O(1)
stack_layer::stack_layer()
{
    top_ptr = nullptr;
    size = 0;
}

// Destructor - free all nodes
// Time Complexity: O(n)
stack_layer::~stack_layer()
{
    clear_stack();
}

// push() - Save a new system state onto the stack
// Time Complexity: O(1)
// Used when system wants to save current zone state before risky operation
void stack_layer::push(int zone_id, float temp, float smoke, float humidity, string label)
{
    StackNode *new_node = new StackNode(zone_id, temp, smoke, humidity, label);
    new_node->next = top_ptr;
    top_ptr = new_node;
    size++;
    cout << "  [STACK] State saved -> Zone " << zone_id
         << " | Label: " << label << endl;
}

// pop() - Remove top state from stack (discard it)
// Time Complexity: O(1)
void stack_layer::pop()
{
    if (is_empty())
    {
        cout << "  [STACK] Stack is empty. Nothing to pop." << endl;
        return;
    }
    StackNode *temp = top_ptr;
    top_ptr = top_ptr->next;
    delete temp;
    size--;
}

// peek() - Look at top state without removing it
// Time Complexity: O(1)
// Returns true if successful, false if empty
bool stack_layer::peek(int &zone_id, float &temp, float &smoke, float &humidity, string &label)
{
    if (is_empty())
        return false;
    zone_id = top_ptr->zone_id;
    temp = top_ptr->temperature;
    smoke = top_ptr->smoke;
    humidity = top_ptr->humidity;
    label = top_ptr->state_label;
    return true;
}

// rollback() - Restore last saved state and remove it from stack
// Time Complexity: O(1)
// This is the main use case: undo to last safe state
bool stack_layer::rollback(int &zone_id, float &temp, float &smoke,
                           float &humidity, string &label)
{
    if (is_empty())
    {
        cout << "  [STACK] No saved states to rollback to!" << endl;
        return false;
    }
    zone_id = top_ptr->zone_id;
    temp = top_ptr->temperature;
    smoke = top_ptr->smoke;
    humidity = top_ptr->humidity;
    label = top_ptr->state_label;
    pop();
    cout << "  [STACK] Rollback successful -> Restored Zone " << zone_id
         << " state: " << label << endl;
    return true;
}

// is_empty() - Check if stack has no states
// Time Complexity: O(1)
bool stack_layer::is_empty()
{
    return top_ptr == nullptr;
}

// get_size() - Return number of saved states
// Time Complexity: O(1)
int stack_layer::get_size()
{
    return size;
}

// display_stack() - Print all saved states from top to bottom
// Time Complexity: O(n)
void stack_layer::display_stack()
{
    if (is_empty())
    {
        cout << "  [STACK] No states saved in stack." << endl;
        return;
    }
    cout << "\n  ============ SAVED STATES (Top -> Bottom) ============" << endl;
    cout << "  " << left << setw(6) << "Pos"
         << setw(8) << "Zone"
         << setw(10) << "Temp"
         << setw(10) << "Smoke"
         << setw(10) << "Humidity"
         << "Label" << endl;
    cout << "  ------------------------------------------------------" << endl;

    StackNode *curr = top_ptr;
    int pos = 1;
    while (curr != nullptr)
    {
        cout << "  " << setw(6) << pos++
             << setw(8) << curr->zone_id
             << setw(10) << curr->temperature
             << setw(10) << curr->smoke
             << setw(10) << curr->humidity
             << curr->state_label << endl;
        curr = curr->next;
    }
    cout << "  ======================================================" << endl;
    cout << "  Total saved states: " << size << endl;
}

// clear_stack() - Remove all saved states
// Time Complexity: O(n)
void stack_layer::clear_stack()
{
    while (!is_empty())
    {
        pop();
    }
}

// dfs_stack Implementation
// Simple stack for iterative DFS graph traversal

// Constructor
// Time Complexity: O(1)
dfs_stack::dfs_stack()
{
    top_ptr = nullptr;
}

// Destructor
// Time Complexity: O(n)
dfs_stack::~dfs_stack()
{
    clear();
}

// push() - Add zone to DFS stack
// Time Complexity: O(1)
void dfs_stack::push(int zone_id)
{
    DFSNode *node = new DFSNode(zone_id);
    node->next = top_ptr;
    top_ptr = node;
}

// pop() - Remove and return top zone ID
// Time Complexity: O(1)
// Returns -1 if empty
int dfs_stack::pop()
{
    if (is_empty())
        return -1;
    DFSNode *temp = top_ptr;
    int val = temp->zone_id;
    top_ptr = top_ptr->next;
    delete temp;
    return val;
}

// peek() - View top zone without removing
// Time Complexity: O(1)
int dfs_stack::peek()
{
    if (is_empty())
        return -1;
    return top_ptr->zone_id;
}

// is_empty()
// Time Complexity: O(1)
bool dfs_stack::is_empty()
{
    return top_ptr == nullptr;
}

// clear() - Empty the DFS stack
// Time Complexity: O(n)
void dfs_stack::clear()
{
    while (!is_empty())
        pop();
}