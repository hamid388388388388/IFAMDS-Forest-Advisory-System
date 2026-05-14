#include "tree_layer.h"

// CONSTRUCTOR

tree_layer::tree_layer()
{
    T1_zone_hierarchy = NULL;
    T2_subzone_decomposition = NULL;
    T3_terrain_classification = NULL;
    T4_water_resource = NULL;
    T5_fire_control = NULL;
    T6_equipment_allocation = NULL;
    T7_fire_classification = NULL;
    T8_wildlife_activity = NULL;
    T9_human_activity = NULL;
    T10_local_decision = NULL;
    T11_regional_escalation = NULL;
    T12_global_emergency = NULL;
}

// HELPER: ADD CHILD
// Time Complexity: O(1)

void tree_layer::add_child(tree_node *parent, tree_node *child)
{
    if (parent->child_count < 4)
    {
        parent->children[parent->child_count] = child;
        parent->child_count++;
    }
}

// HELPER: DISPLAY TREE
// Recursive pre-order traversal with indentation to show tree shape
// Time Complexity: O(n) where n = total nodes in tree

void tree_layer::display_tree(tree_node *root, int level)
{
    if (root == NULL)
        return;

    string indent = "";
    for (int i = 0; i < level; i++)
        indent += "    ";

    // Build data string
    string data = "";
    if (root->temperature != 0)
        data += " T=" + to_string(root->temperature) + " H=" + to_string(root->humidity) + " S=" + to_string(root->smoke_lvl);
    if (root->risk_score != 0)
    {
        string rs = to_string(root->risk_score);
        data += " Risk=" + rs.substr(0, min((int)rs.length(), 5));
    }
    if (root->fire_score != 0)
    {
        string fs = to_string(root->fire_score);
        data += " Fire=" + fs.substr(0, min((int)fs.length(), 5));
    }
    if (root->fire_spread_rate != 0)
    {
        string sr = to_string(root->fire_spread_rate);
        data += " Spread=" + sr.substr(0, min((int)sr.length(), 4));
    }

    string label = "[" + root->zone_name + "]" + data;
    cout << indent << "+-- " << label << endl;

    for (int i = 0; i < root->child_count; i++)
        display_tree(root->children[i], level + 1);

    if (root->left != NULL)
        display_tree(root->left, level + 1);
    if (root->right != NULL)
        display_tree(root->right, level + 1);
}

// DECISION SCORE FORMULA
// From project spec:
// Score = w1*(Fire) + w2*(Smoke) + w3*(Temperature)
// w1=0.4  w2=0.3  w3=0.3
// fire_factor        = temperature / 100.0
// smoke_factor       = smoke_lvl   / 100.0
// humidity_penalty   = (100 - humidity) / 100.0  (low humidity = high risk)
// Time Complexity: O(1)

float tree_layer::compute_decision_score(int temperature, int smoke_lvl, int humidity)
{
    float fire_factor = temperature / 100.0;
    float smoke_factor = smoke_lvl / 100.0;
    float humidity_penalty = (100 - humidity) / 100.0;

    float score = 0.4 * fire_factor + 0.3 * smoke_factor + 0.3 * humidity_penalty;
    return score;
}

// T1: ZONE HIERARCHY TREE
// Structure: Forest_Root -> Zone A/B/C -> Zone A1/A2 -> Zone A1-1/A1-2
// Purpose: Divides forest into structured monitoring zones
// Time Complexity to build: O(1) fixed structure

