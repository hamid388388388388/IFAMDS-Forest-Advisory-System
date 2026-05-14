#include "graph_layer.h"

// CONSTRUCTOR
// Initialize all adjacency list heads to NULL,
// matrix to INF (no connections), visited to false
graph_layer::graph_layer()
{
    num_zones = MAX_ZONES;

    for (int i = 0; i < MAX_ZONES; i++)
    {
        adj_list[i] = NULL;
        visited[i]  = false;

        for (int j = 0; j < MAX_ZONES; j++)
            adj_matrix[i][j] = (i == j) ? 0 : INF;

        zones[i].zone_id   = i;
        zones[i].zone_name = "Zone_" + to_string(i);
    }
}

// LOAD ZONE DATA FROM ARRAY LAYER
// Pulls sensor readings into graph nodes so all layers share
// the same environmental data (realistic integration)
// Time Complexity: O(n)
void graph_layer::load_zone_data(int temps[], int humidities[], int smokes[], int count)
{
    int load = (count < MAX_ZONES) ? count : MAX_ZONES;

    for (int i = 0; i < load; i++)
    {
        zones[i].temperature = temps[i];
        zones[i].humidity    = humidities[i];
        zones[i].smoke_lvl   = smokes[i];

        // fire_level: normalized 0..1 based on temp+smoke
        // formula derived from project spec decision score
        float f = (temps[i] / 100.0f) * 0.5f + (smokes[i] / 100.0f) * 0.5f;
        zones[i].fire_level = f;

        // mark zone blocked if conditions are extreme
        zones[i].blocked = (temps[i] > 45 && smokes[i] > 70);
    }

    cout << "[Graph] Zone data loaded from array layer (" << load << " zones)." << endl;
}

// HELPER: ADD DIRECTED EDGE TO ADJACENCY LIST
// Inserts at head of linked list for zone 'from'
// Time Complexity: O(1)
void graph_layer::add_adj_list_edge(int from, int to, float weight)
{
    adj_node *newnode = new adj_node(to, weight);
    newnode->next     = adj_list[from];
    adj_list[from]    = newnode;
}

// HELPER: COMPUTE BASE PATH COST
// Path Cost = Distance + Danger   (from project spec)
// Distance between adjacent grid zones = 1 unit (normalized)
// Danger = fire_level of destination zone
// Time Complexity: O(1)
float graph_layer::compute_path_cost(int from, int to)
{
    float distance = 1.0f;  // adjacent zones are 1 unit apart
    float danger   = zones[to].fire_level * 10.0f;  // scale to readable number
    return distance + danger;
}

// HELPER: PRINT ZONE LABEL WITH STATUS
void graph_layer::print_zone_label(int id)
{
    cout << zones[id].zone_name;
    if (zones[id].blocked)
        cout << "[BLOCKED]";
}

// BUILD G1: ADJACENCY LIST
// Forest modeled as 3x3 grid. Each zone connects to its
// N/S/E/W neighbors (no diagonals - realistic forest paths)
// Zone numbering:
//   0 1 2
//   3 4 5
//   6 7 8
// Time Complexity: O(n)  n = num_zones
void graph_layer::build_adjacency_list()
{
    // clear existing list
    for (int i = 0; i < MAX_ZONES; i++)
        adj_list[i] = NULL;

    int rows = 3, cols = 3;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            int zone = i * cols + j;

            // right neighbor
            if (j + 1 < cols)
            {
                int right = i * cols + (j + 1);
                float w   = compute_path_cost(zone, right);
                add_adj_list_edge(zone, right, w);
                add_adj_list_edge(right, zone, w);  // undirected
            }

            // bottom neighbor
            if (i + 1 < rows)
            {
                int below = (i + 1) * cols + j;
                float w   = compute_path_cost(zone, below);
                add_adj_list_edge(zone, below, w);
                add_adj_list_edge(below, zone, w);  // undirected
            }
        }
    }

    cout << "[Graph] G1 Adjacency List built (3x3 grid, bidirectional)." << endl;
}

