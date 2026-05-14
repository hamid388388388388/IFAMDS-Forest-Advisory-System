#include "hash_layer.h"

// CONSTRUCTOR
// Initialize H1 slots as empty, H2 chains as NULL,
// H3 cache as invalid, cache pointer at 0
hash_layer::hash_layer()
{
    for (int i = 0; i < H1_SIZE; i++)
    {
        H1[i].is_occupied = false;
        H2[i] = NULL;
    }
    for (int i = 0; i < H3_SIZE; i++)
        H3[i].valid = false;

    cache_next = 0;
}

// HASH FUNCTION
// Formula from project spec: Index = Key mod TableSize
// Time Complexity: O(1)
int hash_layer::hash_function(int zone_id)
{
    return zone_id % H1_SIZE;
}

// HELPER: COMPUTE STATUS STRING
string hash_layer::compute_status(int temp, int humidity, int smoke)
{
    if (temp > 45 && smoke > 70 && humidity < 20)
        return "!! FIRE RISK !!";
    else if (temp > 45)
        return "High Temp Warning";
    else if (smoke > 70)
        return "High Smoke Warning";
    else if (humidity < 20)
        return "Dry Condition";
    else
        return "Normal";
}

// HELPER: COMPUTE RISK SCORE
// Same formula as tree layer for consistency:
// Score = 0.4*(temp/100) + 0.3*(smoke/100) + 0.3*((100-hum)/100)
// Time Complexity: O(1)
float hash_layer::compute_risk(int temp, int smoke, int humidity)
{
    float f = temp  / 100.0f;
    float s = smoke / 100.0f;
    float h = (100 - humidity) / 100.0f;
    return 0.4f * f + 0.3f * s + 0.3f * h;
}

// H3 CACHE INSERT
// Circular write - newest replaces oldest when full
// Time Complexity: O(1)
void hash_layer::cache_insert(int zone_id, int temp, int hum, int smoke)
{
    H3[cache_next].zone_id     = zone_id;
    H3[cache_next].temperature = temp;
    H3[cache_next].humidity    = hum;
    H3[cache_next].smoke_lvl   = smoke;
    H3[cache_next].valid       = true;
    cache_next = (cache_next + 1) % H3_SIZE;  // circular
}

// H3 CACHE SEARCH
// Returns true and prints data if zone found in cache
// Time Complexity: O(cache_size) = O(5) = O(1)
bool hash_layer::cache_search(int zone_id)
{
    for (int i = 0; i < H3_SIZE; i++)
    {
        if (H3[i].valid && H3[i].zone_id == zone_id)
        {
            cout << "  [H3 CACHE HIT] Zone_" << zone_id
                 << "  Temp=" << H3[i].temperature
                 << "  Humidity=" << H3[i].humidity
                 << "  Smoke=" << H3[i].smoke_lvl << endl;
            return true;
        }
    }
    return false;
}

// INSERT - H1 Primary Table
// Step 1: compute index = zone_id % H1_SIZE
// Step 2: if slot free -> store in H1 directly
// Step 3: if slot taken (collision) -> store in H2 chain
// Time Complexity: O(1) average
void hash_layer::insert(int zone_id, int temp, int humidity, int smoke)
{
    int index = hash_function(zone_id);

    cout << "  Inserting Zone_" << zone_id
         << "  -->  Index = " << zone_id << " % " << H1_SIZE
         << " = " << index;

    if (!H1[index].is_occupied)
    {
        // Slot free - direct insert into H1
        H1[index].zone_id     = zone_id;
        H1[index].temperature = temp;
        H1[index].humidity    = humidity;
        H1[index].smoke_lvl   = smoke;
        H1[index].risk_score  = compute_risk(temp, smoke, humidity);
        H1[index].status      = compute_status(temp, humidity, smoke);
        H1[index].is_occupied = true;
        cout << "  --> Stored in H1[" << index << "]" << endl;
    }
    else
    {
        // Collision - store in H2 chaining
        cout << "  --> COLLISION at H1[" << index << "]"
             << "  (Zone_" << H1[index].zone_id << " already there)"
             << "  --> Chained in H2" << endl;

        collision_node *newnode = new collision_node();
        newnode->data.zone_id     = zone_id;
        newnode->data.temperature = temp;
        newnode->data.humidity    = humidity;
        newnode->data.smoke_lvl   = smoke;
        newnode->data.risk_score  = compute_risk(temp, smoke, humidity);
        newnode->data.status      = compute_status(temp, humidity, smoke);
        newnode->data.is_occupied = true;

        // insert at head of chain
        newnode->next = H2[index];
        H2[index]     = newnode;
    }

    // also add to H3 cache
    cache_insert(zone_id, temp, humidity, smoke);
}