void tree_layer::build_T1_zone_hierarchy()
{
    // root = entire forest
    T1_zone_hierarchy = new tree_node("Forest_Root");
    T1_zone_hierarchy->temperature = 30;
    T1_zone_hierarchy->humidity = 50;
    T1_zone_hierarchy->smoke_lvl = 10;
    T1_zone_hierarchy->risk_score = compute_decision_score(30, 10, 50);

    //---------- Level 1: Main Zones ----------
    tree_node *zoneA = new tree_node("Zone_A");
    zoneA->temperature = 50;
    zoneA->humidity = 20;
    zoneA->smoke_lvl = 80;
    zoneA->risk_score = compute_decision_score(50, 80, 20);

    tree_node *zoneB = new tree_node("Zone_B");
    zoneB->temperature = 30;
    zoneB->humidity = 60;
    zoneB->smoke_lvl = 15;
    zoneB->risk_score = compute_decision_score(30, 15, 60);

    tree_node *zoneC = new tree_node("Zone_C");
    zoneC->temperature = 55;
    zoneC->humidity = 15;
    zoneC->smoke_lvl = 85;
    zoneC->risk_score = compute_decision_score(55, 85, 15);

    //---------- Level 2: Sub-Zones of Zone A ----------
    tree_node *zA1 = new tree_node("Zone_A1");
    zA1->temperature = 52;
    zA1->humidity = 18;
    zA1->smoke_lvl = 82;
    zA1->risk_score = compute_decision_score(52, 82, 18);

    tree_node *zA2 = new tree_node("Zone_A2");
    zA2->temperature = 48;
    zA2->humidity = 22;
    zA2->smoke_lvl = 75;
    zA2->risk_score = compute_decision_score(48, 75, 22);

    //---------- Level 3: Sub-Sub-Zones of Zone A1 ----------
    tree_node *zA1_1 = new tree_node("Zone_A1-1");
    zA1_1->temperature = 53;
    zA1_1->humidity = 17;
    zA1_1->smoke_lvl = 83;
    zA1_1->risk_score = compute_decision_score(53, 83, 17);

    tree_node *zA1_2 = new tree_node("Zone_A1-2");
    zA1_2->temperature = 51;
    zA1_2->humidity = 19;
    zA1_2->smoke_lvl = 81;
    zA1_2->risk_score = compute_decision_score(51, 81, 19);

    //---------- Level 2: Sub-Zones of Zone B ----------
    tree_node *zB1 = new tree_node("Zone_B1");
    zB1->temperature = 28;
    zB1->humidity = 65;
    zB1->smoke_lvl = 12;
    zB1->risk_score = compute_decision_score(28, 12, 65);

    tree_node *zB2 = new tree_node("Zone_B2");
    zB2->temperature = 32;
    zB2->humidity = 55;
    zB2->smoke_lvl = 18;
    zB2->risk_score = compute_decision_score(32, 18, 55);

    //---------- Level 2: Sub-Zones of Zone C ----------
    tree_node *zC1 = new tree_node("Zone_C1");
    zC1->temperature = 57;
    zC1->humidity = 12;
    zC1->smoke_lvl = 88;
    zC1->risk_score = compute_decision_score(57, 88, 12);

    tree_node *zC2 = new tree_node("Zone_C2");
    zC2->temperature = 53;
    zC2->humidity = 18;
    zC2->smoke_lvl = 82;
    zC2->risk_score = compute_decision_score(53, 82, 18);

    // linking
    add_child(zA1, zA1_1);
    add_child(zA1, zA1_2);
    add_child(zoneA, zA1);
    add_child(zoneA, zA2);
    add_child(zoneB, zB1);
    add_child(zoneB, zB2);
    add_child(zoneC, zC1);
    add_child(zoneC, zC2);
    add_child(T1_zone_hierarchy, zoneA);
    add_child(T1_zone_hierarchy, zoneB);
    add_child(T1_zone_hierarchy, zoneC);

    cout << "T1 Zone Hierarchy Tree built." << endl;
}

// T2: SUB-ZONE DECOMPOSITION TREE
// Structure: Zone_3 -> North / South / East / West -> Inner/Outer
// Purpose: Splits zones into directional monitoring units
// Time Complexity to build: O(1)

void tree_layer::build_T2_subzone_decomposition()
{
    T2_subzone_decomposition = new tree_node("Zone_3");
    T2_subzone_decomposition->temperature = 45;
    T2_subzone_decomposition->humidity = 25;
    T2_subzone_decomposition->smoke_lvl = 70;
    T2_subzone_decomposition->risk_score = compute_decision_score(45, 70, 25);

    //---------- 4 directional sub-zones ----------
    tree_node *north = new tree_node("Zone_3_North");
    north->temperature = 47;
    north->humidity = 22;
    north->smoke_lvl = 72;
    north->risk_score = compute_decision_score(47, 72, 22);

    tree_node *south = new tree_node("Zone_3_South");
    south->temperature = 43;
    south->humidity = 28;
    south->smoke_lvl = 68;
    south->risk_score = compute_decision_score(43, 68, 28);

    tree_node *east = new tree_node("Zone_3_East");
    east->temperature = 50;
    east->humidity = 20;
    east->smoke_lvl = 75;
    east->risk_score = compute_decision_score(50, 75, 20);

    tree_node *west = new tree_node("Zone_3_West");
    west->temperature = 44;
    west->humidity = 26;
    west->smoke_lvl = 69;
    west->risk_score = compute_decision_score(44, 69, 26);

    //---------- North split into inner/outer ----------
    tree_node *north_inner = new tree_node("Zone_3_North_Inner");
    north_inner->temperature = 49;
    north_inner->humidity = 20;
    north_inner->smoke_lvl = 78;
    north_inner->risk_score = compute_decision_score(49, 78, 20);

    tree_node *north_outer = new tree_node("Zone_3_North_Outer");
    north_outer->temperature = 45;
    north_outer->humidity = 24;
    north_outer->smoke_lvl = 70;
    north_outer->risk_score = compute_decision_score(45, 70, 24);

    add_child(north, north_inner);
    add_child(north, north_outer);
    add_child(T2_subzone_decomposition, north);
    add_child(T2_subzone_decomposition, south);
    add_child(T2_subzone_decomposition, east);
    add_child(T2_subzone_decomposition, west);

    cout << "T2 Sub-Zone Decomposition Tree built." << endl;
}