// BUILD G2: ADJACENCY MATRIX
// Same connections as G1 but stored as float matrix
// matrix[i][j] = edge weight,  INF = not connected
// Time Complexity: O(n^2) to initialize, O(E) to fill edges
void graph_layer::build_adjacency_matrix()
{
    // reset
    for (int i = 0; i < MAX_ZONES; i++)
        for (int j = 0; j < MAX_ZONES; j++)
            adj_matrix[i][j] = (i == j) ? 0 : INF;

    int rows = 3, cols = 3;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            int zone = i * cols + j;

            if (j + 1 < cols)
            {
                int right = i * cols + (j + 1);
                float w   = compute_path_cost(zone, right);
                adj_matrix[zone][right] = w;
                adj_matrix[right][zone] = w;
            }
            if (i + 1 < rows)
            {
                int below = (i + 1) * cols + j;
                float w   = compute_path_cost(zone, below);
                adj_matrix[zone][below] = w;
                adj_matrix[below][zone] = w;
            }
        }
    }

    cout << "[Graph] G2 Adjacency Matrix built (3x3 grid)." << endl;
}

// DISPLAY G1: ADJACENCY LIST
// Shows each zone and its connected neighbors with costs
void graph_layer::display_adj_list()
{
    cout << "\r\n  +============================================+" << endl;
    cout << "  |      G1: ZONE ADJACENCY LIST               |" << endl;
    cout << "  |      (Zone --> Neighbors [cost])           |" << endl;
    cout << "  +============================================+" << endl;

    for (int i = 0; i < MAX_ZONES; i++)
    {
        string label = zones[i].zone_name + (zones[i].blocked ? "[BLOCKED]" : "");
        cout << "  | " << label << string(max(0, 10-(int)label.length()), ' ') << " --> ";

        adj_node *curr = adj_list[i];
        if (curr == NULL)
        {
            cout << "No connections" << string(11, ' ') << "|" << endl;
        }
        else
        {
            string row = "";
            while (curr != NULL)
            {
                row += zones[curr->neighbor_zone].zone_name + "[" + to_string(curr->edge_weight).substr(0,3) + "]";
                if (curr->next != NULL) row += " -> ";
                curr = curr->next;
            }
            cout << row << string(max(0, 26-(int)row.length()), ' ') << "|" << endl;
        }
    }
    cout << "  +============================================+" << endl;
}

// DISPLAY G2: ADJACENCY MATRIX
// Shows matrix grid - INF means no connection
void graph_layer::display_adj_matrix()
{
    cout << "\n===== G2: ADJACENCY MATRIX =====" << endl;
    cout << "     ";
    for (int i = 0; i < MAX_ZONES; i++)
        cout << "  Z" << i << "  ";
    cout << endl;

    for (int i = 0; i < MAX_ZONES; i++)
    {
        cout << "Z" << i << "  ";
        for (int j = 0; j < MAX_ZONES; j++)
        {
            if (adj_matrix[i][j] == INF)
                cout << "  INF ";
            else
                cout << "  " << adj_matrix[i][j] << "  ";
        }
        cout << endl;
    }
}

// DISPLAY ZONE STATUS
// Shows current sensor state of all zones from array layer
void graph_layer::display_zone_status()
{
    cout << "\r\n  +================================================================+" << endl;
    cout << "  |              ZONE STATUS  (from Array Layer)                   |" << endl;
    cout << "  +================================================================+" << endl;
    cout << "  | Zone   | Temp | Humid | Smoke | FireLvl | Status               |" << endl;
    cout << "  +--------+------+-------+-------+---------+----------------------+" << endl;

    for (int i = 0; i < MAX_ZONES; i++)
    {
        string status = zones[i].blocked ? "!! FIRE/BLOCKED !!" : "Normal";
        string fl = to_string(zones[i].fire_level).substr(0,4);
        cout << "  | Zone_" << i
             << "  | " << zones[i].temperature << string(5-(int)to_string(zones[i].temperature).length(),' ')
             << "| " << zones[i].humidity      << string(6-(int)to_string(zones[i].humidity).length(),' ')
             << "| " << zones[i].smoke_lvl     << string(6-(int)to_string(zones[i].smoke_lvl).length(),' ')
             << "| " << fl                     << string(8-(int)fl.length(),' ')
             << "| " << status                 << string(max(0,21-(int)status.length()),' ') << "|" << endl;
    }
    cout << "  +================================================================+" << endl;
}

