#ifndef GRAPH_LAYER_H
#define GRAPH_LAYER_H

#include <iostream>
#include <string>
using namespace std;

// GRAPH LAYER - Spatial Connectivity and Routing
// Represents forest as a graph:
//   - Nodes  = forest zones (Zone_0 .. Zone_8 from 3x3 grid)
//   - Edges  = paths between adjacent zones
// Two representations:
//   G1 - Adjacency List  (sparse connections, realistic forest)
//   G2 - Adjacency Matrix (dense grid, used for path checking)

#define MAX_ZONES 9       // 3x3 grid -> 9 zones total
#define MAX_ADJ   4       // Maximum neighbors per zone (North/South/East/West)
#define INF       99999   // Represents no connection between zones

// ------ Adjacency List Node ------
// Each node in the linked list stores one neighbor connection and its cost
struct adj_node
{
    int   neighbor_zone;   // Index of the connected zone (0-8)
    float edge_weight;     // Path cost = distance + danger level
    adj_node *next;        // Next neighbor in the list

    adj_node(int n, float w)
    {
        neighbor_zone = n;
        edge_weight   = w;
        next          = NULL;
    }
};

// ------ Zone Info ------
// Stores environmental sensor data for each forest zone
struct zone_info
{
    int   zone_id;         // Zone number (0-8)
    int   temperature;     // Temperature reading
    int   humidity;        // Humidity reading
    int   smoke_lvl;       // Smoke level reading
    float fire_level;      // Fire danger level (0.0 to 1.0)
    bool  blocked;         // True if zone is on fire or impassable
    string zone_name;      // Name of the zone

    zone_info()
    {
        zone_id     = 0;
        temperature = 0;
        humidity    = 0;
        smoke_lvl   = 0;
        fire_level  = 0.0f;
        blocked     = false;
        zone_name   = "";
    }
};

// Graph Layer Class - Manages forest zone connectivity and routing
class graph_layer
{
private:
    // ---- G1: Adjacency List ----
    // Each zone has a linked list of its neighbors
    adj_node *adj_list[MAX_ZONES];   // Head pointer for each zone's neighbor list

    // ---- G2: Adjacency Matrix ----
    // matrix[i][j] = edge weight between zones i and j (INF = no connection)
    float adj_matrix[MAX_ZONES][MAX_ZONES];

    // ---- Zone data (loaded from array layer) ----
    zone_info zones[MAX_ZONES];  // Sensor data for each zone
    int num_zones;               // Number of zones (usually 9 for 3x3 grid)

    // ---- BFS/DFS helpers ----
    bool visited[MAX_ZONES];     // Tracks visited zones during traversal

    // ---- Internal helper functions ----
    void   add_adj_list_edge(int from, int to, float weight);  // Add edge to adjacency list
    float  compute_path_cost(int from, int to);                // Calculate path cost between zones
    void   print_zone_label(int id);                           // Print zone name and status

public:
    graph_layer();  // Constructor - initializes empty graph

    // --- Setup Functions ---

    // Load zone sensor data from external arrays (called from main)
    // Time Complexity: O(n) where n = number of zones
    void load_zone_data(int temps[], int humidities[], int smokes[], int count);

    // Build G1 (adjacency list) connecting neighboring zones in grid
    // Time Complexity: O(n) where n = number of zones
    void build_adjacency_list();

    // Build G2 (adjacency matrix) with same connections in matrix form
    // Time Complexity: O(n^2) where n = number of zones
    void build_adjacency_matrix();

    // --- Display Functions ---
    void display_adj_list();     // Show adjacency list representation
    void display_adj_matrix();   // Show adjacency matrix representation
    void display_zone_status();  // Show current status of all zones

    // --- Graph Algorithms ---

    // BFS from a start zone - shows how fire spreads level by level
    // Time Complexity: O(V + E) where V=vertices(zones), E=edges(paths)
    void bfs_fire_spread(int start_zone);

    // DFS from a start zone - performs deep path analysis
    // Time Complexity: O(V + E) where V=vertices(zones), E=edges(paths)
    void dfs_deep_analysis(int start_zone);
    void dfs_helper(int zone);   // Recursive helper for DFS

    // Compute cheapest safe path from source to destination zone
    // Path Cost = Distance + Danger level (from project spec)
    // Time Complexity: O(V^2) using simple Dijkstra-style algorithm
    void compute_safe_path(int src, int dest);

    // Update edge weights when fire level increases in a zone
    // New Cost = Distance * (1 + fire_level) (from project spec)
    // Time Complexity: O(E) where E = number of edges
    void update_blocked_routes(int fire_zone, float fire_level);
};

#endif