// T3: TERRAIN CLASSIFICATION TREE
// Structure: Terrain_Root -> High / Medium / Low Risk
// Formula: terrain_risk = (slope + dryness + vegetation_density) / 3
// Purpose: Classifies land physical danger level
// Time Complexity to build: O(1)

void tree_layer::build_T3_terrain_classification()
{
    T3_terrain_classification = new tree_node("Terrain_Root");

    //---------- High Risk Terrain ----------
    tree_node *high = new tree_node("High_Risk_Terrain");
    high->slope = 0.8;
    high->dryness = 0.9;
    high->vegetation_density = 0.7;
    high->terrain_risk = (high->slope + high->dryness + high->vegetation_density) / 3.0;
    high->temperature = 55;
    high->humidity = 10;
    high->smoke_lvl = 85;

    // sub-types of high risk
    tree_node *steep_slope = new tree_node("Steep_Slope_Zone");
    steep_slope->slope = 0.9;
    steep_slope->dryness = 0.8;
    steep_slope->vegetation_density = 0.8;
    steep_slope->terrain_risk = (0.9 + 0.8 + 0.8) / 3.0;

    tree_node *extreme_dry = new tree_node("Extreme_Dry_Zone");
    extreme_dry->slope = 0.7;
    extreme_dry->dryness = 1.0;
    extreme_dry->vegetation_density = 0.9;
    extreme_dry->terrain_risk = (0.7 + 1.0 + 0.9) / 3.0;

    //---------- Medium Risk Terrain ----------
    tree_node *medium = new tree_node("Medium_Risk_Terrain");
    medium->slope = 0.5;
    medium->dryness = 0.5;
    medium->vegetation_density = 0.5;
    medium->terrain_risk = (0.5 + 0.5 + 0.5) / 3.0;
    medium->temperature = 35;
    medium->humidity = 40;
    medium->smoke_lvl = 45;

    //---------- Low Risk Terrain ----------
    tree_node *low = new tree_node("Low_Risk_Terrain");
    low->slope = 0.2;
    low->dryness = 0.1;
    low->vegetation_density = 0.3;
    low->terrain_risk = (0.2 + 0.1 + 0.3) / 3.0;
    low->temperature = 25;
    low->humidity = 70;
    low->smoke_lvl = 10;

    add_child(high, steep_slope);
    add_child(high, extreme_dry);
    add_child(T3_terrain_classification, high);
    add_child(T3_terrain_classification, medium);
    add_child(T3_terrain_classification, low);

    cout << "T3 Terrain Classification Tree built." << endl;
}

// T4: WATER RESOURCE TREE
// Structure: Water_HQ -> Zone Tanks -> Sub-Sources
// Formula: water_availability = available_water / required_water
// Purpose: Tracks water availability across all zones
// Time Complexity to build: O(1)

void tree_layer::build_T4_water_resource()
{
    T4_water_resource = new tree_node("Water_HQ");
    T4_water_resource->available_water = 300.0;
    T4_water_resource->required_water = 300.0;
    T4_water_resource->water_availability = 1.0; // fully available

    //---------- Zone A Tank ----------
    tree_node *tankA = new tree_node("Tank_Zone_A");
    tankA->available_water = 80.0;
    tankA->required_water = 100.0;
    tankA->water_availability = 80.0 / 100.0; // 0.80 sufficient

    tree_node *riverA = new tree_node("River_Source_A");
    riverA->available_water = 50.0;
    riverA->required_water = 100.0;
    riverA->water_availability = 0.5;

    tree_node *reserveA = new tree_node("Reserve_Tank_A");
    reserveA->available_water = 30.0;
    reserveA->required_water = 100.0;
    reserveA->water_availability = 0.3;

    //---------- Zone B Tank ----------
    tree_node *tankB = new tree_node("Tank_Zone_B");
    tankB->available_water = 60.0;
    tankB->required_water = 100.0;
    tankB->water_availability = 60.0 / 100.0; // 0.60 limited

    //---------- Zone C Tank ----------
    tree_node *tankC = new tree_node("Tank_Zone_C");
    tankC->available_water = 40.0;
    tankC->required_water = 100.0;
    tankC->water_availability = 40.0 / 100.0; // 0.40 critical

    tree_node *emergencyC = new tree_node("Emergency_Supply_C");
    emergencyC->available_water = 20.0;
    emergencyC->required_water = 100.0;
    emergencyC->water_availability = 0.2;

    add_child(tankA, riverA);
    add_child(tankA, reserveA);
    add_child(tankC, emergencyC);
    add_child(T4_water_resource, tankA);
    add_child(T4_water_resource, tankB);
    add_child(T4_water_resource, tankC);

    cout << "T4 Water Resource Tree built." << endl;
}