// BFS: BREADTH FIRST SEARCH - Fire Spread Simulation
// Starts from start_zone and visits all reachable zones
// level by level - simulates how fire expands outward
// Time Complexity: O(V + E)
void graph_layer::bfs_fire_spread(int start_zone)
{
    if (start_zone < 0 || start_zone >= MAX_ZONES)
    {
        cout << "  Invalid zone number." << endl;
        return;
    }

    string sl = "Zone_" + to_string(start_zone) + (zones[start_zone].blocked ? "[BLOCKED]" : "");
    cout << "\r\n  +============================================+" << endl;
    cout << "  |  BFS: FIRE SPREAD from " << sl
         << string(max(0, 20-(int)sl.length()), ' ') << "|" << endl;
    cout << "  +============================================+" << endl;

    for (int i = 0; i < MAX_ZONES; i++)
        visited[i] = false;

    int queue[MAX_ZONES];
    int level[MAX_ZONES];
    int front = 0, back = 0;

    queue[back]  = start_zone;
    level[back]  = 0;
    back++;
    visited[start_zone] = true;

    int current_level = -1;

    while (front < back)
    {
        int zone  = queue[front];
        int lvl   = level[front];
        front++;

        if (lvl != current_level)
        {
            current_level = lvl;
            cout << "  |  [Wave " << lvl << "]";
            cout << string(36, '-') << "|" << endl;
        }

        string zn = zones[zone].zone_name + (zones[zone].blocked ? "[BLOCKED]" : "");
        string fire_tag = zones[zone].blocked ? " *** FIRE ***" : "";
        cout << "  |   --> " << zn
             << "  T=" << zones[zone].temperature
             << " S=" << zones[zone].smoke_lvl
             << fire_tag
             << string(max(0, 3 - (int)fire_tag.length()), ' ') << endl;

        adj_node *curr = adj_list[zone];
        while (curr != NULL)
        {
            int neighbor = curr->neighbor_zone;
            if (!visited[neighbor])
            {
                visited[neighbor] = true;
                queue[back]  = neighbor;
                level[back]  = lvl + 1;
                back++;
            }
            curr = curr->next;
        }
    }

    cout << "  +--------------------------------------------+" << endl;
    cout << "  | BFS Done: " << back << " zones reachable from Zone_" << start_zone
         << string(max(0, 9 - (int)to_string(back).length() - (int)to_string(start_zone).length()), ' ') << "|" << endl;
    cout << "  +============================================+" << endl;
}

// DFS: DEPTH FIRST SEARCH - Deep Path Analysis
// Follows one path as deep as possible before backtracking
// Useful for tracing a single fire corridor through forest
// Time Complexity: O(V + E)
void graph_layer::dfs_deep_analysis(int start_zone)
{
    if (start_zone < 0 || start_zone >= MAX_ZONES)
    {
        cout << "  Invalid zone number." << endl;
        return;
    }

    string sl = "Zone_" + to_string(start_zone);
    cout << "\r\n  +============================================+" << endl;
    cout << "  |  DFS: DEEP PATH ANALYSIS from " << sl
         << string(max(0, 13-(int)sl.length()), ' ') << "|" << endl;
    cout << "  +============================================+" << endl;

    for (int i = 0; i < MAX_ZONES; i++)
        visited[i] = false;

    dfs_helper(start_zone);

    cout << "  +============================================+" << endl;
    cout << "  |  [DFS Complete]                            |" << endl;
    cout << "  +============================================+" << endl;
}

void graph_layer::dfs_helper(int zone)
{
    visited[zone] = true;
    string zn = zones[zone].zone_name + (zones[zone].blocked ? "[BLOCKED]" : "");
    cout << "  |   --> " << zn
         << "  T=" << zones[zone].temperature
         << "  FireLvl=" << zones[zone].fire_level << endl;

    adj_node *curr = adj_list[zone];
    while (curr != NULL)
    {
        if (!visited[curr->neighbor_zone])
            dfs_helper(curr->neighbor_zone);
        curr = curr->next;
    }
}

