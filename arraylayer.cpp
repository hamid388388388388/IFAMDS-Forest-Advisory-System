#include "arraylayer.h"

// Constructor - initialize size and clear dynamic grid
// Time Complexity: O(n^2) where n=3 (grid size), effectively O(1)
arraylayer::arraylayer()
{
    size = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            dynamicgrid[i][j] = 0;
        }
    }
}

// add_sensor_data() - Store one sensor reading (temp, humidity, smoke) into 1D arrays
// Called during data ingestion from sensor nodes
// Time Complexity: O(1) - direct index insertion
void arraylayer::add_sensor_data(int t, int h, int s)
{
    if (size < 100)
    {
        sensor_temp[size] = t;
        sensor_humidity[size] = h;
        sensor_smoke_lvl[size] = s;
        size++;
    }
}

// display_sensor_data() - Print all stored sensor readings sequentially
// Time Complexity: O(n) - single pass through all n sensor readings
void arraylayer::display_sensor_data()
{
    cout << "\n  +============================================+" << endl;
    cout << "  |        SENSOR READINGS - ALL ZONES         |" << endl;
    cout << "  +============================================+" << endl;
    for (int i = 0; i < size; i++)
    {
        int r = i / 3, c = i % 3;
        cout << "  | Zone (" << r << "," << c << ")                              |" << endl;
        cout << "  |  Temperature : " << sensor_temp[i]     << " C"
             << string(28 - to_string(sensor_temp[i]).length(), ' ') << "|" << endl;
        cout << "  |  Humidity    : " << sensor_humidity[i] << " %"
             << string(28 - to_string(sensor_humidity[i]).length(), ' ') << "|" << endl;
        cout << "  |  Smoke Level : " << sensor_smoke_lvl[i]
             << string(29 - to_string(sensor_smoke_lvl[i]).length(), ' ') << "|" << endl;
        cout << "  +--------------------------------------------+" << endl;
    }
}

// display_static_grid() - Display the fixed 3x3 baseline reference grid
// Static array: values never change during execution (normal forest conditions)
// Time Complexity: O(n^2) where n=3 (grid rows/cols), effectively O(1)
void arraylayer::display_static_grid()
{
    cout << "\n  +============================================+" << endl;
    cout << "  |        STATIC FOREST GRID (Baseline)        |" << endl;
    cout << "  +============================================+" << endl;
    cout << "  |        Col-0    Col-1    Col-2              |" << endl;
    cout << "  +--------------------------------------------+" << endl;
    for (int i = 0; i < 3; i++)
    {
        cout << "  | Row-" << i << "  ";
        for (int j = 0; j < 3; j++)
        {
            cout << "[ " << staticgrid[i][j] << " ]   ";
        }
        cout << "         |" << endl;
    }
    cout << "  +============================================+" << endl;
}

// calculate_risk() - Compute a simple risk score from sensor readings
// Formula: risk = temperature + smoke - humidity
// Higher score = higher danger level
// Time Complexity: O(1) - single arithmetic operation
int arraylayer::calculate_risk(int t, int s, int h)
{
    return (t + s - h);
}

// auto_fill_dynamic_grid() - Map 1D sensor readings into 2D 3x3 dynamic grid
// Each sensor index i maps to grid position (i/3, i%3)
// Dynamic array: values change as new sensor data arrives
// Time Complexity: O(n) where n = min(size, 9) sensor readings
void arraylayer::auto_fill_dynamic_grid()
{
    for (int i = 0; i < size && i < 9; i++)
    {
        int r = i / 3;
        int c = i % 3;
        int risk = calculate_risk(sensor_temp[i], sensor_smoke_lvl[i], sensor_humidity[i]);
        dynamicgrid[r][c] = risk;
    }
}

