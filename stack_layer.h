#ifndef STACK_LAYER_H
#define STACK_LAYER_H

#include <iostream>
#include <string>
using namespace std;

// STACK LAYER - Execution Control & Rollback System
// Department 3: Computational Reasoning Layer
// Uses: State tracking, rollback, iterative DFS traversal

// Single node of the stack (linked-node based stack)
struct StackNode {
    int   zone_id;        // Forest zone this state belongs to
    float temperature;    // Saved temperature value
    float smoke;          // Saved smoke level
    float humidity;       // Saved humidity value
    string state_label;   // Label describing this state (e.g. "PRE_FIRE")
    StackNode* next;      // Pointer to node below in stack

    StackNode(int z, float t, float s, float h, string label)
        : zone_id(z), temperature(t), smoke(s), humidity(h),
          state_label(label), next(nullptr) {}
};

// Stack Layer Class - Linked-node based stack for saving/restoring system states
class stack_layer {
private:
    StackNode* top_ptr;   // Points to top of stack
    int        size;      // Current number of elements in stack

public:
    // Constructor / Destructor
    stack_layer();  // Create empty stack
    ~stack_layer(); // Clean up memory when stack is destroyed

    // Core stack operations - Last In, First Out (LIFO)
    void push(int zone_id, float temp, float smoke, float humidity, string label); // Add state to top - O(1)
    void pop();                        // Remove top state - O(1)
    bool peek(int &zone_id, float &temp, float &smoke, float &humidity, string &label); // Look at top without removing - O(1)

    // Utility functions
    bool  is_empty();                  // Check if stack has no states - O(1)
    int   get_size();                  // Get number of saved states - O(1)
    void  display_stack();             // Show all saved states - O(n)
    void  clear_stack();               // Remove all states - O(n)

    // Rollback operation - restore system to last saved state
    bool rollback(int &zone_id, float &temp, float &smoke,
                  float &humidity, string &label);           // Restore last state - O(1)
};

// DFS Stack - Used for iterative graph DFS traversal
// Stores zone IDs to visit during depth-first search
struct DFSNode {
    int     zone_id;     // Zone to visit
    DFSNode* next;       // Next zone in stack
    DFSNode(int z) : zone_id(z), next(nullptr) {}
};

class dfs_stack {
private:
    DFSNode* top_ptr;  // Top of DFS stack

public:
    dfs_stack();   // Create empty DFS stack
    ~dfs_stack();  // Clean up memory

    void push(int zone_id);   // Add zone to visit - O(1)
    int  pop();               // Get next zone to visit, returns -1 if empty - O(1)
    int  peek();              // Look at next zone without removing - O(1)
    bool is_empty();          // Check if no zones left to visit - O(1)
    void clear();             // Clear all zones from stack
};

#endif