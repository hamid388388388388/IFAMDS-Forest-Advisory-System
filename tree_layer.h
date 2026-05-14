#ifndef TREE_LAYER_H
#define TREE_LAYER_H

#include <iostream>
#include <string>
using namespace std;

//-------------------- Tree Node --------------------
// Each node represents a zone, resource, or decision point in the forest monitoring system
// Contains sensor data, risk calculations, and tree structure pointers

class tree_node
{
public:
    // --- identity ---
    string zone_name;            // Name of zone, resource, event, or action

    // --- sensor readings (T1, T2, T6, T7, T10, T11, T12) ---
    int temperature;             // Temperature reading from sensors
    int humidity;                // Humidity reading from sensors
    int smoke_lvl;               // Smoke level reading from sensors

    // --- general risk score (T1, T2, T10, T11, T12) ---
    // Formula: 0.4*(fire_factor) + 0.3*(smoke_factor) + 0.3*(humidity_penalty)
    float risk_score;            // Overall risk level for this zone

    // --- terrain specific fields (T3) ---
    float slope;                 // Slope factor (0 to 1) - steeper = higher risk
    float dryness;               // Dryness factor (0 to 1) - drier = higher risk
    float vegetation_density;    // Vegetation density factor (0 to 1)
    float terrain_risk;          // Average of slope + dryness + vegetation_density

    // --- water resource fields (T4) ---
    float available_water;       // Liters of water available
    float required_water;        // Liters of water needed
    float water_availability;    // Ratio: available_water / required_water

    // --- fire control fields (T5) ---
    float readiness_level;       // Team/tool readiness level (0 to 1)

    // --- equipment allocation fields (T6) ---
    float impact_factor;         // Zone impact level (0 to 1)
    float priority_score;        // Risk score multiplied by impact factor

    // --- fire classification fields (T7) ---
    float fire_score;            // Fire intensity: alpha*(temp/100) + beta*(smoke/100)

    // --- wildlife fields (T8) ---
    float movement_score;        // Wildlife movement intensity (0 to 1)

    // --- human activity fields (T9) ---
    float movement_factor;           // Human movement intensity (0 to 1)
    float restricted_area_factor;    // How restricted this area is (0 to 1)
    float human_risk;                // Movement factor times restricted area factor

    // --- regional spread field (T11) ---
    float fire_spread_rate;      // How fast fire is spreading (0 to 1)

    // --- tree structure ---
    tree_node* children[4];      // Up to 4 children for general trees
    int        child_count;      // Number of children this node has
    tree_node* left;             // YES branch for binary decision trees
    tree_node* right;            // NO branch for binary decision trees

    // Constructor - sets all numeric fields to 0 and pointers to NULL
    tree_node(string name)
    {
        zone_name              = name;
        temperature            = 0;
        humidity               = 0;
        smoke_lvl              = 0;
        risk_score             = 0.0;
        slope                  = 0.0;
        dryness                = 0.0;
        vegetation_density     = 0.0;
        terrain_risk           = 0.0;
        available_water        = 0.0;
        required_water         = 0.0;
        water_availability     = 0.0;
        readiness_level        = 0.0;
        impact_factor          = 0.0;
        priority_score         = 0.0;
        fire_score             = 0.0;
        movement_score         = 0.0;
        movement_factor        = 0.0;
        restricted_area_factor = 0.0;
        human_risk             = 0.0;
        fire_spread_rate       = 0.0;
        child_count            = 0;
        left                   = NULL;
        right                  = NULL;
        for (int i = 0; i < 4; i++)
            children[i] = NULL;
    }
};

//-------------------- Tree Layer --------------------
// Manages different types of trees for forest monitoring decisions
// Each tree type handles different aspects of fire prevention and response

class tree_layer
{
private:
    // Structural Trees - organize the forest into zones and sub-zones
    tree_node* T1_zone_hierarchy;        // Forest -> Zone A -> Zone A1 -> Zone A1-1
    tree_node* T2_subzone_decomposition; // Each zone -> N, S, E, W sub-zones
    tree_node* T3_terrain_classification;// Land -> High/Medium/Low risk terrain

    // Resource Trees - manage water, teams, and equipment
    tree_node* T4_water_resource;        // Water HQ -> Zone Tanks -> Sources
    tree_node* T5_fire_control;          // Fire HQ -> Teams -> Tools
    tree_node* T6_equipment_allocation;  // Equipment Pool -> Zone Priorities

    // Incident Trees - classify fires and monitor activity
    tree_node* T7_fire_classification;   // Fire Root -> Major/Moderate/Low/None
    tree_node* T8_wildlife_activity;     // Wildlife -> Normal/Unusual/Mass Flee
    tree_node* T9_human_activity;        // Human -> Authorized/Unauthorized/Breach

    // Decision Trees (Binary) - make automated decisions
    tree_node* T10_local_decision;       // Zone Check -> Activate/Monitor
    tree_node* T11_regional_escalation;  // Region Check -> Escalate/Keep Local
    tree_node* T12_global_emergency;     // Global Check -> Global Alert/Standby

    // Internal helper functions
    void add_child(tree_node* parent, tree_node* child);  // Add child to parent node
    void display_tree(tree_node* root, int level);        // Show tree structure

public:
    tree_layer();  // Constructor - creates empty trees

    // Decision score formula from project spec
    // Score = 0.4*(temp/100) + 0.3*(smoke/100) + 0.3*((100-humidity)/100)
    float compute_decision_score(int temperature, int smoke_lvl, int humidity);

    // Build functions - create each type of tree
    void build_T1_zone_hierarchy();        // Build zone organization tree
    void build_T2_subzone_decomposition(); // Build sub-zone breakdown tree
    void build_T3_terrain_classification(); // Build terrain risk tree
    void build_T4_water_resource();         // Build water resource tree
    void build_T5_fire_control();           // Build fire control team tree
    void build_T6_equipment_allocation();   // Build equipment priority tree
    void build_T7_fire_classification();    // Build fire severity tree
    void build_T8_wildlife_activity();      // Build wildlife monitoring tree
    void build_T9_human_activity();         // Build human activity tree
    void build_T10_local_decision();        // Build local decision tree
    void build_T11_regional_escalation();   // Build regional escalation tree
    void build_T12_global_emergency();      // Build global emergency tree

    // Display functions - show each tree structure
    void show_T1();   // Display zone hierarchy
    void show_T2();   // Display sub-zone decomposition
    void show_T3();   // Display terrain classification
    void show_T4();   // Display water resources
    void show_T5();   // Display fire control structure
    void show_T6();   // Display equipment allocation
    void show_T7();   // Display fire classification
    void show_T8();   // Display wildlife activity
    void show_T9();   // Display human activity
    void show_T10();  // Display local decisions
    void show_T11();  // Display regional escalation
    void show_T12();  // Display global emergency decisions

    // Decision functions - make automated choices
    void compute_risk_score_manual(int temperature, int humidity, int smoke_lvl);  // Manual risk calculation
    void zone_level_decision();      // Decide what to do for a zone
    void regional_decision();        // Decide if region needs help
    void global_emergency_decision(); // Decide if global alert needed
    void execute_final_action();     // Take the final action based on decisions
};

#endif