// RETRIEVE - O(1) access
// Check H3 cache first (fastest), then H1, then H2 chain
// Time Complexity: O(1) average
void hash_layer::retrieve(int zone_id)
{
    cout << "\n  Retrieving Zone_" << zone_id
         << "  (index = " << zone_id << " % " << H1_SIZE
         << " = " << hash_function(zone_id) << ")" << endl;

    // Step 1: Check H3 cache first
    if (cache_search(zone_id))
        return;   // cache hit - done

    // Step 2: Check H1 primary table
    int index = hash_function(zone_id);

    if (H1[index].is_occupied && H1[index].zone_id == zone_id)
    {
        cout << "  [H1 HIT] Zone_" << zone_id
             << "  Temp=" << H1[index].temperature
             << "  Humidity=" << H1[index].humidity
             << "  Smoke=" << H1[index].smoke_lvl
             << "  Risk=" << H1[index].risk_score
             << "  Status=" << H1[index].status << endl;

        // promote to cache for next time
        cache_insert(zone_id, H1[index].temperature,
                     H1[index].humidity, H1[index].smoke_lvl);
        return;
    }

    // Step 3: Check H2 collision chain
    collision_node *curr = H2[index];
    while (curr != NULL)
    {
        if (curr->data.zone_id == zone_id)
        {
            cout << "  [H2 CHAIN HIT] Zone_" << zone_id
                 << "  Temp=" << curr->data.temperature
                 << "  Humidity=" << curr->data.humidity
                 << "  Smoke=" << curr->data.smoke_lvl
                 << "  Risk=" << curr->data.risk_score
                 << "  Status=" << curr->data.status << endl;

            cache_insert(zone_id, curr->data.temperature,
                         curr->data.humidity, curr->data.smoke_lvl);
            return;
        }
        curr = curr->next;
    }

    cout << "  Zone_" << zone_id << " not found in H1 or H2." << endl;
}

// UPDATE - find and update existing record
// Time Complexity: O(1) average
void hash_layer::update(int zone_id, int temp, int humidity, int smoke)
{
    int index = hash_function(zone_id);

    // Check H1 first
    if (H1[index].is_occupied && H1[index].zone_id == zone_id)
    {
        H1[index].temperature = temp;
        H1[index].humidity    = humidity;
        H1[index].smoke_lvl   = smoke;
        H1[index].risk_score  = compute_risk(temp, smoke, humidity);
        H1[index].status      = compute_status(temp, humidity, smoke);
        cout << "  [H1] Zone_" << zone_id << " updated successfully." << endl;
        cache_insert(zone_id, temp, humidity, smoke);
        return;
    }

    // Check H2 chain
    collision_node *curr = H2[index];
    while (curr != NULL)
    {
        if (curr->data.zone_id == zone_id)
        {
            curr->data.temperature = temp;
            curr->data.humidity    = humidity;
            curr->data.smoke_lvl   = smoke;
            curr->data.risk_score  = compute_risk(temp, smoke, humidity);
            curr->data.status      = compute_status(temp, humidity, smoke);
            cout << "  [H2] Zone_" << zone_id << " updated in collision chain." << endl;
            cache_insert(zone_id, temp, humidity, smoke);
            return;
        }
        curr = curr->next;
    }

    cout << "  Zone_" << zone_id << " not found. Use insert first." << endl;
}