// T5: FIRE CONTROL RESOURCE TREE
// Structure: FireControl_HQ -> Teams -> Tools
// readiness_level: how ready is the team/tool (0 to 1)
// Purpose: Tracks fire response teams and equipment per zone
// Time Complexity to build: O(1)

void tree_layer::build_T5_fire_control()
{
    T5_fire_control = new tree_node("FireControl_HQ");
    T5_fire_control->readiness_level = 1.0;

    //---------- Zone A Team - fully equipped ----------
    tree_node *teamA = new tree_node("FireTeam_Zone_A");
    teamA->readiness_level = 0.9;

    tree_node *truckA = new tree_node("FireTruck_A1");
    truckA->readiness_level = 0.9;

    tree_node *droneA = new tree_node("Drone_Unit_A2");
    droneA->readiness_level = 0.8;

    //---------- Zone B Team - partially equipped ----------
    tree_node *teamB = new tree_node("FireTeam_Zone_B");
    teamB->readiness_level = 0.6;

    tree_node *truckB = new tree_node("FireTruck_B1");
    truckB->readiness_level = 0.6;

    //---------- Zone C Team - low resources ----------
    tree_node *teamC = new tree_node("FireTeam_Zone_C");
    teamC->readiness_level = 0.3;

    tree_node *pumpC = new tree_node("Portable_Pump_C1");
    pumpC->readiness_level = 0.3;

    add_child(teamA, truckA);
    add_child(teamA, droneA);
    add_child(teamB, truckB);
    add_child(teamC, pumpC);
    add_child(T5_fire_control, teamA);
    add_child(T5_fire_control, teamB);
    add_child(T5_fire_control, teamC);

    cout << "T5 Fire Control Resource Tree built." << endl;
}

// T6: EQUIPMENT ALLOCATION TREE
// Structure: Equipment_Pool -> Zone Allocations -> Equipment Items
// Formula: priority_score = risk_score * impact_factor
// Purpose: Assigns resources to zones based on priority
// Time Complexity to build: O(1)

void tree_layer::build_T6_equipment_allocation()
{
    T6_equipment_allocation = new tree_node("Equipment_Pool");

    //---------- Zone C - highest priority ----------
    tree_node *allocC = new tree_node("Allocate_Zone_C");
    allocC->temperature = 55;
    allocC->humidity = 15;
    allocC->smoke_lvl = 85;
    allocC->risk_score = compute_decision_score(55, 85, 15);
    allocC->impact_factor = 0.9;
    allocC->priority_score = allocC->risk_score * allocC->impact_factor;

    tree_node *retardantC = new tree_node("Fire_Retardant_C");
    retardantC->priority_score = allocC->priority_score;

    //---------- Zone A - high priority ----------
    tree_node *allocA = new tree_node("Allocate_Zone_A");
    allocA->temperature = 50;
    allocA->humidity = 20;
    allocA->smoke_lvl = 80;
    allocA->risk_score = compute_decision_score(50, 80, 20);
    allocA->impact_factor = 0.8;
    allocA->priority_score = allocA->risk_score * allocA->impact_factor;

    tree_node *pumpA = new tree_node("Pump_Unit_A");
    pumpA->priority_score = allocA->priority_score;

    tree_node *hoseA = new tree_node("Hose_System_A");
    hoseA->priority_score = allocA->priority_score;

    //---------- Zone B - low priority ----------
    tree_node *allocB = new tree_node("Allocate_Zone_B");
    allocB->temperature = 30;
    allocB->humidity = 60;
    allocB->smoke_lvl = 15;
    allocB->risk_score = compute_decision_score(30, 15, 60);
    allocB->impact_factor = 0.4;
    allocB->priority_score = allocB->risk_score * allocB->impact_factor;

    add_child(allocC, retardantC);
    add_child(allocA, pumpA);
    add_child(allocA, hoseA);
    add_child(T6_equipment_allocation, allocC); // highest priority first
    add_child(T6_equipment_allocation, allocA);
    add_child(T6_equipment_allocation, allocB);

    cout << "T6 Equipment Allocation Tree built." << endl;
}