// display_dynamic_grid() - Display the 2D terrain grid, auto-interpolating missing cells
// Cells with value 0 are treated as missing and filled via spatial interpolation
// Time Complexity: O(n^2) where n=3 (grid size), effectively O(1)
void arraylayer::display_dynamic_grid()
{
    cout << "\n  +============================================+" << endl;
    cout << "  |       DYNAMIC TERRAIN GRID (Risk Scores)   |" << endl;
    cout << "  +============================================+" << endl;
    cout << "  |        Col-0    Col-1    Col-2              |" << endl;
    cout << "  +--------------------------------------------+" << endl;
    for (int i = 0; i < 3; i++)
    {
        cout << "  | Row-" << i << "  ";
        for (int j = 0; j < 3; j++)
        {
            if (dynamicgrid[i][j] == 0)
            {
                autoInterpolate(i, j);
            }
            cout << "[ " << dynamicgrid[i][j] << " ]\t";
        }
        cout << "    |" << endl;
    }
    cout << "  +============================================+" << endl;
}

// autoInterpolate() - Estimate a missing grid cell value using 4 adjacent neighbours
// Formula: value = (top + bottom + left + right) / count_of_valid_neighbours
// Used when a sensor fails and its zone value is missing (0)
// Time Complexity: O(1) - checks at most 4 neighbours (fixed grid size)
void arraylayer::autoInterpolate(int r, int c)
{
    int tempGrid[3][3];

    // copy original grid to avoid modifying it during calculation
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            tempGrid[i][j] = dynamicgrid[i][j];
        }
    }

    int sum = 0, count = 0;

    // top neighbour
    if (r - 1 >= 0 && tempGrid[r - 1][c] > 0)
    {
        sum += tempGrid[r - 1][c];
        count++;
    }

    // bottom neighbour
    if (r + 1 < 3 && tempGrid[r + 1][c] > 0)
    {
        sum += tempGrid[r + 1][c];
        count++;
    }

    // left neighbour
    if (c - 1 >= 0 && tempGrid[r][c - 1] > 0)
    {
        sum += tempGrid[r][c - 1];
        count++;
    }

    // right neighbour
    if (c + 1 < 3 && tempGrid[r][c + 1] > 0)
    {
        sum += tempGrid[r][c + 1];
        count++;
    }

    if (count > 0)
    {
        dynamicgrid[r][c] = sum / count;
    }
}

// check_anomilies() - Scan all sensor readings and flag threshold violations
// Rules: temp > 45 = fire risk | humidity < 20 = dry | smoke > 70 = fire possible
// Time Complexity: O(n) - single pass through n sensor readings
void arraylayer::check_anomilies()
{
    cout << "\n  +============================================+" << endl;
    cout << "  |          ANOMALY DETECTION REPORT          |" << endl;
    cout << "  +============================================+" << endl;
    bool any = false;
    for (int i = 0; i < size; i++)
    {
        int r = i / 3, c = i % 3;
        bool flagged = false;
        string alerts = "";
        if (sensor_temp[i] > 45)      { alerts += "  [!] FIRE RISK     - High Temperature\n"; flagged = true; }
        if (sensor_humidity[i] < 20)  { alerts += "  [!] DRY CONDITION - Low Humidity\n";     flagged = true; }
        if (sensor_smoke_lvl[i] > 70) { alerts += "  [!] FIRE POSSIBLE - High Smoke\n";       flagged = true; }
        if (flagged)
        {
            any = true;
            cout << "  | Zone (" << r << "," << c << ")  Temp=" << sensor_temp[i]
                 << "  Hum=" << sensor_humidity[i]
                 << "  Smoke=" << sensor_smoke_lvl[i] << endl;
            cout << alerts;
            cout << "  +--------------------------------------------+" << endl;
        }
    }
    if (!any)
    {
        cout << "  |  No anomalies detected. All zones normal.  |" << endl;
        cout << "  +============================================+" << endl;
    }
}