// DISPLAY H1: PRIMARY INDEX TABLE
void hash_layer::display_H1()
{
    cout << "\r\n  +================================================================+" << endl;
    cout << "  |              H1: PRIMARY HASH INDEX TABLE                      |" << endl;
    cout << "  +================================================================+" << endl;
    cout << "  | Slot | ZoneID    | Temp | Humid | Smoke | Risk  | Status       |" << endl;
    cout << "  +------+-----------+------+-------+-------+-------+--------------+" << endl;

    for (int i = 0; i < H1_SIZE; i++)
    {
        cout << "  |  " << i << "   | ";
        if (H1[i].is_occupied)
        {
            string st = H1[i].status;
            string rid = "Zone_" + to_string(H1[i].zone_id);
            string risk = to_string(H1[i].risk_score).substr(0,5);
            cout << rid << string(10 - rid.length(), ' ')
                 << "| " << H1[i].temperature << string(5 - (int)to_string(H1[i].temperature).length(), ' ')
                 << "| " << H1[i].humidity    << string(6 - (int)to_string(H1[i].humidity).length(), ' ')
                 << "| " << H1[i].smoke_lvl   << string(6 - (int)to_string(H1[i].smoke_lvl).length(), ' ')
                 << "| " << risk              << string(6 - (int)risk.length(), ' ')
                 << "| " << st << string(max(0,13-(int)st.length()), ' ') << "|" << endl;
        }
        else
        {
            cout << "[EMPTY]    | ---  | ---   | ---   | ---   | ---          |" << endl;
        }
    }
    cout << "  +================================================================+" << endl;
}

// DISPLAY H2: COLLISION CHAIN TABLE
void hash_layer::display_collision_table()
{
    cout << "\r\n  +============================================+" << endl;
    cout << "  |      H2: COLLISION HANDLING TABLE          |" << endl;
    cout << "  +============================================+" << endl;
    bool any_collision = false;

    for (int i = 0; i < H1_SIZE; i++)
    {
        if (H2[i] != NULL)
        {
            any_collision = true;
            cout << "  | Slot " << i << " Chain:                              |" << endl;
            collision_node *curr = H2[i];
            while (curr != NULL)
            {
                string st = curr->data.status;
                cout << "  |   Zone_" << curr->data.zone_id
                     << "  T=" << curr->data.temperature
                     << "  S=" << curr->data.smoke_lvl
                     << "  [" << st << "]"
                     << string(max(0, 10 - (int)st.length()), ' ') << "|" << endl;
                if (curr->next) cout << "  |              v                             |" << endl;
                curr = curr->next;
            }
            cout << "  +--------------------------------------------+" << endl;
        }
    }

    if (!any_collision)
    {
        cout << "  |  No collisions in current table.           |" << endl;
        cout << "  +============================================+" << endl;
    }
}

// DISPLAY H3: FAST RETRIEVAL CACHE
void hash_layer::display_cache()
{
    cout << "\r\n  +============================================+" << endl;
    cout << "  |       H3: FAST RETRIEVAL CACHE             |" << endl;
    cout << "  |       (Last " << H3_SIZE << " Accessed Zones)             |" << endl;
    cout << "  +============================================+" << endl;

    bool any = false;
    for (int i = 0; i < H3_SIZE; i++)
    {
        if (H3[i].valid)
        {
            any = true;
            cout << "  | [" << i << "] Zone_" << H3[i].zone_id
                 << "  T=" << H3[i].temperature
                 << "  H=" << H3[i].humidity
                 << "  S=" << H3[i].smoke_lvl;
            int used = 8 + (int)to_string(H3[i].zone_id).length()
                         + (int)to_string(H3[i].temperature).length()
                         + (int)to_string(H3[i].humidity).length()
                         + (int)to_string(H3[i].smoke_lvl).length();
            cout << string(max(0, 44 - used), ' ') << "|" << endl;
        }
    }
    if (!any)
    {
        cout << "  |  Cache is empty.                           |" << endl;
    }
    cout << "  +============================================+" << endl;
}

// UPDATE CACHE MANUALLY (Menu option)
void hash_layer::update_cache(int zone_id, int temp, int hum, int smoke)
{
    cache_insert(zone_id, temp, hum, smoke);
    cout << "  Cache updated with Zone_" << zone_id << " data." << endl;
}

// DISPLAY ALL THREE STRUCTURES
void hash_layer::display_all()
{
    display_H1();
    display_collision_table();
    display_cache();
}

// LOAD FROM SENSOR DATA
// Integrates with array layer - same 9 zone readings inserted
// Time Complexity: O(n)
void hash_layer::load_from_sensor_data(int temps[], int humids[], int smokes[], int count)
{
    cout << "\n===== LOADING ZONE DATA INTO HASH TABLE =====" << endl;
    for (int i = 0; i < count; i++)
        insert(i, temps[i], humids[i], smokes[i]);

    cout << "[Hash] " << count << " zones loaded." << endl;
}