// T7: FIRE CLASSIFICATION TREE
// Structure: Fire_Root -> Major / Moderate / Low / No_Fire
// Formula: fire_score = alpha*(temperature/100) + beta*(smoke_lvl/100)
//          alpha = 0.5, beta = 0.5
// Purpose: Classify fire intensity level
// Time Complexity to build: O(1)

void tree_layer::build_T7_fire_classification()
{
    float alpha = 0.5;
    float beta = 0.5;

    T7_fire_classification = new tree_node("Fire_Classification_Root");

    //---------- Major Fire ----------
    tree_node *major = new tree_node("Major_Fire");
    major->temperature = 70;
    major->humidity = 10;
    major->smoke_lvl = 95;
    major->fire_score = alpha * (70.0 / 100) + beta * (95.0 / 100); // 0.825

    tree_node *explosive = new tree_node("Explosive_Spread");
    explosive->fire_score = 0.95;

    tree_node *major_contained = new tree_node("Major_Contained");
    major_contained->fire_score = 0.75;

    //---------- Moderate Fire ----------
    tree_node *moderate = new tree_node("Moderate_Fire");
    moderate->temperature = 50;
    moderate->humidity = 25;
    moderate->smoke_lvl = 70;
    moderate->fire_score = alpha * (50.0 / 100) + beta * (70.0 / 100); // 0.60

    //---------- Low Fire ----------
    tree_node *low = new tree_node("Low_Fire");
    low->temperature = 30;
    low->humidity = 60;
    low->smoke_lvl = 30;
    low->fire_score = alpha * (30.0 / 100) + beta * (30.0 / 100); // 0.30

    //---------- No Fire ----------
    tree_node *no_fire = new tree_node("No_Fire");
    no_fire->temperature = 25;
    no_fire->humidity = 70;
    no_fire->smoke_lvl = 5;
    no_fire->fire_score = alpha * (25.0 / 100) + beta * (5.0 / 100); // 0.15

    add_child(major, explosive);
    add_child(major, major_contained);
    add_child(T7_fire_classification, major);
    add_child(T7_fire_classification, moderate);
    add_child(T7_fire_classification, low);
    add_child(T7_fire_classification, no_fire);

    cout << "T7 Fire Classification Tree built." << endl;
}

// T8: WILDLIFE ACTIVITY TREE
// Structure: Wildlife_Monitor -> Normal / Unusual / Mass_Flee
// movement_score: intensity of wildlife movement (0 to 1)
// Mass flee pattern = fire nearby indicator
// Time Complexity to build: O(1)

void tree_layer::build_T8_wildlife_activity()
{
    T8_wildlife_activity = new tree_node("Wildlife_Monitor");

    //---------- Normal Movement ----------
    tree_node *normal = new tree_node("Normal_Movement");
    normal->movement_score = 0.1;

    //---------- Unusual Movement ----------
    tree_node *unusual = new tree_node("Unusual_Movement");
    unusual->movement_score = 0.5;

    tree_node *grouping = new tree_node("Group_Clustering");
    grouping->movement_score = 0.4;

    tree_node *migration = new tree_node("Unusual_Migration");
    migration->movement_score = 0.6;

    //---------- Mass Flee (fire nearby) ----------
    tree_node *flee = new tree_node("Mass_Flee_Pattern");
    flee->movement_score = 0.9;

    tree_node *flee_north = new tree_node("Flee_Direction_North");
    flee_north->movement_score = 0.9;

    tree_node *flee_south = new tree_node("Flee_Direction_South");
    flee_south->movement_score = 0.85;

    add_child(unusual, grouping);
    add_child(unusual, migration);
    add_child(flee, flee_north);
    add_child(flee, flee_south);
    add_child(T8_wildlife_activity, normal);
    add_child(T8_wildlife_activity, unusual);
    add_child(T8_wildlife_activity, flee);

    cout << "T8 Wildlife Activity Tree built." << endl;
}

// T9: HUMAN ACTIVITY TREE
// Structure: Human_Monitor -> Authorized / Unauthorized / Breach
// Formula: human_risk = movement_factor * restricted_area_factor
// Purpose: Detects and classifies human presence in forest
// Time Complexity to build: O(1)

