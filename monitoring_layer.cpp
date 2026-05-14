#include "monitoring_layer.h"

// CONSTRUCTOR
// Initialize all 6 module stats with names and capacities
monitoring_layer::monitoring_layer()
{
    modules[0].module_name         = "Array Layer     (A1-A4)";
    modules[0].processing_capacity = 100;

    modules[1].module_name         = "LinkedList Layer(L1-L10)";
    modules[1].processing_capacity = 50;

    modules[2].module_name         = "Queue Layer     (Q1-Q4)";
    modules[2].processing_capacity = 30;

    modules[3].module_name         = "Tree Layer      (T1-T12)";
    modules[3].processing_capacity = 12;

    modules[4].module_name         = "Graph Layer     (G1-G2)";
    modules[4].processing_capacity = 20;

    modules[5].module_name         = "Hash Layer      (H1-H3)";
    modules[5].processing_capacity = 10;

    start_clock         = 0;
    recorded_latency_ms = 0.0f;
}

// HELPER: Compute health from load%
// Time Complexity: O(1)
string monitoring_layer::compute_health(float load_percent)
{
    if      (load_percent >= 80.0f) return "!! OVERLOADED !!";
    else if (load_percent >= 50.0f) return "WARNING";
    else                            return "Healthy";
}

// HELPER: Find module with highest latency = bottleneck
// Time Complexity: O(n)
int monitoring_layer::find_bottleneck_index()
{
    int   idx     = 0;
    float highest = modules[0].latency_ms;
    for (int i = 1; i < TOTAL_MODULES; i++)
    {
        if (modules[i].latency_ms > highest)
        {
            highest = modules[i].latency_ms;
            idx     = i;
        }
    }
    return idx;
}

// UPDATE MODULE STATS
// Load% = (active_tasks / capacity) * 100
// Latency simulated based on load
// Time Complexity: O(n)
void monitoring_layer::update_module_stats(int array_tasks, int ll_tasks,
                                            int queue_tasks, int tree_tasks,
                                            int graph_tasks, int hash_tasks)
{
    int   counts[TOTAL_MODULES]  = {array_tasks, ll_tasks, queue_tasks,
                                     tree_tasks,  graph_tasks, hash_tasks};
    float base_lat[TOTAL_MODULES]= {0.5f, 1.2f, 0.8f, 2.0f, 1.5f, 0.3f};

    for (int i = 0; i < TOTAL_MODULES; i++)
        modules[i].is_bottleneck = false;

    for (int i = 0; i < TOTAL_MODULES; i++)
    {
        modules[i].active_tasks  = counts[i];
        float load = (float)counts[i] / (float)modules[i].processing_capacity * 100.0f;
        if (load > 100.0f) load  = 100.0f;
        modules[i].load_percent  = load;
        modules[i].latency_ms    = base_lat[i] * (1.0f + (load / 100.0f));
        modules[i].health_status = compute_health(load);
    }

    modules[find_bottleneck_index()].is_bottleneck = true;
}

// START TIMER
// Time Complexity: O(1)
void monitoring_layer::start_timer()
{
    start_clock = clock();
    cout << "  [Timer started]" << endl;
}

// STOP TIMER
// Latency = Finish Time - Start Time
// Time Complexity: O(1)
void monitoring_layer::stop_timer(string operation_name)
{
    clock_t finish      = clock();
    recorded_latency_ms = (float)(finish - start_clock) / CLOCKS_PER_SEC * 1000.0f;
    cout << "  [Timer stopped]" << endl;
    cout << "  Operation : " << operation_name << endl;
    cout << "  Latency   : " << recorded_latency_ms << " ms" << endl;
    if      (recorded_latency_ms < 1.0f)  cout << "  Status    : Excellent (sub-ms)" << endl;
    else if (recorded_latency_ms < 10.0f) cout << "  Status    : Good" << endl;
    else                                  cout << "  Status    : High - check bottleneck" << endl;
}

// MONITOR SYSTEM LOAD
// Time Complexity: O(n)
void monitoring_layer::monitor_system_load()
{
    cout << "\n  Module                    | Tasks | Capacity | Load%  | Status" << endl;
    cout << "  --------------------------|-------|----------|--------|--------" << endl;

    for (int i = 0; i < TOTAL_MODULES; i++)
    {
        cout << "  " << modules[i].module_name
             << " |  "  << modules[i].active_tasks
             << "    |    " << modules[i].processing_capacity
             << "     |  " << modules[i].load_percent
             << "%  | "   << modules[i].health_status;
        if (modules[i].is_bottleneck) cout << "  <-- BOTTLENECK";
        cout << endl;
    }

    cout << "\n  Load Bars:" << endl;
    for (int i = 0; i < TOTAL_MODULES; i++)
    {
        cout << "  " << modules[i].module_name.substr(0, 12) << " [";
        int bars = (int)(modules[i].load_percent / 10);
        for (int b = 0; b < 10; b++)
            cout << (b < bars ? "#" : "-");
        cout << "] " << modules[i].load_percent << "%" << endl;
    }
}

// TRACK EXECUTION TIME
// Time Complexity: O(n)
void monitoring_layer::track_execution_time()
{
    cout << "\n  Last Recorded Latency: " << recorded_latency_ms << " ms" << endl;
    cout << "\n  Per-Module Estimated Latency:" << endl;
    cout << "  Module                    | Latency (ms)" << endl;
    cout << "  --------------------------|-------------" << endl;
    for (int i = 0; i < TOTAL_MODULES; i++)
    {
        cout << "  " << modules[i].module_name
             << " |   " << modules[i].latency_ms << " ms";
        if (modules[i].is_bottleneck) cout << "  <-- SLOWEST";
        cout << endl;
    }
    cout << "\n  Formula: Latency = Finish_Time - Start_Time" << endl;
}