// COMPUTE SAFE PATH
// Finds lowest-cost path from src to dest
// Uses simple greedy selection (Dijkstra-style, no STL)
// Skips blocked zones (fire zones)
// Path Cost = Distance + Danger  (from project spec)
// Time Complexity: O(V^2)
void graph_layer::compute_safe_path(int src, int dest)
{
    if (src < 0 || src >= MAX_ZONES || dest < 0 || dest >= MAX_ZONES)
    {
        cout << "Invalid zone numbers." << endl;
        return;
    }

    cout << "\r\n  +============================================+" << endl;
    cout << "  |  SAFE PATH: Zone_" << src << " --> Zone_" << dest
         << string(max(0, 22 - (int)to_string(src).length() - (int)to_string(dest).length()), ' ') << "|" << endl;
    cout << "  +============================================+" << endl;

    float dist[MAX_ZONES];
    int   prev[MAX_ZONES];
    bool  done[MAX_ZONES];

    for (int i = 0; i < MAX_ZONES; i++)
    {
        dist[i] = INF;
        prev[i] = -1;
        done[i] = false;
    }
    dist[src] = 0;

    for (int step = 0; step < MAX_ZONES; step++)
    {
        // pick unvisited zone with smallest distance
        int u = -1;
        for (int i = 0; i < MAX_ZONES; i++)
        {
            if (!done[i] && (u == -1 || dist[i] < dist[u]))
                u = i;
        }

        if (u == -1 || dist[u] == INF) break;
        done[u] = true;

        // relax neighbors
        adj_node *curr = adj_list[u];
        while (curr != NULL)
        {
            int v = curr->neighbor_zone;

            // skip blocked zones (fire zone = unsafe)
            if (!zones[v].blocked && !done[v])
            {
                float new_dist = dist[u] + curr->edge_weight;
                if (new_dist < dist[v])
                {
                    dist[v] = new_dist;
                    prev[v] = u;
                }
            }
            curr = curr->next;
        }
    }

    // reconstruct path
    if (dist[dest] == INF)
    {
        cout << "  | !! No safe path found !!                   |" << endl;
        cout << "  |  All routes to Zone_" << dest << " are blocked.     |" << endl;
        cout << "  +============================================+" << endl;
        return;
    }

    int path[MAX_ZONES];
    int path_len = 0;
    int cur = dest;
    while (cur != -1)
    {
        path[path_len++] = cur;
        cur = prev[cur];
    }

    cout << "  | Safe Path Found  (total cost = " << dist[dest] << ")" << endl;
    cout << "  | Route: ";
    for (int i = path_len - 1; i >= 0; i--)
    {
        string zn = zones[path[i]].zone_name;
        cout << zn;
        if (i > 0) cout << " --> ";
    }
    cout << endl;
    cout << "  +============================================+" << endl;
}

// UPDATE BLOCKED ROUTES
// When fire level increases in a zone, recalculate edge weights
// Formula from project spec:
//   Updated Cost = Distance * (1 + fire_level)
// Time Complexity: O(E)  - scans all adjacency list edges
void graph_layer::update_blocked_routes(int fire_zone, float fire_level)
{
    if (fire_zone < 0 || fire_zone >= MAX_ZONES)
    {
        cout << "Invalid zone." << endl;
        return;
    }

    cout << "\n===== UPDATE BLOCKED ROUTES: Zone_" << fire_zone
         << " FireLevel=" << fire_level << " =====" << endl;

    // update zone's fire level
    zones[fire_zone].fire_level = fire_level;

    // if fire is extreme, mark zone blocked
    if (fire_level > 0.7f)
    {
        zones[fire_zone].blocked = true;
        cout << "  Zone_" << fire_zone << " marked BLOCKED (fire_level > 0.7)" << endl;
    }

    // recalculate weights for all edges entering this zone
    for (int i = 0; i < MAX_ZONES; i++)
    {
        adj_node *curr = adj_list[i];
        while (curr != NULL)
        {
            if (curr->neighbor_zone == fire_zone)
            {
                float old_w = curr->edge_weight;
                // Updated Cost = Distance * (1 + fire_level)
                float new_w = 1.0f * (1.0f + fire_level);
                curr->edge_weight = new_w;

                cout << "  Edge Zone_" << i << " -> Zone_" << fire_zone
                     << "  cost: " << old_w << " --> " << new_w << endl;

                // also update matrix
                adj_matrix[i][fire_zone] = new_w;
                adj_matrix[fire_zone][i] = new_w;
            }
            curr = curr->next;
        }
    }

    cout << "[Route update complete]" << endl;
}