void tree_layer::build_T9_human_activity()
{
    T9_human_activity = new tree_node("Human_Monitor");

    //---------- Authorized Entry ----------
    tree_node *authorized = new tree_node("Authorized_Entry");
    authorized->movement_factor = 0.1;
    authorized->restricted_area_factor = 0.1;
    authorized->human_risk = authorized->movement_factor * authorized->restricted_area_factor;

    //---------- Unauthorized Entry ----------
    tree_node *unauthorized = new tree_node("Unauthorized_Entry");
    unauthorized->movement_factor = 0.8;
    unauthorized->restricted_area_factor = 0.5;
    unauthorized->human_risk = unauthorized->movement_factor * unauthorized->restricted_area_factor;

    tree_node *tourist = new tree_node("Lost_Tourist");
    tourist->movement_factor = 0.3;
    tourist->restricted_area_factor = 0.3;
    tourist->human_risk = tourist->movement_factor * tourist->restricted_area_factor;

    tree_node *poacher = new tree_node("Poacher");
    poacher->movement_factor = 0.8;
    poacher->restricted_area_factor = 0.8;
    poacher->human_risk = poacher->movement_factor * poacher->restricted_area_factor;

    //---------- Restricted Zone Breach ----------
    tree_node *breach = new tree_node("Restricted_Zone_Breach");
    breach->movement_factor = 0.9;
    breach->restricted_area_factor = 1.0;
    breach->human_risk = breach->movement_factor * breach->restricted_area_factor;

    tree_node *arson = new tree_node("Arson_Suspect");
    arson->movement_factor = 0.95;
    arson->restricted_area_factor = 1.0;
    arson->human_risk = arson->movement_factor * arson->restricted_area_factor;

    tree_node *intrusion = new tree_node("Zone_Intrusion");
    intrusion->movement_factor = 0.8;
    intrusion->restricted_area_factor = 1.0;
    intrusion->human_risk = intrusion->movement_factor * intrusion->restricted_area_factor;

    add_child(unauthorized, tourist);
    add_child(unauthorized, poacher);
    add_child(breach, arson);
    add_child(breach, intrusion);
    add_child(T9_human_activity, authorized);
    add_child(T9_human_activity, unauthorized);
    add_child(T9_human_activity, breach);

    cout << "T9 Human Activity Tree built." << endl;
}

// T10: LOCAL DECISION TREE (Binary)
// Rule: If risk_score > 0.6 => Activate_Local_Response (left)
//       Else                 => Continue_Normal_Monitoring (right)
// Time Complexity to build: O(1), decision: O(1)

void tree_layer::build_T10_local_decision()
{
    T10_local_decision = new tree_node("Zone_3_Local_Check");
    T10_local_decision->temperature = 50;
    T10_local_decision->humidity = 20;
    T10_local_decision->smoke_lvl = 80;
    T10_local_decision->risk_score = compute_decision_score(50, 80, 20);

    // YES branch - risk is high
    T10_local_decision->left = new tree_node("ACTIVATE_Local_Response");

    // NO branch - risk is low
    T10_local_decision->right = new tree_node("Continue_Normal_Monitoring");

    cout << "T10 Local Decision Tree built." << endl;
}

// T11: REGIONAL ESCALATION TREE (Binary)
// Rule: If fire_spread_rate > 0.5 => Escalate (left)
//       Else                        => Keep_Local (right)
// Time Complexity to build: O(1), decision: O(1)

void tree_layer::build_T11_regional_escalation()
{
    T11_regional_escalation = new tree_node("Regional_Fire_Spread_Check");
    T11_regional_escalation->temperature = 50;
    T11_regional_escalation->humidity = 20;
    T11_regional_escalation->smoke_lvl = 80;
    T11_regional_escalation->risk_score = compute_decision_score(50, 80, 20);
    T11_regional_escalation->fire_spread_rate = 0.72; // derived from risk

    // YES branch
    T11_regional_escalation->left = new tree_node("ESCALATE_To_Regional_Response");

    // NO branch
    T11_regional_escalation->right = new tree_node("Keep_Local_Response_Only");

    // affected zones shown as children
    tree_node *zone3 = new tree_node("Zone_3_Origin");
    zone3->temperature = 50;
    zone3->humidity = 20;
    zone3->smoke_lvl = 80;
    zone3->risk_score = compute_decision_score(50, 80, 20);
    zone3->fire_spread_rate = 0.80;

    tree_node *zone4 = new tree_node("Zone_4_Spreading");
    zone4->temperature = 47;
    zone4->humidity = 22;
    zone4->smoke_lvl = 75;
    zone4->risk_score = compute_decision_score(47, 75, 22);
    zone4->fire_spread_rate = 0.65;

    tree_node *zone6 = new tree_node("Zone_6_Monitoring");
    zone6->temperature = 35;
    zone6->humidity = 40;
    zone6->smoke_lvl = 45;
    zone6->risk_score = compute_decision_score(35, 45, 40);
    zone6->fire_spread_rate = 0.40;

    add_child(T11_regional_escalation, zone3);
    add_child(T11_regional_escalation, zone4);
    add_child(T11_regional_escalation, zone6);

    cout << "T11 Regional Escalation Tree built." << endl;
}

