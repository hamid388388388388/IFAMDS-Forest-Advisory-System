#ifndef HASH_LAYER_H
#define HASH_LAYER_H

#include <iostream>
#include <string>
using namespace std;

// HASH LAYER - Index and Retrieval Acceleration
// Three hash structures as per project spec:
//   H1: Primary Index Table   - main zone data storage
//   H2: Collision Handling    - chaining for collisions
//   H3: Fast Retrieval Cache  - recently accessed data
//
// Hash Function: Index = Key mod TableSize  (from project spec)

#define H1_SIZE 10   // Primary table size
#define H3_SIZE 5    // Cache size (stores 5 most recent)

// ------ Zone Record (data stored at each hash slot) ------
struct zone_record
{
    int    zone_id;       // Zone number (0-8)
    int    temperature;   // Temperature reading
    int    humidity;      // Humidity reading
    int    smoke_lvl;     // Smoke level reading
    float  risk_score;    // Calculated risk level
    bool   is_occupied;   // True if this slot contains data
    string status;        // Status: "Normal", "Fire Risk", "Dry", etc.

    zone_record()
    {
        zone_id     = -1;
        temperature = 0;
        humidity    = 0;
        smoke_lvl   = 0;
        risk_score  = 0.0f;
        is_occupied = false;
        status      = "";
    }
};

// ------ Collision Chain Node (for H2) ------
// Used when two zone IDs hash to the same index - creates linked list
struct collision_node
{
    zone_record data;         // The zone data that collided
    collision_node *next;     // Next item in collision chain

    collision_node()
    {
        next = NULL;
    }
};

// ------ Cache Entry (for H3) ------
struct cache_entry
{
    int  zone_id;
    int  temperature;
    int  humidity;
    int  smoke_lvl;
    bool valid;         // True if this cache entry has valid data

    cache_entry()
    {
        zone_id     = -1;
        temperature = 0;
        humidity    = 0;
        smoke_lvl   = 0;
        valid       = false;
    }
};

// Hash Layer Class - Fast data indexing and retrieval
class hash_layer
{
private:
    // H1: Primary Index Table
    // Direct slot storage using hash function
    zone_record H1[H1_SIZE];

    // H2: Collision Handling Table
    // Linked list chains at each slot for overflow
    collision_node *H2[H1_SIZE];

    // H3: Fast Retrieval Cache
    // Circular array - newest entry replaces oldest
    cache_entry H3[H3_SIZE];
    int cache_next;   // points to next write position in cache

    // ---- Internal helpers ----
    // Hash Function: Index = Key mod TableSize  (from project spec)
    // Time Complexity: O(1)
    int hash_function(int zone_id);

    // Compute status string from sensor values
    string compute_status(int temp, int humidity, int smoke);

    // Compute risk score  (same formula as tree layer)
    float compute_risk(int temp, int smoke, int humidity);

    // Add to H3 cache
    void cache_insert(int zone_id, int temp, int hum, int smoke);

    // Search H3 cache
    bool cache_search(int zone_id);

public:
    hash_layer();

    // --- H1 Operations ---

    // Insert zone data into H1 primary table
    // If slot is free -> direct insert
    // If slot is taken -> goes to H2 collision chain
    // Time Complexity: O(1) average, O(n) worst case with collisions
    void insert(int zone_id, int temp, int humidity, int smoke);

    // Retrieve zone data by zone_id (O(1) direct access)
    // Checks H3 cache first, then H1, then H2 collision chain
    // Time Complexity: O(1) average
    void retrieve(int zone_id);

    // Update existing zone record with new sensor data
    // Time Complexity: O(1) average
    void update(int zone_id, int temp, int humidity, int smoke);

    // --- H2 Operations ---
    void display_collision_table();

    // --- H3 Operations ---
    void display_cache();
    void update_cache(int zone_id, int temp, int hum, int smoke);

    // --- Display ---
    void display_H1();
    void display_all();

    // Pre-load all 9 zones from sensor arrays (integration with array layer)
    // Time Complexity: O(n)
    void load_from_sensor_data(int temps[], int humids[], int smokes[], int count);
};

#endif