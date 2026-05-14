#ifndef ARRAYLAYER_H
#define ARRAYLAYER_H

#include <iostream>
#include <cmath>
#include <string>
#include "linklist_layer.h"
using namespace std;

// Array Layer class - handles sensor data storage and processing
// Uses arrays to store temperature, humidity, smoke readings from forest zones
class arraylayer
{
private:
    // Fixed reference values for normal forest conditions
    int base_temp[5] = {5, 5, 5, 5, 5};        // Normal temperatures
    int base_humidity[5] = {6, 6, 6, 6, 6};    // Normal humidity levels
    int base_smoke_lvl[5] = {1, 1, 1, 1, 1};   // Normal smoke levels

    // Dynamic arrays for live sensor readings
    int sensor_temp[100];       // Stores temperature readings
    int sensor_humidity[100];   // Stores humidity readings
    int sensor_smoke_lvl[100];  // Stores smoke level readings
    int size;                   // Number of readings stored

    // Static grid - fixed reference map of forest zones
    int staticgrid[3][3] = {
        {25, 30, 28},
        {27, 45, 32},
        {26, 29, 31}};

    // Dynamic grid - calculated risk values for each zone
    int dynamicgrid[3][3];

public:
    arraylayer();  // Constructor - sets up the arrays

    // Basic sensor data operations
    void add_sensor_data(int t, int h, int s);  // Add new temperature, humidity, smoke reading
    void display_sensor_data();                 // Show all stored sensor readings
    void display_static_grid();                 // Show the fixed reference grid

    // Risk calculation and grid operations
    int calculate_risk(int t, int s, int h);   // Calculate risk score for a zone
    void auto_fill_dynamic_grid();             // Fill grid with calculated risk values
    void display_dynamic_grid();               // Show the dynamic risk grid
    void autoInterpolate(int r, int c);        // Estimate missing values using neighbors

    // Anomaly and boundary detection
    void check_anomilies();    // Find readings that are too high (fire risk)
    void check_boundary();     // Find sharp changes between nearby zones

    // Event generation for linked lists
    void generate_events(linklist_layer &elist);                    // Create events for singly linked list
    void generate_events_doubly(doubly_linklist_layer &dlist);      // Create events for doubly linked list
    void generate_events_circular(circular_linklist_layer &clist);  // Create events for circular linked list

    // Data processing and correction
    void verified_event_stream(linklist_layer &llist);              // Filter out bad readings
    void generate_anomaly_stream(linklist_layer &llist, linklist_layer &anomaly_list);  // Find dangerous events
    void forward_correction_chain(linklist_layer &verified_list, linklist_layer &forward_correcction_list);  // Fix future events
    void convert_verify_singly_to_doubly(linklist_layer &llist, doubly_linklist_layer &doubly_verified_list);  // Convert list types
    void backward_correction_chain(doubly_linklist_layer &dlist);   // Fix past events by going backwards
    void state_synchronization(doubly_linklist_layer &source, linklist_layer &global_list, circular_linklist_layer &global_loop);  // Make all systems consistent

    // Utility
    int getsize() { return size; }  // Get number of readings stored
};

#endif