// T12: GLOBAL EMERGENCY TREE (Binary)
// Rule: If total_risk_score > 0.7 => Global_Alert (left)
//       Else                        => System_On_Standby (right)
// total_risk = average risk of all affected zones
// Time Complexity to build: O(1), decision: O(1)

void tree_layer::build_T12_global_emergency()
{
    float zone3_risk = compute_decision_score(50, 80, 20);
    float zone4_risk = compute_decision_score(47, 75, 22);
    float zone6_risk = compute_decision_score(35, 45, 40);
    float total_risk = (zone3_risk + zone4_risk + zone6_risk) / 3.0;

    T12_global_emergency = new tree_node("Global_System_Check");
    T12_global_emergency->temperature = 55;
    T12_global_emergency->humidity = 15;
    T12_global_emergency->smoke_lvl = 85;
    T12_global_emergency->risk_score = total_risk;

    // YES branch
    T12_global_emergency->left = new tree_node("GLOBAL_ALERT_ACTIVATED");

    // NO branch
    T12_global_emergency->right = new tree_node("System_On_Standby");

    // all affected zones as children
    tree_node *z3 = new tree_node("Zone_3");
    z3->risk_score = zone3_risk;
    z3->temperature = 50;
    z3->humidity = 20;
    z3->smoke_lvl = 80;

    tree_node *z4 = new tree_node("Zone_4");
    z4->risk_score = zone4_risk;
    z4->temperature = 47;
    z4->humidity = 22;
    z4->smoke_lvl = 75;

    tree_node *z6 = new tree_node("Zone_6");
    z6->risk_score = zone6_risk;
    z6->temperature = 35;
    z6->humidity = 40;
    z6->smoke_lvl = 45;

    add_child(T12_global_emergency, z3);
    add_child(T12_global_emergency, z4);
    add_child(T12_global_emergency, z6);

    cout << "T12 Global Emergency Tree built." << endl;
}

// DISPLAY WRAPPERS

void tree_layer::show_T1()
{
    cout << "\n--- T1: Zone Hierarchy Tree ---" << endl;
    display_tree(T1_zone_hierarchy, 0);
}
void tree_layer::show_T2()
{
    cout << "\n--- T2: Sub-Zone Decomposition Tree ---" << endl;
    display_tree(T2_subzone_decomposition, 0);
}
void tree_layer::show_T3()
{
    cout << "\n--- T3: Terrain Classification Tree ---" << endl;
    display_tree(T3_terrain_classification, 0);
}
void tree_layer::show_T4()
{
    cout << "\n--- T4: Water Resource Tree ---" << endl;
    display_tree(T4_water_resource, 0);
}
void tree_layer::show_T5()
{
    cout << "\n--- T5: Fire Control Resource Tree ---" << endl;
    display_tree(T5_fire_control, 0);
}
void tree_layer::show_T6()
{
    cout << "\n--- T6: Equipment Allocation Tree ---" << endl;
    display_tree(T6_equipment_allocation, 0);
}
void tree_layer::show_T7()
{
    cout << "\n--- T7: Fire Classification Tree ---" << endl;
    display_tree(T7_fire_classification, 0);
}
void tree_layer::show_T8()
{
    cout << "\n--- T8: Wildlife Activity Tree ---" << endl;
    display_tree(T8_wildlife_activity, 0);
}
void tree_layer::show_T9()
{
    cout << "\n--- T9: Human Activity Tree ---" << endl;
    display_tree(T9_human_activity, 0);
}
void tree_layer::show_T10()
{
    cout << "\n--- T10: Local Decision Tree ---" << endl;
    display_tree(T10_local_decision, 0);
}
void tree_layer::show_T11()
{
    cout << "\n--- T11: Regional Escalation Tree ---" << endl;
    display_tree(T11_regional_escalation, 0);
}
void tree_layer::show_T12()
{
    cout << "\n--- T12: Global Emergency Tree ---" << endl;
    display_tree(T12_global_emergency, 0);
}

// DECISION FUNCTIONS

// Manual risk score from user input
// Time Complexity: O(1)

