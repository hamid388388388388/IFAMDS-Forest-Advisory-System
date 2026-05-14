#ifndef LINKLIST_LAYER_H
#define LINKLIST_LAYER_H

#include <iostream>
#include <cmath>
#include <string>
using namespace std;

//-------------------------- Link list layer ---------------------
// This file contains different types of linked lists for storing forest events
// Events can be fire, smoke, or dry conditions

// Basic node for storing event data
class event_node
{
public:
    event_node *next;     // Points to next event in list
    event_node *prev;     // Points to previous event (for doubly linked)
    string type;          // Type of event (fire, smoke, dry)
    int temperature;      // Temperature reading
    int humidity;         // Humidity reading
    int smoke_lvl;        // Smoke level reading
    int val;              // General value field
    int time;             // Time when event occurred
    int row, col;         // Zone location (row, column)

    // Constructor for basic sensor data
    event_node(int t, int h, int s, int i, int r, int c)
    {
        prev = NULL;
        next = NULL;
        temperature = t;
        humidity = h;
        smoke_lvl = s;
        time = i;
        row = r;
        col = c;
    }

    // Constructor with event type
    event_node(string e, int t, int h, int s, int i, int r, int c)
    {
        prev = NULL;
        next = NULL;
        type = e;
        temperature = t;
        humidity = h;
        smoke_lvl = s;
        time = i;
        row = r;
        col = c;
    }

    // Constructor for simple value events
    event_node(string e, int v, int t, int r, int c)
    {
        prev = NULL;
        next = NULL;
        val = v;
        time = t;
        row = r;
        col = c;
        type = e;
    }
};

//*****************************--Singly Linked List Layer----------*/
// Stores events in one direction only (forward)
class linklist_layer
{
private:
    event_node *head;  // Start of the list

public:
    linklist_layer();  // Constructor - creates empty list
    event_node *gethead();  // Get the first node
    void inserting_event(string e, int v, int t, int r, int c);  // Add new event to list
    void inserting_verified_event_stream(int t, int h, int s, int i, int r, int c);  // Add verified sensor data
    void display_event_list();  // Show all events in the list
    void display_event_list_global();  // Show global synchronized events
    void display_verified_list();  // Show only verified (good) events
    void display_forward_correction_chain();  // Show corrected future events
};

//___________________---------- Doubly Linked List Layer--------------
// Can move forward and backward through events
class doubly_linklist_layer
{
    event_node *head;  // Start of the list
    event_node *tail;  // End of the list

public:
    doubly_linklist_layer();  // Constructor - creates empty list
    event_node *gethead();    // Get the first node
    event_node *getTail();    // Get the last node
    void inserting_event(string e, int v, int t, int r, int c);  // Add event with value
    void inserting_event(int t, int h, int s, int i, int r, int c);  // Add sensor data event
    void display_forward();   // Show events from start to end
    void display_backward();  // Show events from end to start
};

//------------- Circular Linked List Layer --------------
// Last node connects back to first node - creates a loop
class circular_linklist_layer
{
    event_node *head;  // Start of the circular list
    event_node *tail;  // End of the circular list (connects to head)

public:
    circular_linklist_layer();  // Constructor - creates empty circular list
    void inserting_event(string e, int v, int t, int r, int c);  // Add simple event
    void inserting_event(string e, int t, int h, int s, int i, int r, int c);  // Add full sensor event
    void monitor_single_zone(int r, int c, int cycles);  // Monitor one zone repeatedly
    void monitor_all_zones(int cycles);  // Monitor all zones in loop
    void emergency_monitoring_loop(int cycles);  // Fast monitoring when danger detected
    void stability_monitoring_loop(int cycles);  // Check long-term stability
    void display_circular_linklist();  // Show the circular list
    void display_circular_global();    // Show global circular events
};

#endif