// check_boundary() - Detect sharp value changes between adjacent grid zones
// A boundary is flagged when |zone_A - zone_B| > 25 (sudden environmental shift)
// Used to detect potential fire spread between zones
// Time Complexity: O(n^2) where n=3, effectively O(1)
void arraylayer::check_boundary()
{
    cout << "\n  +============================================+" << endl;
    cout << "  |        ZONE BOUNDARY ANALYSIS              |" << endl;
    cout << "  +============================================+" << endl;
    int diff;
    bool found = false;
    for (int i = 0; i < 3 && i < size; i++)
    {
        for (int j = 0; j < 3 && j < size; j++)
        {
            if (j + 1 < 3)
            {
                diff = abs(dynamicgrid[i][j] - dynamicgrid[i][j + 1]);
                if (diff > 25)
                {
                    found = true;
                    cout << "  | >> Fire-spread boundary detected!          |" << endl;
                    cout << "  |    Zone (" << i << "," << j << ")  <-->  Zone (" << i << "," << j+1 << ")"
                         << "   diff=" << diff << endl;
                    cout << "  +--------------------------------------------+" << endl;
                }
            }
            if (i + 1 < 3)
            {
                diff = abs(dynamicgrid[i][j] - dynamicgrid[i + 1][j]);
                if (diff > 25)
                {
                    found = true;
                    cout << "  | >> Fire-spread boundary detected!          |" << endl;
                    cout << "  |    Zone (" << i << "," << j << ")  <-->  Zone (" << i+1 << "," << j << ")"
                         << "   diff=" << diff << endl;
                    cout << "  +--------------------------------------------+" << endl;
                }
            }
        }
    }
    if (!found)
    {
        cout << "  |  No significant boundaries found.          |" << endl;
        cout << "  +============================================+" << endl;
    }
}

// generate_events() - Convert sensor readings into classified event nodes for singly linked list
// Classifies each reading as: Extreme alert / High fire / Critical / Fire Risk / Smoke / Dry
// Time Complexity: O(n) - single pass through n sensor readings
void arraylayer::generate_events(linklist_layer &elist)
{
    for (int i = 0; i < size; i++)
    {
        int t = sensor_temp[i];
        int h = sensor_humidity[i];
        int s = sensor_smoke_lvl[i];
        int r = i / 3;
        int c = i % 3;
        if (t > 45 && s > 70 && h < 20)
        {
            elist.inserting_event("Extreme alert", t + s - h, i, r, c);
        }
        else if (t > 45 && s > 70)
        {
            elist.inserting_event("High fire", t + s, i, r, c);
        }
        else if (t > 45 && h < 20)
        {
            elist.inserting_event("critical condition", t - h, i, r, c);
        }
        else
        {
            if (t > 45)
                elist.inserting_event("Fire Risk", t, i, r, c);
            if (s > 70)
                elist.inserting_event("smoke alert", s, i, r, c);
            if (h < 20)
                elist.inserting_event("dry condition", h, i, r, c);
        }
    }
}

// generate_events_doubly() - Same classification logic but inserts into doubly linked list
// Doubly linked list allows forward AND backward traversal for correction chains
// Time Complexity: O(n) - single pass through n sensor readings
void arraylayer::generate_events_doubly(doubly_linklist_layer &dlist)
{
    for (int i = 0; i < size; i++)
    {
        int t = sensor_temp[i];
        int h = sensor_humidity[i];
        int s = sensor_smoke_lvl[i];
        int r = i / 3;
        int c = i % 3;
        if (t > 45 && s > 70 && h < 20)
        {
            dlist.inserting_event("Extreme alert", t + s - h, i, r, c);
        }
        else if (t > 45 && s > 70)
        {
            dlist.inserting_event("High fire", t + s, i, r, c);
        }
        else if (t > 45 && h < 20)
        {
            dlist.inserting_event("critical condition", t - h, i, r, c);
        }
        else
        {
            if (t > 45)
                dlist.inserting_event("Fire Risk", t, i, r, c);
            if (s > 70)
                dlist.inserting_event("smoke alert", s, i, r, c);
            if (h < 20)
                dlist.inserting_event("dry condition", h, i, r, c);
        }
    }
}