// DETECT BOTTLENECKS
// Time Complexity: O(n)
void monitoring_layer::detect_bottlenecks()
{
    int bi = find_bottleneck_index();
    cout << "\r\n  +================================================+" << endl;
    cout << "  |           BOTTLENECK DETECTION SCAN           |" << endl;
    cout << "  +================================================+" << endl;

    for (int i = 0; i < TOTAL_MODULES; i++)
    {
        int bars = (int)(modules[i].load_percent / 10);
        string bar = "[";
        for (int b = 0; b < 10; b++)
            bar += (b < bars ? "#" : "-");
        bar += "]";

        cout << "  | " << modules[i].module_name
             << "  " << bar << " " << modules[i].load_percent << "%"
             << "  " << modules[i].latency_ms << "ms";
        if (i == bi) cout << "  *** BOTTLENECK ***";
        cout << endl;
    }

    cout << "  +------------------------------------------------+" << endl;
    cout << "  |  PRIMARY BOTTLENECK : " << modules[bi].module_name << endl;
    cout << "  |  Load               : " << modules[bi].load_percent << "%" << endl;
    cout << "  |  Estimated Latency  : " << modules[bi].latency_ms << " ms" << endl;

    if      (modules[bi].load_percent > 80.0f) cout << "  |  Severity: CRITICAL                            |" << endl;
    else if (modules[bi].load_percent > 50.0f) cout << "  |  Severity: MODERATE                            |" << endl;
    else                                       cout << "  |  Severity: LOW - system healthy                |" << endl;
    cout << "  +================================================+" << endl;
}

// OPTIMIZE PERFORMANCE
// Time Complexity: O(n)
void monitoring_layer::optimize_performance()
{
    bool any = false;
    for (int i = 0; i < TOTAL_MODULES; i++)
    {
        if (modules[i].load_percent >= 50.0f)
        {
            any = true;
            cout << "\n  [!] " << modules[i].module_name
                 << "  Load=" << modules[i].load_percent << "%" << endl;

            if      (i == 0) cout << "      Suggestion: Use spatial interpolation to reduce scan range." << endl;
            else if (i == 1) cout << "      Suggestion: Clear stale events from L1. Archive old anomalies." << endl;
            else if (i == 2) cout << "      Suggestion: Apply load balancing (Q2->Q1). Dequeue processed Q3 events." << endl;
            else if (i == 3) cout << "      Suggestion: Cache T10/T11/T12 decisions. Avoid rebuilding unchanged trees." << endl;
            else if (i == 4) cout << "      Suggestion: Limit BFS depth during non-emergency. Use cached paths." << endl;
            else if (i == 5) cout << "      Suggestion: Expand H1 table size. Clear old H3 cache entries." << endl;
        }
    }

    if (!any)
        cout << "\n  All modules within normal load. No optimization needed." << endl;

    cout << "\n  General Strategy:" << endl;
    cout << "   1. Emergency tasks get full resources (Q3 priority)" << endl;
    cout << "   2. Non-critical modules throttled during high load"  << endl;
    cout << "   3. H3 cache ensures repeated queries are instant"    << endl;
    cout << "   4. BFS/DFS limited to affected zones during fire"    << endl;
}

// VIEW SYSTEM HEALTH
// Time Complexity: O(n)
void monitoring_layer::view_system_health()
{
    int healthy = 0, warning = 0, overloaded = 0;

    for (int i = 0; i < TOTAL_MODULES; i++)
    {
        if      (modules[i].health_status == "Healthy")         healthy++;
        else if (modules[i].health_status == "WARNING")         warning++;
        else                                                    overloaded++;
    }

    cout << "\r\n  +================================================+" << endl;
    cout << "  |          SYSTEM HEALTH DASHBOARD               |" << endl;
    cout << "  +================================================+" << endl;
    cout << "  |  Total Modules : " << TOTAL_MODULES
         << string(31, ' ') << "|" << endl;
    cout << "  |  [ OK ]  Healthy    : " << healthy
         << string(27, ' ') << "|" << endl;
    cout << "  |  [WARN] Warning    : " << warning
         << string(27, ' ') << "|" << endl;
    cout << "  |  [CRIT] Overloaded : " << overloaded
         << string(27, ' ') << "|" << endl;
    cout << "  +------------------------------------------------+" << endl;

    for (int i = 0; i < TOTAL_MODULES; i++)
    {
        string badge;
        if      (modules[i].health_status == "Healthy")   badge = "[ OK ]";
        else if (modules[i].health_status == "WARNING")   badge = "[WARN]";
        else                                              badge = "[CRIT]";

        // load bar
        int bars = (int)(modules[i].load_percent / 10);
        string bar = "[";
        for (int b = 0; b < 10; b++)
            bar += (b < bars ? "#" : "-");
        bar += "]";

        cout << "  | " << badge << " " << modules[i].module_name
             << "  " << bar << " " << modules[i].load_percent << "%";
        if (modules[i].is_bottleneck) cout << " << BOTTLENECK";
        cout << endl;
    }

    cout << "  +------------------------------------------------+" << endl;
    if      (overloaded > 0) cout << "  |  OVERALL: !! SYSTEM UNDER STRESS !!           |" << endl;
    else if (warning    > 0) cout << "  |  OVERALL: System Stable with Warnings         |" << endl;
    else                     cout << "  |  OVERALL: System Fully Healthy                |" << endl;
    cout << "  +================================================+" << endl;
}