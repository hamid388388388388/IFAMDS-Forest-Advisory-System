#ifndef MONITORING_LAYER_H
#define MONITORING_LAYER_H

#include <iostream>
#include <string>
#include <ctime>
using namespace std;

#define TOTAL_MODULES 6  // Number of data structure modules being monitored

// Structure to track performance statistics for each module
struct module_stats
{
    string module_name;        // Name of the module (Array, Linked List, etc.)
    int    active_tasks;       // Number of tasks currently running
    int    processing_capacity; // Maximum tasks this module can handle
    float  load_percent;       // Current load as percentage (0-100%)
    float  latency_ms;         // Response time in milliseconds
    bool   is_bottleneck;      // True if this module is slowing down the system
    string health_status;      // "Healthy", "Warning", "Critical"

    module_stats()
    {
        module_name         = "";
        active_tasks        = 0;
        processing_capacity = 10;
        load_percent        = 0.0f;
        latency_ms          = 0.0f;
        is_bottleneck       = false;
        health_status       = "Healthy";
    }
};

// Monitoring Layer Class - Tracks system performance and health
class monitoring_layer
{
private:
    module_stats modules[TOTAL_MODULES];  // Stats for each data structure module
    clock_t      start_clock;             // Timer start time
    float        recorded_latency_ms;     // Last measured latency

    // Internal helper functions
    string compute_health(float load_percent);  // Determine health status from load
    int    find_bottleneck_index();             // Find which module is the bottleneck

public:
    monitoring_layer();  // Constructor - initializes monitoring system

    // Load current task counts from all active data structure layers
    // Time Complexity: O(n) where n = number of modules
    void update_module_stats(int array_tasks, int ll_tasks, int queue_tasks,
                              int tree_tasks,  int graph_tasks, int hash_tasks);

    // Measure operation execution time
    // Latency = Finish Time - Start Time
    // Time Complexity: O(1)
    void start_timer();                    // Start timing an operation
    void stop_timer(string operation_name); // Stop timer and record latency

    // System monitoring functions
    // Time Complexity: O(n) where n = TOTAL_MODULES
    void monitor_system_load();     // Check load on all modules
    void track_execution_time();    // Show timing statistics
    void detect_bottlenecks();      // Find modules that are overloaded
    void optimize_performance();    // Suggest performance improvements
    void view_system_health();      // Display overall system status
};

#endif