// generate_events_circular() - Same classification but inserts into circular linked list
// Circular list used for continuous monitoring loops (last node points back to first)
// Time Complexity: O(n) - single pass through n sensor readings
void arraylayer::generate_events_circular(circular_linklist_layer &clist)
{
    for (int i = 0; i < size; i++)
    {
        int t = sensor_temp[i];
        int h = sensor_humidity[i];
        int s = sensor_smoke_lvl[i];
        int r = i / 3;
        int c = i % 3;
        if (t > 45 && s > 70 && h < 20)
        {
            clist.inserting_event("Extreme alert", t + s - h, i, r, c);
        }
        else if (t > 45 && s > 70)
        {
            clist.inserting_event("High fire", t + s, i, r, c);
        }
        else if (t > 45 && h < 20)
        {
            clist.inserting_event("critical condition", t - h, i, r, c);
        }
        else
        {
            if (t > 45)
                clist.inserting_event("Fire Risk", t, i, r, c);
            if (s > 70)
                clist.inserting_event("smoke alert", s, i, r, c);
            if (h < 20)
                clist.inserting_event("dry condition", h, i, r, c);
        }
    }
}

// verified_event_stream() - Filter out noisy/invalid readings before storing in linked list
// Noise rule: if |current - previous| > threshold(20), skip the reading
// Invalid rule: skip if any value <= 0 (physically impossible)
// Time Complexity: O(n) - single pass through n sensor readings
void arraylayer::verified_event_stream(linklist_layer &llist)
{
    for (int i = 0; i < size; i++)
    {
        int t = sensor_temp[i];
        int h = sensor_humidity[i];
        int s = sensor_smoke_lvl[i];
        int r = i / 3;
        int c = i % 3;

        int threshold = 20;

        // skip physically invalid readings
        if (t <= 0 || h <= 0 || s <= 0)
        {
            continue;
        }

        // skip noisy readings (sudden jumps beyond threshold)
        if (i > 0)
        {
            if (abs(t - sensor_temp[i - 1]) > threshold ||
                abs(h - sensor_humidity[i - 1]) > threshold ||
                abs(s - sensor_smoke_lvl[i - 1]) > threshold)
            {
                continue;
            }

            llist.inserting_verified_event_stream(t, h, s, i, r, c);
        }
    }
}

// generate_anomaly_stream() - Compare consecutive events and flag large value changes
// Anomaly condition: |current - previous| > threshold(20) for any sensor dimension
// Time Complexity: O(n) - single pass through linked list of n events
void arraylayer::generate_anomaly_stream(linklist_layer &llist, linklist_layer &anomaly_list)
{
    event_node *temp = llist.gethead();
    if (temp == NULL)
    {
        return;
    }
    event_node *prev = temp;
    temp = temp->next;
    int threshold = 20;
    while (temp != NULL)
    {
        int t = temp->temperature;
        int h = temp->humidity;
        int s = temp->smoke_lvl;

        int pt = prev->temperature;
        int ph = prev->humidity;
        int ps = prev->smoke_lvl;

        int i = temp->time;
        int r = temp->row;
        int c = temp->col;

        if (abs(pt - t) > threshold)
        {
            anomaly_list.inserting_event("Temperature anomaly ", t, i, r, c);
        }
        if (abs(ph - h) > threshold)
        {
            anomaly_list.inserting_event("Humidity anomaly ", h, i, r, c);
        }
        if (abs(ps - s) > threshold)
        {
            anomaly_list.inserting_event("smoke level anomaly ", s, i, r, c);
        }
        prev = temp;
        temp = temp->next;
    }
}