void tree_layer::compute_risk_score_manual(int temperature, int humidity, int smoke_lvl)
{
    float score = compute_decision_score(temperature, smoke_lvl, humidity);

    cout << "Score = 0.4*(temp/100) + 0.3*(smoke/100) + 0.3*((100-humidity)/100)" << endl;
    cout << "Score = " << score << endl;

    if (score > 0.6)
        cout << ">> Score > 0.6 => EMERGENCY ACTIVATED" << endl;
    else if (score > 0.3)
        cout << ">> Score 0.3 to 0.6 => INCREASED MONITORING" << endl;
    else
        cout << ">> Score < 0.3 => NORMAL CONDITIONS" << endl;
}

// T10: Zone Level Decision
// Rule: risk_score > 0.6 => Activate Local Response
// Time Complexity: O(1)

void tree_layer::zone_level_decision()
{
    if (T10_local_decision == NULL)
    {
        cout << "Build T10 first." << endl;
        return;
    }

    float risk_score = T10_local_decision->risk_score;
    cout << "Zone:       " << T10_local_decision->zone_name << endl;
    cout << "RiskScore:  " << risk_score << endl;
    cout << "Threshold:  0.6" << endl;

    if (risk_score > 0.6)
    {
        cout << "RiskScore > 0.6 => " << T10_local_decision->left->zone_name << endl;
        cout << ">> Local emergency teams deployed to zone." << endl;
    }
    else
    {
        cout << "RiskScore <= 0.6 => " << T10_local_decision->right->zone_name << endl;
        cout << ">> Zone stable. Normal monitoring continues." << endl;
    }
}

// T11: Regional Escalation Decision
// Rule: fire_spread_rate > 0.5 => Escalate to region
// Time Complexity: O(1)

void tree_layer::regional_decision()
{
    if (T11_regional_escalation == NULL)
    {
        cout << "Build T11 first." << endl;
        return;
    }

    float fire_spread_rate = T11_regional_escalation->fire_spread_rate;
    cout << "FireSpreadRate: " << fire_spread_rate << endl;
    cout << "Threshold:      0.5" << endl;

    if (fire_spread_rate > 0.5)
    {
        cout << "SpreadRate > 0.5 => " << T11_regional_escalation->left->zone_name << endl;
        cout << ">> Alert spreading to Zone 4 and Zone 6." << endl;
    }
    else
    {
        cout << "SpreadRate <= 0.5 => " << T11_regional_escalation->right->zone_name << endl;
        cout << ">> Fire contained. No regional escalation." << endl;
    }
}

// T12: Global Emergency Decision
// Rule: total risk_score > 0.7 => Global Alert
// Time Complexity: O(1)

void tree_layer::global_emergency_decision()
{
    if (T12_global_emergency == NULL)
    {
        cout << "Build T12 first." << endl;
        return;
    }

    float total_risk_score = T12_global_emergency->risk_score;
    float threshold = 0.7;

    cout << "TotalRiskScore: " << total_risk_score << endl;
    cout << "Threshold:      " << threshold << endl;

    if (total_risk_score > threshold)
    {
        cout << "TotalRisk > 0.7 => " << T12_global_emergency->left->zone_name << endl;
        cout << ">> Full system emergency activated across all zones." << endl;
    }
    else
    {
        cout << "TotalRisk <= 0.7 => " << T12_global_emergency->right->zone_name << endl;
        cout << ">> No global emergency. System on standby." << endl;
    }
}

// Execute Final Action combining T10 + T11 + T12
// Time Complexity: O(1)

void tree_layer::execute_final_action()
{
    if (T10_local_decision == NULL || T11_regional_escalation == NULL || T12_global_emergency == NULL)
    {
        cout << "Build all decision trees (T10, T11, T12) first." << endl;
        return;
    }

    float local_risk_score = T10_local_decision->risk_score;
    float regional_risk_score = T11_regional_escalation->risk_score;
    float global_risk_score = T12_global_emergency->risk_score;
    float combined_score = (local_risk_score + regional_risk_score + global_risk_score) / 3.0;

    cout << "\n===== FINAL ACTION DECISION =====" << endl;
    cout << "T10 LocalRiskScore:    " << local_risk_score << endl;
    cout << "T11 RegionalRiskScore: " << regional_risk_score << endl;
    cout << "T12 GlobalRiskScore:   " << global_risk_score << endl;
    cout << "CombinedScore:         " << combined_score << endl;

    if (combined_score > 0.7)
        cout << ">> ACTION: FULL EMERGENCY - Deploy all resources immediately." << endl;
    else if (combined_score > 0.5)
        cout << ">> ACTION: REGIONAL ALERT - Pre-deploy teams to nearby zones." << endl;
    else if (combined_score > 0.3)
        cout << ">> ACTION: LOCAL RESPONSE - Send fire team to affected zone." << endl;
    else
        cout << ">> ACTION: NORMAL MONITORING - System stable." << endl;
}