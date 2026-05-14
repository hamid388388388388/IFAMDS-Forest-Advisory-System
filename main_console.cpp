#include <iostream>
#include <cmath>
#include <string>
#include <limits>  // for numeric_limits
#include "linklist_layer.h"
#include "queue_layer.h"
#include "arraylayer.h"
#include "tree_layer.h"
#include "graph_layer.h"
#include "hash_layer.h"
#include "monitoring_layer.h"
#include "stack_layer.h"
#include "validation_helpers.h"
using namespace std;

// VALIDATION HELPERS NOW IMPORTED FROM validation_helpers.h

// MENU 1: INPUT ENVIRONMENTAL DATA

// MAIN FUNCTION - Program Entry Point
// This is the main menu system for IFAMDS forest monitoring system
// Handles user input and calls different data structure operations
int console_main()
{
    // Create objects for all data structures used in the system
    arraylayer obj;  // Handles sensor data arrays

    linklist_layer raw_list;  // Stores raw sensor events
    linklist_layer verified_list;  // Stores verified (filtered) events
    linklist_layer anomaly_list;  // Stores abnormal events
    linklist_layer forward_correcction_list;  // Stores corrected events
    linklist_layer elist;  // General event list

    doubly_linklist_layer dlist;  // Doubly linked list for bidirectional traversal
    bool dlist_loaded = false;  // Flag to check if doubly list is initialized
    bool global_loaded = false;  // Flag for global state loading
    bool clist_loaded = false;  // Flag for circular list loading
    bool queue_paused = false; // Pause/Resume flag for Task Scheduling System
    doubly_linklist_layer doubly_verified_list;

    circular_linklist_layer clist;
    linklist_layer global_list;
    circular_linklist_layer global_loop;

    queue_layer q1_routing;
    queue_layer q2_surviellance;
    queue_layer q3_emergency;
    queue_layer q4_decision;

    tree_layer tobj;
    graph_layer gobj;
    hash_layer hobj;

    monitoring_layer mobj;
    stack_layer sobj; // Execution control stack (Dept 3 - rollback states)

    // PRE-LOAD SENSOR DATA
    // Load initial sensor readings for 9 forest zones
    // Temperature, Humidity, Smoke values for testing
    obj.add_sensor_data(50, 10, 80);
    obj.add_sensor_data(50, 30, 80);
    obj.add_sensor_data(50, 10, 20);
    obj.add_sensor_data(50, 40, 20);
    obj.add_sensor_data(30, 40, 80);
    obj.add_sensor_data(30, 10, 20);
    obj.add_sensor_data(55, 12, 85);
    obj.add_sensor_data(28, 60, 40);
    obj.add_sensor_data(42, 22, 65);

    // Arrays to store the sensor data for graph and hash initialization
    int g_temps[] = {50, 50, 50, 50, 30, 30, 55, 28, 42};
    int g_humids[] = {10, 30, 10, 40, 40, 10, 12, 60, 22};
    int g_smokes[] = {80, 80, 20, 20, 80, 20, 85, 40, 65};

    // Initialize all system layers and data structures
    cout << "\n  [IFAMDS] Initializing system layers..." << endl;
    gobj.load_zone_data(g_temps, g_humids, g_smokes, 9);  // Load data into graph
    gobj.build_adjacency_list();  // Create graph connections
    gobj.build_adjacency_matrix();  // Build matrix representation
    hobj.load_from_sensor_data(g_temps, g_humids, g_smokes, 9);  // Initialize hash table
    cout << "  [IFAMDS] All layers ready.\n"
         << endl;

    // MENU SYSTEM
    // Main program loop - displays menu and processes user choices
    // Continues until user enters 0 to exit
    int main_choice;
    do
    {
        // Display main menu header
        cout << "\n  ==========================================" << endl;
        cout << "    IFAMDS - Intelligent Forest Advisory    " << endl;
        cout << "       Multi-Structure Decision System      " << endl;
        cout << "  ==========================================" << endl;
        cout << "   1.  Input Environmental Data" << endl;
        cout << "   2.  View Forest Grid Status" << endl;
        cout << "   3.  Event Memory System (Linked Lists)" << endl;
        cout << "   4.  Fire Detection and Control" << endl;
        cout << "   5.  Task Scheduling System (Queues)" << endl;
        cout << "   6.  Decision System (Trees)" << endl;
        cout << "   7.  Spatial Routing System (Graphs)" << endl;
        cout << "   8.  Hash-Based Fast Access System" << endl;
        cout << "   9.  System Monitoring" << endl;
        cout << "   10. Scenario Simulation" << endl;
        cout << "   11. Execution Control Stack (Dept 3)" << endl;
        cout << "   0.  Exit System" << endl;
        cout << "  ==========================================" << endl;
        cout << "  Enter choice: ";
        main_choice = get_valid_int("", 0, 11);

        // MENU 1
        if (main_choice == 1)
        {
            try {
                int sub;
                cout << "\n  --- [Menu 1] Input Environmental Data ---" << endl;
                cout << "   1 = Add Sensor Reading (Temperature, Humidity, Smoke)" << endl;
                cout << "   2 = Store Data in Dynamic Array" << endl;
                cout << "   3 = Compare with Static Baseline" << endl;
                cout << "   4 = Validate and Filter Noise" << endl;
                cout << "  Enter sub-choice: ";
                sub = get_valid_int("", 1, 4);

                if (sub == 1)
                {
                    // Add a new sensor reading into dynamic arrays
                    int t = get_valid_int("  Enter Temperature (0-100): ", 0, 100);
                    int h = get_valid_int("  Enter Humidity (0-100): ", 0, 100);
                    int s = get_valid_int("  Enter Smoke Level (0-100): ", 0, 100);
                    
                    // Validate and show warnings
                    SensorReading sr = validate_sensor_data(t, h, s, obj.getsize());
                    
                    if (check_array_capacity(obj.getsize())) {
                        obj.add_sensor_data(t, h, s);
                        print_header("SENSOR READING ADDED  [A2: Dynamic Sensor Stream]");
                        cout << "   Temp=" << t << "  Humidity=" << h << "  Smoke=" << s << endl;
                        print_sensor_warnings(sr);
                        cout << "  Reading stored in Dynamic Array A2." << endl;
                    } else {
                        print_error("Cannot add reading - array full!");
                    }
                }
            else if (sub == 2)
            {
                // 1.2 - Store sensor readings into 2D Dynamic Terrain Grid (A4)
                // Maps 1D sensor array into 2D spatial grid using risk scores
                print_header("A4: STORE DATA IN DYNAMIC ARRAY  [2D Terrain Grid]");
                cout << "  Mapping sensor readings into 2D Dynamic Terrain Grid..." << endl;
                obj.auto_fill_dynamic_grid();
                cout << "  Dynamic grid filled with risk scores (Temp + Smoke - Humidity)." << endl;
                cout << "\n  Displaying A4 Dynamic Terrain Expansion Matrix:" << endl;
                obj.display_dynamic_grid();
                cout << "\n  Raw sensor stream (A2) also stored:" << endl;
                obj.display_sensor_data();
            }
            else if (sub == 3)
            {
                // 1.3 - Compare live sensor data against static baseline (A1)
                print_header("A1 vs A2: COMPARE WITH STATIC BASELINE  [Normal: Temp=25 Hum=60 Smoke=0]");
                cout << "  Static Baseline Grid (A1 - fixed reference values):" << endl;
                obj.display_static_grid();
                cout << "\n  Live Sensor Stream (A2 - dynamic values):" << endl;
                obj.display_sensor_data();
            }
            else if (sub == 4)
            {
                // 1.4 - Validate readings and filter noise/anomalies
                print_header("VALIDATE AND FILTER NOISE  [Threshold: Temp>45 | Smoke>70 | Hum<20]");
                cout << "  Running anomaly detection..." << endl;
                obj.check_anomilies();
                print_header("BOUNDARY DETECTION  [Sharp value change > 25 between zones]");
                obj.check_boundary();
            }
            else
            {
                cout << "  Invalid sub-choice." << endl;
            }
            } catch (const exception& e) {
                cout << "  Error in Menu 1: " << e.what() << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        // MENU 2
        else if (main_choice == 2)
        {
            cout << "\n  --- [Menu 2] View Forest Grid Status ---" << endl;
            cout << "   1 = Display 1D Time Series Data" << endl;
            cout << "   2 = Display 2D Forest Zone Matrix" << endl;
            cout << "   3 = View Zone-wise Conditions" << endl;
            int sub = get_valid_int("  Enter sub-choice: ", 1, 3);

            if (sub == 1)
            {
                print_header("A2: 1D SENSOR DATA  [Time Series - Sequential]");
                obj.display_sensor_data();
            }
            else if (sub == 2)
            {
                print_header("A3/A4: 2D DYNAMIC FOREST GRID  [Spatial Matrix]");
                cout << "  Each cell = combined risk value of that zone" << endl;
                print_divider();
                obj.auto_fill_dynamic_grid();
                obj.display_dynamic_grid();
            }
            else if (sub == 3)
            {
                print_header("ZONE-WISE CONDITIONS  [Anomaly + Boundary]");
                obj.check_anomilies();
                print_divider();
                obj.check_boundary();
            }
            else
            {
                cout << "  Invalid sub-choice." << endl;
            }
        }

        // MENU 3
        else if (main_choice == 3)
        {
            cout << "\n  --- [Menu 3] Event Memory System (Linked Lists) ---" << endl;
            cout << "   1 = L1-L4: Raw / Verified / Anomaly / Forward Correction" << endl;
            cout << "   2 = Doubly LL - Forward Traversal  [--->]" << endl;
            cout << "   3 = Doubly LL - Backward Traversal [<---]" << endl;
            cout << "   4 = L7-L10: Circular Monitoring Loops" << endl;
            cout << "   5 = L5+L6: Backward Correction + State Sync" << endl;
            int sub = get_valid_int("  Enter sub-choice: ", 1, 5);

            if (sub == 1)
            {
                print_header("L1: RAW EVENT STREAM  [Singly Linked - Direct Sensor Readings]");
                cout << "  Storing raw readings without any filtering..." << endl;
                print_divider();
                obj.generate_events(elist);
                elist.display_event_list();

                print_header("L2: VERIFIED EVENT STREAM  [Noise Removed]");
                cout << "  Filtering invalid and out-of-range readings..." << endl;
                print_divider();
                obj.verified_event_stream(verified_list);
                verified_list.display_verified_list();

                print_header("L3: ANOMALY EVENT STREAM  [Dangerous Readings]");
                cout << "  Flagging readings that deviate from normal pattern..." << endl;
                print_divider();
                obj.generate_anomaly_stream(verified_list, anomaly_list);
                anomaly_list.display_event_list();

                print_header("L4: FORWARD CORRECTION CHAIN  [Propagated Fix]");
                cout << "  Applying correction forward from last verified event..." << endl;
                print_divider();
                obj.forward_correction_chain(verified_list, forward_correcction_list);
                forward_correcction_list.display_forward_correction_chain();
            }
            else if (sub == 2)
            {
                print_header("DOUBLY LINKED LIST  [Forward Traversal --->]");
                cout << "  Reading events oldest to newest..." << endl;
                print_divider();
                if (!dlist_loaded)
                {
                    obj.generate_events_doubly(dlist);
                    dlist_loaded = true;
                }
                dlist.display_forward();
            }
            else if (sub == 3)
            {
                print_header("DOUBLY LINKED LIST  [Backward Traversal <---]");
                cout << "  Reading events newest to oldest..." << endl;
                print_divider();
                if (!dlist_loaded)
                {
                    obj.generate_events_doubly(dlist);
                    dlist_loaded = true;
                }
                dlist.display_backward();
            }
            else if (sub == 4)
            {
                int cycles = get_valid_cycles("  Enter number of monitoring cycles: ");

                print_header("L7: LOCAL MONITORING LOOP  [Single Zone Repeat]");
                cout << "  Continuously monitoring Zone(0,1) for " << cycles << " cycles..." << endl;
                print_divider();
                if (!clist_loaded)
                {
                    obj.generate_events_circular(clist);
                    clist_loaded = true;
                }
                clist.monitor_single_zone(0, 1, cycles);

                print_header("L8: SYSTEM-WIDE MONITORING LOOP  [All Zones]");
                cout << "  Scanning all forest zones for " << cycles << " cycles..." << endl;
                print_divider();
                clist.monitor_all_zones(cycles);

                print_header("L9: EMERGENCY MONITORING LOOP  [Danger Zones Only]");
                cout << "  Fast scan mode - dangerous condition detected..." << endl;
                print_divider();
                clist.emergency_monitoring_loop(cycles);

                print_header("L10: STABILITY MONITORING LOOP  [Long-term Check]");
                cout << "  Monitoring long-term forest stability..." << endl;
                print_divider();
                clist.stability_monitoring_loop(cycles);
            }
            else if (sub == 5)
            {
                print_header("L5: BACKWARD CORRECTION CHAIN  [Fix Past Events]");
                cout << "  Tracing back through doubly linked list to correct errors..." << endl;
                print_divider();
                obj.verified_event_stream(verified_list);
                obj.convert_verify_singly_to_doubly(verified_list, doubly_verified_list);
                obj.backward_correction_chain(doubly_verified_list);

                print_header("L6: STATE SYNCHRONIZATION  [Global Consistency]");
                cout << "  All modules updated to same consistent forest state..." << endl;
                print_divider();
                if (!global_loaded)
                {
                    obj.state_synchronization(doubly_verified_list, global_list, global_loop);
                    global_loaded = true;
                }
                global_list.display_event_list_global();
                global_loop.display_circular_global();
            }
            else
            {
                print_error("Invalid sub-choice.");
            }
        }

        // MENU 4
        else if (main_choice == 4)
        {
            try {
                int sub;
                cout << "\n  --- [Menu 4] Fire Detection and Control ---" << endl;
                cout << "   1 = Detect Fire Risk (Threshold Check)" << endl;
                cout << "   2 = Trigger Emergency Alert (Q3 Queue)" << endl;
                cout << "   3 = Priority-Based Fire Response" << endl;
                cout << "   4 = Simulate Fire Spread (Graph BFS)" << endl;
                cout << "   5 = Allocate Firefighting Resources (Safe Path)" << endl;
                cout << "  Enter sub-choice: ";
                sub = get_valid_int("", 1, 5);

            if (sub == 1)
            {
                print_header("FIRE RISK DETECTION  [Threshold: Temp>45 | Smoke>70 | Hum<20]");
                obj.check_anomilies();
            }
            else if (sub == 2)
            {
                print_header("EMERGENCY ALERT  [Q3: Emergency Queue]");
                cout << "  Building global state, pushing danger events to Q3..." << endl;
                print_divider();
                if (!global_loaded)
                {
                    obj.verified_event_stream(verified_list);
                    obj.convert_verify_singly_to_doubly(verified_list, doubly_verified_list);
                    obj.state_synchronization(doubly_verified_list, global_list, global_loop);
                    global_loaded = true;
                }
                q1_routing.build_queue(global_list, q1_routing, q2_surviellance, q3_emergency, q4_decision);
                cout << "  Q3 Emergency Events:" << endl;
                q3_emergency.display_queue();
            }
            else if (sub == 3)
            {
                print_header("PRIORITY-BASED FIRE RESPONSE  [Emergency > Surveillance > Routine]");
                q1_routing.priority_switching(q1_routing, q2_surviellance, q3_emergency);
            }
            else if (sub == 4)
            {
                print_header("FIRE SPREAD SIMULATION  [Graph BFS - Level by Level]");
                cout << "  BFS shows how fire expands wave by wave from origin zone" << endl;
                int start = get_valid_int("  Enter fire origin zone (0-8): ", 0, 8);
                print_divider();
                gobj.bfs_fire_spread(start);
            }
            else if (sub == 5)
            {
                print_header("RESOURCE ALLOCATION PATH  [Safe Path - Avoid Fire Zones]");
                cout << "  Finding lowest cost route, skipping blocked zones..." << endl;
                int src = get_valid_int("  Enter resource source zone (0-8): ", 0, 8);
                int dest = get_valid_int("  Enter target fire zone    (0-8): ", 0, 8);
                print_divider();
                gobj.compute_safe_path(src, dest);
            }
            else
            {
                cout << "  Invalid sub-choice." << endl;
            }
            } catch (const exception& e) {
                cout << "  Error in Menu 4: " << e.what() << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        // MENU 5
        else if (main_choice == 5)
        {
            try {
                int sub;
                cout << "\n  --- [Menu 5] Task Scheduling System (Queues) ---" << endl;
                cout << "   1 = Add Routine Task (Q1)" << endl;
                cout << "   2 = Add Surveillance Task (Q2)" << endl;
                cout << "   3 = Add Emergency Task (Q3 - Priority)" << endl;
                cout << "   4 = Process Tasks (FIFO / Priority Switching)" << endl;
                cout << "   5 = Pause and Resume Tasks" << endl;
                cout << "   6 = Build Queues from Global State" << endl;
                cout << "   7 = Display All Queues" << endl;
                cout << "  Enter sub-choice: ";
                sub = get_valid_int("", 1, 7);

            if (sub == 1)
            {
                // 5.1 - Manually add a routine task to Q1
                if (queue_paused)
                {
                    cout << "  [PAUSED] Task scheduling is paused. Resume first (option 5)." << endl;
                }
                else
                {
                    int t = get_valid_int("  Enter Temperature (0-100): ", 0, 100);
                    int h = get_valid_int("  Enter Humidity (0-100): ", 0, 100);
                    int s = get_valid_int("  Enter Smoke Level (0-100): ", 0, 100);
                    q1_routing.enqueue(new event_node(t, h, s, 0, 0, 0));
                    print_header("ROUTINE TASK ADDED  [Q1 - Normal Monitoring]");
                    cout << "  Task enqueued in Q1. Q1 size: " << q1_routing.getsize() << endl;
                }
            }
            else if (sub == 2)
            {
                // 5.2 - Manually add a surveillance task to Q2
                if (queue_paused)
                {
                    cout << "  [PAUSED] Task scheduling is paused. Resume first (option 5)." << endl;
                }
                else
                {
                    int t = get_valid_int("  Enter Temperature (0-100): ", 0, 100);
                    int h = get_valid_int("  Enter Humidity (0-100): ", 0, 100);
                    int s = get_valid_int("  Enter Smoke Level (0-100): ", 0, 100);
                    q2_surviellance.enqueue(new event_node(t, h, s, 0, 0, 0));
                    print_header("SURVEILLANCE TASK ADDED  [Q2 - Frequent Zone Updates]");
                    cout << "  Task enqueued in Q2. Q2 size: " << q2_surviellance.getsize() << endl;
                }
            }
            else if (sub == 3)
            {
                // 5.3 - Manually add an emergency task to Q3 (highest priority)
                if (queue_paused)
                {
                    cout << "  [PAUSED] Task scheduling is paused. Resume first (option 5)." << endl;
                }
                else
                {
                    int t = get_valid_int("  Enter Temperature (0-100): ", 0, 100);
                    int h = get_valid_int("  Enter Humidity (0-100): ", 0, 100);
                    int s = get_valid_int("  Enter Smoke Level (0-100): ", 0, 100);
                    q3_emergency.enqueue(new event_node(t, h, s, 0, 0, 0));
                    print_header("EMERGENCY TASK ADDED  [Q3 - Urgent Response Queue]");
                    cout << "  Task enqueued in Q3. Q3 size: " << q3_emergency.getsize() << endl;
                    cout << "  Emergency tasks will be processed FIRST in priority switching." << endl;
                }
            }
            else if (sub == 4)
            {
                // 5.4 - Process all tasks using priority order: Q3 -> Q2 -> Q1
                if (queue_paused)
                {
                    cout << "  [PAUSED] Task scheduling is paused. Resume first (option 5)." << endl;
                }
                else
                {
                    print_header("PROCESS TASKS  [Priority: Emergency > Surveillance > Routine]");
                    cout << "  Tasks before processing:" << endl;
                    cout << "   Q1 Routine     : " << q1_routing.getsize() << " tasks" << endl;
                    cout << "   Q2 Surveillance: " << q2_surviellance.getsize() << " tasks" << endl;
                    cout << "   Q3 Emergency   : " << q3_emergency.getsize() << " tasks" << endl;
                    print_divider();
                    q1_routing.priority_switching(q1_routing, q2_surviellance, q3_emergency);
                    print_divider();
                    cout << "  All tasks processed." << endl;
                }
            }
            else if (sub == 5)
            {
                // 5.5 - Pause or Resume the task scheduling system
                if (!queue_paused)
                {
                    queue_paused = true;
                    print_header("TASK SCHEDULING PAUSED");
                    cout << "  System paused. No new tasks will be added or processed." << endl;
                    cout << "  Current queue status (frozen):" << endl;
                    cout << "   Q1 Routine     : " << q1_routing.getsize() << " tasks (waiting)" << endl;
                    cout << "   Q2 Surveillance: " << q2_surviellance.getsize() << " tasks (waiting)" << endl;
                    cout << "   Q3 Emergency   : " << q3_emergency.getsize() << " tasks (waiting)" << endl;
                    cout << "   Q4 Decision    : " << q4_decision.getsize() << " tasks (waiting)" << endl;
                    cout << "\n  Run option 5 again to RESUME." << endl;
                }
                else
                {
                    queue_paused = false;
                    print_header("TASK SCHEDULING RESUMED");
                    cout << "  System resumed. Tasks can be added and processed normally." << endl;
                    cout << "  Pending tasks in queue:" << endl;
                    cout << "   Q1 Routine     : " << q1_routing.getsize() << " tasks" << endl;
                    cout << "   Q2 Surveillance: " << q2_surviellance.getsize() << " tasks" << endl;
                    cout << "   Q3 Emergency   : " << q3_emergency.getsize() << " tasks" << endl;
                    cout << "   Q4 Decision    : " << q4_decision.getsize() << " tasks" << endl;
                }
            }
            else if (sub == 6)
            {
                // 6 - Build all queues automatically from global synchronized state
                if (!global_loaded)
                {
                    obj.verified_event_stream(verified_list);
                    obj.convert_verify_singly_to_doubly(verified_list, doubly_verified_list);
                    obj.state_synchronization(doubly_verified_list, global_list, global_loop);
                    global_loaded = true;
                }
                print_header("BUILDING ALL QUEUES  [From Global Synchronized State]");
                q1_routing.build_queue(global_list, q1_routing, q2_surviellance, q3_emergency, q4_decision);
                cout << "   Q1 Routine     : " << q1_routing.getsize() << " events" << endl;
                cout << "   Q2 Surveillance: " << q2_surviellance.getsize() << " events" << endl;
                cout << "   Q3 Emergency   : " << q3_emergency.getsize() << " events" << endl;
                cout << "   Q4 Decision    : " << q4_decision.getsize() << " events" << endl;
            }
            else if (sub == 7)
            {
                // 7 - Display all queues
                print_header("Q1: ROUTINE MONITORING QUEUE");
                if (q1_routing.isempty())
                    cout << "  Q1 empty." << endl;
                else
                    q1_routing.display_queue();

                print_header("Q2: SURVEILLANCE QUEUE");
                if (q2_surviellance.isempty())
                    cout << "  Q2 empty." << endl;
                else
                    q2_surviellance.display_queue();

                print_header("Q3: EMERGENCY QUEUE");
                if (q3_emergency.isempty())
                    cout << "  Q3 empty." << endl;
                else
                    q3_emergency.display_queue();

                print_header("Q4: MULTI-FACTOR DECISION QUEUE");
                if (q4_decision.isempty())
                    cout << "  Q4 empty." << endl;
                else
                    q4_decision.display_queue();
            }
            else
            {
                cout << "  Invalid sub-choice." << endl;
            }
            } catch (const exception& e) {
                cout << "  Error in Menu 5: " << e.what() << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        // MENU 6
        else if (main_choice == 6)
        {
            cout << "\n  --- [Menu 6] Decision System (Trees) ---" << endl;
            cout << "   1  = Build All 12 Trees" << endl;
            cout << "   2  = T1: Zone Hierarchy" << endl;
            cout << "   3  = T2: Sub-Zone Decomposition" << endl;
            cout << "   4  = T3: Terrain Classification" << endl;
            cout << "   5  = T4: Water Resource" << endl;
            cout << "   6  = T5: Fire Control Resource" << endl;
            cout << "   7  = T6: Equipment Allocation" << endl;
            cout << "   8  = T7: Fire Classification" << endl;
            cout << "   9  = T8: Wildlife Activity" << endl;
            cout << "   10 = T9: Human Activity" << endl;
            cout << "   11 = Compute Risk Score (manual input)" << endl;
            cout << "   12 = T10: Zone-Level Decision" << endl;
            cout << "   13 = T11: Regional Escalation Decision" << endl;
            cout << "   14 = T12: Global Emergency Decision" << endl;
            cout << "   15 = Execute Final Action (T10+T11+T12)" << endl;
            int sub = get_valid_int("  Enter sub-choice: ", 1, 15);

            if (sub == 1)
            {
                print_header("BUILDING ALL 12 TREES");
                cout << "  Structural : T1 Zone Hierarchy | T2 Sub-Zone | T3 Terrain" << endl;
                cout << "  Resource   : T4 Water | T5 Fire Control | T6 Equipment" << endl;
                cout << "  Incident   : T7 Fire Class | T8 Wildlife | T9 Human Activity" << endl;
                cout << "  Decision   : T10 Local | T11 Regional | T12 Global" << endl;
                print_divider();
                tobj.build_T1_zone_hierarchy();
                tobj.build_T2_subzone_decomposition();
                tobj.build_T3_terrain_classification();
                tobj.build_T4_water_resource();
                tobj.build_T5_fire_control();
                tobj.build_T6_equipment_allocation();
                tobj.build_T7_fire_classification();
                tobj.build_T8_wildlife_activity();
                tobj.build_T9_human_activity();
                tobj.build_T10_local_decision();
                tobj.build_T11_regional_escalation();
                tobj.build_T12_global_emergency();
                cout << "  All 12 trees built successfully." << endl;
            }
            else if (sub == 2)
            {
                print_header("T1: ZONE HIERARCHY TREE");
                tobj.show_T1();
            }
            else if (sub == 3)
            {
                print_header("T2: SUB-ZONE DECOMPOSITION TREE");
                tobj.show_T2();
            }
            else if (sub == 4)
            {
                print_header("T3: TERRAIN CLASSIFICATION TREE");
                tobj.show_T3();
            }
            else if (sub == 5)
            {
                print_header("T4: WATER RESOURCE TREE");
                tobj.show_T4();
            }
            else if (sub == 6)
            {
                print_header("T5: FIRE CONTROL RESOURCE TREE");
                tobj.show_T5();
            }
            else if (sub == 7)
            {
                print_header("T6: EQUIPMENT ALLOCATION TREE");
                tobj.show_T6();
            }
            else if (sub == 8)
            {
                print_header("T7: FIRE CLASSIFICATION TREE");
                tobj.show_T7();
            }
            else if (sub == 9)
            {
                print_header("T8: WILDLIFE ACTIVITY TREE");
                tobj.show_T8();
            }
            else if (sub == 10)
            {
                print_header("T9: HUMAN ACTIVITY TREE");
                tobj.show_T9();
            }
            else if (sub == 11)
            {
                print_header("RISK SCORE CALCULATOR  [0.4*Temp + 0.3*Smoke + 0.3*(1-Hum)]");
                int t, h, s;
                cout << "  Enter Temperature : ";
                cin >> t;
                cout << "  Enter Humidity    : ";
                cin >> h;
                cout << "  Enter Smoke Level : ";
                cin >> s;
                print_divider();
                tobj.compute_risk_score_manual(t, h, s);
            }
            else if (sub == 12)
            {
                print_header("T10: ZONE-LEVEL DECISION  [If RiskScore > 0.6 => Local Response]");
                tobj.show_T10();
                tobj.zone_level_decision();
            }
            else if (sub == 13)
            {
                print_header("T11: REGIONAL ESCALATION  [If SpreadRate > 0.5 => Escalate]");
                tobj.show_T11();
                tobj.regional_decision();
            }
            else if (sub == 14)
            {
                print_header("T12: GLOBAL EMERGENCY  [If TotalRisk > 0.7 => Global Alert]");
                tobj.show_T12();
                tobj.global_emergency_decision();
            }
            else if (sub == 15)
            {
                print_header("FINAL ACTION  [Combined Score from T10 + T11 + T12]");
                tobj.execute_final_action();
            }
            else
            {
                cout << "  Invalid sub-choice." << endl;
            }
        }

        // MENU 7
        else if (main_choice == 7)
        {
            try {
                int sub;
                cout << "\n  --- [Menu 7] Spatial Routing System (Graphs) ---" << endl;
                cout << "   1 = Display Zone Status (from Array Layer)" << endl;
                cout << "   2 = Display G1: Adjacency List" << endl;
                cout << "   3 = Display G2: Adjacency Matrix" << endl;
                cout << "   4 = BFS Traversal (Fire Spread Simulation)" << endl;
                cout << "   5 = DFS Traversal (Deep Path Analysis)" << endl;
                cout << "   6 = Compute Safe Rescue Path" << endl;
                cout << "   7 = Update Blocked Routes (Fire Level Change)" << endl;
                cout << "  Enter sub-choice: ";
                sub = get_valid_int("", 1, 7);

            if (sub == 1)
            {
                print_header("ZONE STATUS  [Integrated from Array Layer Data]");
                gobj.display_zone_status();
            }
            else if (sub == 2)
            {
                print_header("G1: ADJACENCY LIST  [Each Zone -> Neighbors + Path Cost]");
                cout << "  Path Cost = Distance + Danger (fire_level * 10)" << endl;
                print_divider();
                gobj.display_adj_list();
            }
            else if (sub == 3)
            {
                print_header("G2: ADJACENCY MATRIX  [INF = No Direct Connection]");
                gobj.display_adj_matrix();
            }
            else if (sub == 4)
            {
                print_header("BFS: FIRE SPREAD SIMULATION  [Level by Level Expansion]");
                cout << "  Fire expands to all neighbors before going deeper" << endl;
                int start = get_valid_int("  Enter fire origin zone (0-8): ", 0, 8);
                print_divider();
                gobj.bfs_fire_spread(start);
            }
            else if (sub == 5)
            {
                print_header("DFS: DEEP PATH ANALYSIS  [One Corridor at a Time]");
                cout << "  Traces one fire path deeply before backtracking" << endl;
                int start = get_valid_int("  Enter start zone (0-8): ", 0, 8);
                print_divider();
                gobj.dfs_deep_analysis(start);
            }
            else if (sub == 6)
            {
                print_header("SAFE RESCUE PATH  [Shortest Path Avoiding Blocked Zones]");
                int src = get_valid_int("  Enter source zone      (0-8): ", 0, 8);
                int dest = get_valid_int("  Enter destination zone (0-8): ", 0, 8);
                print_divider();
                gobj.compute_safe_path(src, dest);
            }
            else if (sub == 7)
            {
                print_header("UPDATE BLOCKED ROUTES  [Cost = Distance * (1 + fire_level)]");
                int zone = get_valid_int("  Enter zone with fire increase (0-8): ", 0, 8);
                float flevel = get_valid_float("  Enter new fire level (0.0 - 1.0): ");
                print_divider();
                gobj.update_blocked_routes(zone, flevel);
            }
            else
            {
                cout << "  Invalid sub-choice." << endl;
            }
            } catch (const exception& e) {
                cout << "  Error in Menu 7: " << e.what() << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        // MENU 8
        else if (main_choice == 8)
        {
            try {
                int sub;
                cout << "\n  --- [Menu 8] Hash-Based Fast Access System ---" << endl;
                cout << "   1 = Insert Zone Data (Hash Table)" << endl;
                cout << "   2 = Retrieve Zone Data (O(1) Access)" << endl;
                cout << "   3 = View H2: Collision Chain Table" << endl;
                cout << "   4 = Update Cache (H3)" << endl;
                cout << "   5 = View H1: Index Table" << endl;
                cout << "   6 = View All (H1 + H2 + H3)" << endl;
                cout << "   7 = Update Zone Data" << endl;
                cout << "  Enter sub-choice: ";
                sub = get_valid_int("", 1, 7);

            if (sub == 1)
            {
                print_header("H1: INSERT  [Index = ZoneID % 10]");
                int zid = get_valid_int("  Enter Zone ID (0-9): ", 0, 9);
                int t = get_valid_int("  Enter Temperature (0-100): ", 0, 100);
                int h = get_valid_int("  Enter Humidity (0-100): ", 0, 100);
                int s = get_valid_int("  Enter Smoke Level (0-100): ", 0, 100);
                print_divider();
                hobj.insert(zid, t, h, s);
            }
            else if (sub == 2)
            {
                print_header("RETRIEVE  [H3 Cache -> H1 Table -> H2 Chain]");
                int zid = get_valid_int("  Enter Zone ID (0-9): ", 0, 9);
                print_divider();
                hobj.retrieve(zid);
            }
            else if (sub == 3)
            {
                print_header("H2: COLLISION HANDLING  [Chaining - Overflow from H1]");
                cout << "  Zones with same hash index stored as linked chain" << endl;
                print_divider();
                hobj.display_collision_table();
            }
            else if (sub == 4)
            {
                print_header("H3: UPDATE CACHE  [Fast Retrieval Buffer - Last 5 zones]");
                int zid = get_valid_int("  Enter Zone ID (0-9): ", 0, 9);
                int t = get_valid_int("  Enter Temperature (0-100): ", 0, 100);
                int h = get_valid_int("  Enter Humidity (0-100): ", 0, 100);
                int s = get_valid_int("  Enter Smoke Level (0-100): ", 0, 100);
                print_divider();
                hobj.update_cache(zid, t, h, s);
            }
            else if (sub == 5)
            {
                print_header("H1: PRIMARY INDEX TABLE  [Direct Slot Storage]");
                hobj.display_H1();
            }
            else if (sub == 6)
            {
                print_header("FULL HASH SYSTEM  [H1 Primary + H2 Collision + H3 Cache]");
                hobj.display_all();
            }
            else if (sub == 7)
            {
                print_header("UPDATE ZONE  [Search H1 then H2 Chain]");
                int zid = get_valid_int("  Enter Zone ID to update (0-9): ", 0, 9);
                int t = get_valid_int("  Enter new Temperature (0-100): ", 0, 100);
                int h = get_valid_int("  Enter new Humidity (0-100): ", 0, 100);
                int s = get_valid_int("  Enter new Smoke Level (0-100): ", 0, 100);
                print_divider();
                hobj.update(zid, t, h, s);
            }
            else
            {
                cout << "  Invalid sub-choice." << endl;
            }
            } catch (const exception& e) {
                cout << "  Error in Menu 8: " << e.what() << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        // MENU 9
        else if (main_choice == 9)
        {
            cout << "\n  --- [Menu 9] System Monitoring ---" << endl;
            cout << "   1 = Monitor System Load" << endl;
            cout << "   2 = Track Execution Time (start/stop timer)" << endl;
            cout << "   3 = Detect Bottlenecks" << endl;
            cout << "   4 = Optimize Performance" << endl;
            cout << "   5 = View System Health Dashboard" << endl;
            int sub = get_valid_int("  Enter sub-choice: ", 1, 5);

            // update stats before showing anything
            // feed current task counts from all live modules
            mobj.update_module_stats(
                obj.getsize(),                                                                                     // array tasks
                0,                                                                                                 // linked list (no size func, use 0)
                q1_routing.getsize() + q2_surviellance.getsize() + q3_emergency.getsize() + q4_decision.getsize(), // queue tasks
                12,                                                                                                // tree tasks (12 trees)
                9,                                                                                                 // graph tasks (9 zones)
                9                                                                                                  // hash tasks (9 entries)
            );

            if (sub == 1)
            {
                print_header("SYSTEM LOAD MONITOR  [Load = Active Tasks / Capacity]");
                mobj.monitor_system_load();
            }
            else if (sub == 2)
            {
                print_header("EXECUTION TIME TRACKING  [Latency = Finish - Start]");
                cout << "  Starting timer..." << endl;
                mobj.start_timer();

                // time a small operation to demo
                cout << "  Running anomaly check (timed operation)..." << endl;
                obj.check_anomilies();

                mobj.stop_timer("Anomaly Check");
                mobj.track_execution_time();
            }
            else if (sub == 3)
            {
                print_header("BOTTLENECK DETECTION  [Slowest Module = Bottleneck]");
                mobj.detect_bottlenecks();
            }
            else if (sub == 4)
            {
                print_header("PERFORMANCE OPTIMIZATION  [Module-Specific Suggestions]");
                mobj.optimize_performance();
            }
            else if (sub == 5)
            {
                print_header("SYSTEM HEALTH DASHBOARD  [Full Status Report]");
                mobj.view_system_health();
            }
            else
            {
                cout << "  Invalid sub-choice." << endl;
            }
        }

        // MENU 10
        else if (main_choice == 10)
        {
            cout << "\n  --- [Menu 10] Scenario Simulation ---" << endl;
            cout << "   1 = Cascading Fire Scenario" << endl;
            cout << "   2 = Sensor Failure Scenario" << endl;
            cout << "   3 = Multi-Factor Anomaly Scenario" << endl;
            cout << "   4 = System Overload Scenario" << endl;
            cout << "   5 = Global Emergency Scenario" << endl;
            cout << "   6 = Run Full System Simulation" << endl;
            int sub = get_valid_int("  Enter sub-choice: ", 1, 6);

            if (sub == 1)
            {
                print_header("SCENARIO 1: CASCADING FIRE & RESOURCE CONFLICT");
                cout << "  Fire starts in Zone 3, spreads toward Zone 4 and Zone 6." << endl;
                cout << "  System detects, verifies, and coordinates emergency response." << endl;

                print_step("Step 1: Array Layer - Zone sensor data");
                obj.display_sensor_data();

                print_step("Step 2: Anomaly Detection - Fire zone identification");
                obj.check_anomilies();
                obj.check_boundary();

                print_step("Step 3: Linked List - Event verification and storage");
                obj.verified_event_stream(verified_list);
                verified_list.display_verified_list();

                print_step("Step 4: Global State Synchronization");
                obj.convert_verify_singly_to_doubly(verified_list, doubly_verified_list);
                if (!global_loaded)
                {
                    obj.state_synchronization(doubly_verified_list, global_list, global_loop);
                    global_loaded = true;
                }
                global_list.display_event_list_global();

                print_step("Step 5: Queue Layer - Emergency events routed to Q3");
                q1_routing.build_queue(global_list, q1_routing, q2_surviellance, q3_emergency, q4_decision);
                cout << "  Q3 Emergency Events:" << endl;
                q3_emergency.display_queue();

                print_step("Step 6: Priority Switching - Emergency processed first");
                q1_routing.priority_switching(q1_routing, q2_surviellance, q3_emergency);
            }
            else if (sub == 2)
            {
                print_header("SCENARIO 2: SENSOR FAILURE & SYSTEM RECONSTRUCTION");
                cout << "  Zone 2 sensors send incomplete readings." << endl;
                cout << "  System uses backward correction to rebuild missing data." << endl;

                print_step("Step 1: Array Layer - Current sensor readings");
                obj.display_sensor_data();

                print_step("Step 2: Linked List - Filtering bad sensor data");
                obj.verified_event_stream(verified_list);
                verified_list.display_verified_list();

                print_step("Step 3: Backward Correction - Tracing and fixing past errors");
                obj.convert_verify_singly_to_doubly(verified_list, doubly_verified_list);
                obj.backward_correction_chain(doubly_verified_list);

                print_step("Step 4: Global State - Restored and synchronized");
                if (!global_loaded)
                {
                    obj.state_synchronization(doubly_verified_list, global_list, global_loop);
                    global_loaded = true;
                }
                global_list.display_event_list_global();
            }
            else if (sub == 3)
            {
                print_header("SCENARIO 3: MULTI-FACTOR ANOMALY ESCALATION");
                cout << "  Simultaneous: animal movement + fire risk + human intrusion." << endl;
                cout << "  System combines all signals into one escalated alert." << endl;

                print_step("Step 1: Anomaly Stream - Detecting unusual events");
                obj.verified_event_stream(verified_list);
                obj.generate_anomaly_stream(verified_list, anomaly_list);
                anomaly_list.display_event_list();

                print_step("Step 2: Circular Loop L9 - Emergency scan mode activated");
                if (!clist_loaded)
                {
                    obj.generate_events_circular(clist);
                    clist_loaded = true;
                }
                clist.emergency_monitoring_loop(3);

                print_step("Step 3: Queue - Combined events pushed to Q4 Decision Queue");
                obj.convert_verify_singly_to_doubly(verified_list, doubly_verified_list);
                if (!global_loaded)
                {
                    obj.state_synchronization(doubly_verified_list, global_list, global_loop);
                    global_loaded = true;
                }
                q1_routing.build_queue(global_list, q1_routing, q2_surviellance, q3_emergency, q4_decision);
                cout << "  Q4 Decision Queue:" << endl;
                q4_decision.display_queue();
            }
            else if (sub == 4)
            {
                print_header("SCENARIO 4: SYSTEM OVERLOAD & LOAD REDISTRIBUTION");
                cout << "  Too many simultaneous updates slow down processing." << endl;
                cout << "  System separates critical from routine and rebalances." << endl;

                print_step("Step 1: Building queues from global state");
                if (!global_loaded)
                {
                    obj.verified_event_stream(verified_list);
                    obj.convert_verify_singly_to_doubly(verified_list, doubly_verified_list);
                    obj.state_synchronization(doubly_verified_list, global_list, global_loop);
                    global_loaded = true;
                }
                q1_routing.build_queue(global_list, q1_routing, q2_surviellance, q3_emergency, q4_decision);

                print_step("Step 2: Queue Load Analysis");
                cout << "   Q1 Routine     : " << q1_routing.getsize() << " events" << endl;
                cout << "   Q2 Surveillance: " << q2_surviellance.getsize() << " events" << endl;
                cout << "   Q3 Emergency   : " << q3_emergency.getsize() << " events  <-- HIGH PRIORITY" << endl;

                print_step("Step 3: Load Balancing - Redistributing overflow");
                q1_routing.load_balancing(q1_routing, q2_surviellance);

                print_step("Step 4: Priority Switching - Critical tasks first");
                q1_routing.priority_switching(q1_routing, q2_surviellance, q3_emergency);
            }
            else if (sub == 5)
            {
                print_header("SCENARIO 5: GLOBAL MULTI-ZONE EMERGENCY SYNCHRONIZATION");
                cout << "  Large-scale emergency across multiple zones simultaneously." << endl;
                cout << "  System achieves consistent global state and coordinated response." << endl;

                print_step("Step 1: Array Layer - Full zone scan");
                obj.display_sensor_data();
                obj.check_anomilies();

                print_step("Step 2: Linked Lists - Collection, correction, synchronization");
                obj.verified_event_stream(verified_list);
                obj.generate_anomaly_stream(verified_list, anomaly_list);
                obj.forward_correction_chain(verified_list, forward_correcction_list);
                obj.convert_verify_singly_to_doubly(verified_list, doubly_verified_list);
                obj.backward_correction_chain(doubly_verified_list);

                print_step("Step 3: Global Synchronization - Unified state achieved");
                if (!global_loaded)
                {
                    obj.state_synchronization(doubly_verified_list, global_list, global_loop);
                    global_loaded = true;
                }
                global_list.display_event_list_global();
                global_loop.display_circular_global();

                print_step("Step 4: Emergency Queue - Coordinated response");
                q1_routing.build_queue(global_list, q1_routing, q2_surviellance, q3_emergency, q4_decision);
                q3_emergency.display_queue();
                q1_routing.priority_switching(q1_routing, q2_surviellance, q3_emergency);

                print_divider();
                cout << "\n  !! GLOBAL EMERGENCY SYNCHRONIZED ACROSS ALL ZONES !!" << endl;
                print_divider();
            }
            else if (sub == 6)
            {
                print_header("FULL SYSTEM SIMULATION  [All Layers End-to-End]");

                print_step("[LAYER 1] ARRAY LAYER");
                obj.display_sensor_data();
                obj.display_static_grid();
                obj.auto_fill_dynamic_grid();
                obj.display_dynamic_grid();
                obj.check_anomilies();
                obj.check_boundary();

                print_step("[LAYER 2] LINKED LIST LAYER");
                obj.verified_event_stream(verified_list);
                verified_list.display_verified_list();
                obj.generate_anomaly_stream(verified_list, anomaly_list);
                anomaly_list.display_event_list();
                obj.forward_correction_chain(verified_list, forward_correcction_list);
                forward_correcction_list.display_forward_correction_chain();
                obj.convert_verify_singly_to_doubly(verified_list, doubly_verified_list);
                obj.backward_correction_chain(doubly_verified_list);
                if (!global_loaded)
                {
                    obj.state_synchronization(doubly_verified_list, global_list, global_loop);
                    global_loaded = true;
                }
                global_list.display_event_list_global();
                global_loop.display_circular_global();
                if (!clist_loaded)
                {
                    obj.generate_events_circular(clist);
                    clist_loaded = true;
                }
                clist.monitor_single_zone(0, 1, 3);
                clist.monitor_all_zones(3);
                clist.emergency_monitoring_loop(3);
                clist.stability_monitoring_loop(3);

                print_step("[LAYER 3] QUEUE LAYER");
                q1_routing.build_queue(global_list, q1_routing, q2_surviellance, q3_emergency, q4_decision);
                cout << "  Q1 Routine     : " << endl;
                q1_routing.display_queue();
                cout << "  Q2 Surveillance: " << endl;
                q2_surviellance.display_queue();
                cout << "  Q3 Emergency   : " << endl;
                q3_emergency.display_queue();
                cout << "  Q4 Decision    : " << endl;
                q4_decision.display_queue();
                q1_routing.priority_switching(q1_routing, q2_surviellance, q3_emergency);
                q1_routing.load_balancing(q1_routing, q2_surviellance);

                print_step("[LAYER 4] TREE LAYER");
                tobj.build_T1_zone_hierarchy();
                tobj.build_T7_fire_classification();
                tobj.build_T10_local_decision();
                tobj.build_T11_regional_escalation();
                tobj.build_T12_global_emergency();
                tobj.show_T1();
                tobj.show_T7();
                tobj.zone_level_decision();
                tobj.regional_decision();
                tobj.global_emergency_decision();
                tobj.execute_final_action();

                print_step("[LAYER 5] GRAPH LAYER");
                gobj.display_zone_status();
                gobj.display_adj_list();
                gobj.bfs_fire_spread(3);
                gobj.compute_safe_path(0, 8);

                print_step("[LAYER 6] HASH LAYER");
                hobj.display_all();

                print_step("[LAYER 7] SYSTEM MONITORING");
                mobj.update_module_stats(
                    obj.getsize(), 0,
                    q1_routing.getsize() + q2_surviellance.getsize() + q3_emergency.getsize() + q4_decision.getsize(),
                    12, 9, 9);
                mobj.view_system_health();
                mobj.detect_bottlenecks();
            }
            else
            {
                cout << "  Invalid sub-choice." << endl;
            }
        }

        // MENU 11: EXECUTION CONTROL STACK
        else if (main_choice == 11)
        {
            try {
                int sub;
                cout << "\n  --- [Menu 11] Execution Control Stack (Dept 3) ---" << endl;
                cout << "   1 = Push / Save Current Zone State" << endl;
                cout << "   2 = Pop State (Discard Top)" << endl;
                cout << "   3 = Peek Top State" << endl;
                cout << "   4 = Rollback to Last Saved State" << endl;
                cout << "   5 = Display All Saved States" << endl;
                cout << "   6 = Clear Stack" << endl;
                cout << "  Enter sub-choice: ";
                sub = get_valid_int("", 1, 6);

            if (sub == 1)
            {
                // Save a zone state onto the stack
                int zone = get_valid_int("  Enter Zone ID (0-8): ", 0, 8);
                float temp = get_valid_float("  Enter Temperature: ");
                float smoke = get_valid_float("  Enter Smoke Level: ");
                float humidity = get_valid_float("  Enter Humidity: ");
                string label;
                cout << "  Enter State Label (e.g. PRE_FIRE, STABLE): ";
                cin >> label;
                sobj.push(zone, temp, smoke, humidity, label);
                cout << "  [STACK] State pushed successfully." << endl;
                cout << "  Stack size: " << sobj.get_size() << endl;
            }
            else if (sub == 2)
            {
                // Remove top state
                sobj.pop();
                cout << "  Stack size after pop: " << sobj.get_size() << endl;
            }
            else if (sub == 3)
            {
                // Peek at top without removing
                int zone;
                float temp, smoke, humidity;
                string label;
                if (sobj.peek(zone, temp, smoke, humidity, label))
                {
                    cout << "\n  [STACK PEEK] Top State:" << endl;
                    cout << "  Zone     : " << zone << endl;
                    cout << "  Temp     : " << temp << endl;
                    cout << "  Smoke    : " << smoke << endl;
                    cout << "  Humidity : " << humidity << endl;
                    cout << "  Label    : " << label << endl;
                }
                else
                {
                    cout << "  [STACK] Stack is empty." << endl;
                }
            }
            else if (sub == 4)
            {
                // ROLLBACK - restore last saved state
                int zone;
                float temp, smoke, humidity;
                string label;
                if (sobj.rollback(zone, temp, smoke, humidity, label))
                {
                    cout << "\n  [ROLLBACK SUCCESS] Restored State:" << endl;
                    cout << "  Zone     : " << zone << endl;
                    cout << "  Temp     : " << temp << endl;
                    cout << "  Smoke    : " << smoke << endl;
                    cout << "  Humidity : " << humidity << endl;
                    cout << "  Label    : " << label << endl;
                    cout << "  System restored to last stable state." << endl;
                }
            }
            else if (sub == 5)
            {
                // Display all saved states
                sobj.display_stack();
            }
            else if (sub == 6)
            {
                sobj.clear_stack();
                cout << "  [STACK] All saved states cleared." << endl;
            }
            else
            {
                cout << "  Invalid sub-choice." << endl;
            }
            } catch (const exception& e) {
                cout << "  Error in Menu 11: " << e.what() << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        else if (main_choice != 0)
        {
            cout << "  Invalid choice. Try again." << endl;
        }

    } while (main_choice != 0);

    cout << "\n  System exiting. Goodbye." << endl;
    return 0;
}

// Main entry point - called by the OS
int main() {
    return console_main();
}