// forward_correction_chain() - Smooth large jumps between consecutive events going forward
// If |prev - curr| > threshold, replace curr with average of both values
// Builds a corrected version of the verified list in forward_correction_list
// Time Complexity: O(n) - single forward pass through linked list of n events
void arraylayer::forward_correction_chain(linklist_layer &verified_list, linklist_layer &forward_correcction_list)
{
    event_node *temp = verified_list.gethead();
    event_node *prev = temp;
    int threshold = 20;
    if (temp == NULL)
    {
        cout << "your verified link list is empty " << endl;
    }

    // insert first node as-is (no previous to compare with)
    forward_correcction_list.inserting_verified_event_stream(temp->temperature, temp->humidity, temp->smoke_lvl, temp->time, temp->row, temp->col);

    while (temp != NULL)
    {
        int t = temp->temperature;
        int h = temp->humidity;
        int s = temp->smoke_lvl;

        int pt = prev->temperature;
        int ph = prev->humidity;
        int ps = prev->smoke_lvl;

        int i = temp->time;
        int r = temp->row;
        int c = temp->col;

        // smooth temperature spike
        if (abs(pt - t) > threshold)
        {
            t = (pt + t) / 2;
        }

        // smooth humidity spike
        if (abs(ph - h) > threshold)
        {
            h = (ph + h) / 2;
        }

        // smooth smoke spike
        if (abs(ps - s) > threshold)
        {
            s = (ps + s) / 2;
        }

        forward_correcction_list.inserting_verified_event_stream(t, h, s, i, r, c);

        prev = temp;
        temp = temp->next;
    }
}

// convert_verify_singly_to_doubly() - Copy singly linked list into doubly linked list
// Required before backward correction (doubly list allows reverse traversal)
// Time Complexity: O(n) - single pass through n nodes
void arraylayer::convert_verify_singly_to_doubly(linklist_layer &llist, doubly_linklist_layer &doubly_verified_list)
{
    event_node *temp = llist.gethead();
    int t, h, s, i, r, c;
    while (temp != NULL)
    {
        t = temp->temperature;
        h = temp->humidity;
        s = temp->smoke_lvl;
        i = temp->time;
        r = temp->row;
        c = temp->col;
        doubly_verified_list.inserting_event(t, h, s, i, r, c);
        temp = temp->next;
    }
}

// backward_correction_chain() - Traverse doubly list in REVERSE and smooth value jumps
// Goes from tail to head, correcting previous node values using current node data
// Time Complexity: O(n) - single backward pass through n nodes of doubly linked list
void arraylayer::backward_correction_chain(doubly_linklist_layer &dlist)
{
    event_node *curr = dlist.getTail();

    if (curr == NULL)
    {
        cout << "List empty\n";
        return;
    }

    int threshold = 20;

    // traverse backward from tail to head
    while (curr != NULL && curr->prev != NULL)
    {
        event_node *prev = curr->prev;

        // current node values
        int ct = curr->temperature;
        int ch = curr->humidity;
        int cs = curr->smoke_lvl;

        // previous node values (modify in-place via reference)
        int &pt = prev->temperature;
        int &ph = prev->humidity;
        int &ps = prev->smoke_lvl;

        if (abs(ct - pt) > threshold)
            pt = (ct + pt) / 2;

        if (abs(ch - ph) > threshold)
            ph = (ch + ph) / 2;

        if (abs(cs - ps) > threshold)
            ps = (cs + ps) / 2;

        curr = curr->prev;
    }
}

// state_synchronization() - Sync all corrected events from doubly list to global singly + circular lists
// Ensures all modules see the same updated environmental state (consistency rule)
// Time Complexity: O(n) - single pass through n nodes of doubly linked list
void arraylayer::state_synchronization(doubly_linklist_layer &source, linklist_layer &global_list, circular_linklist_layer &global_loop)
{
    event_node *temp = source.gethead();
    while (temp != NULL)
    {
        int t = temp->temperature;
        int h = temp->humidity;
        int s = temp->smoke_lvl;
        int i = temp->time;
        int r = temp->row;
        int c = temp->col;
        global_list.inserting_verified_event_stream(t, h, s, i, r, c);
        global_loop.inserting_event("synced", t, h, s, i, r, c);
        temp = temp->next;
    }
}