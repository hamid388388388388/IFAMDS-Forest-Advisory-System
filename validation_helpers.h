#ifndef VALIDATION_HELPERS_H
#define VALIDATION_HELPERS_H

#include <iostream>
#include <string>
#include <limits>
#include <vector>
using namespace std;

// ============================================================
// PROFESSIONAL VALIDATION AND WARNING SYSTEM
// Senior Developer Grade Input Validation & Edge Cases
// ============================================================

// ============================================================
// VALIDATION FUNCTIONS WITH RANGE CHECKS
// ============================================================

int get_valid_int(string prompt, int min_val, int max_val) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  ❌ ERROR: Please enter a valid integer." << endl;
            continue;
        }
        
        if (value < min_val || value > max_val) {
            cout << "  ❌ ERROR: Value must be between " << min_val << " and " << max_val << "." << endl;
            continue;
        }
        
        return value;
    }
}

float get_valid_float(string prompt, float min_val = 0.0f, float max_val = 1.0f) {
    float value;
    while (true) {
        cout << prompt;
        cin >> value;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  ❌ ERROR: Please enter a valid number." << endl;
            continue;
        }
        
        if (value < min_val || value > max_val) {
            cout << "  ❌ ERROR: Value must be between " << min_val << " and " << max_val << "." << endl;
            continue;
        }
        
        return value;
    }
}

// ============================================================
// SENSOR DATA VALIDATION WITH WARNINGS
// ============================================================

struct SensorReading {
    int temperature;
    int humidity;
    int smoke;
    vector<string> warnings;
};

SensorReading validate_sensor_data(int temp, int humidity, int smoke, int reading_count = 0) {
    SensorReading sr;
    sr.temperature = temp;
    sr.humidity = humidity;
    sr.smoke = smoke;
    
    // ⚠️ CRITICAL WARNINGS
    if (temp == 0 || humidity == 0 || smoke == 0) {
        sr.warnings.push_back("⚠️  SENSOR MALFUNCTION: Zero reading detected");
    }
    
    if (temp == 100 || humidity == 100 || smoke == 100) {
        sr.warnings.push_back("⚠️  SENSOR SATURATION: Reading at maximum (100)");
    }
    
    // 🔥 FIRE RISK WARNINGS
    if (temp > 45) {
        sr.warnings.push_back("🔥 FIRE RISK: Temperature > 45°C");
    } else if (temp == 45) {
        sr.warnings.push_back("⚠️  BOUNDARY WARNING: Temperature at critical threshold (45°C)");
    }
    
    if (smoke > 70) {
        sr.warnings.push_back("🔥 FIRE POSSIBLE: Smoke level > 70%");
    } else if (smoke == 70 || smoke == 69 || smoke == 68) {
        sr.warnings.push_back("⚠️  BOUNDARY WARNING: Smoke level near critical (68-70%)");
    }
    
    if (humidity < 20) {
        sr.warnings.push_back("⚠️  DRY CONDITION: Humidity < 20% (fire spread risk)");
    } else if (humidity == 20 || humidity == 21) {
        sr.warnings.push_back("⚠️  BOUNDARY WARNING: Humidity at dry threshold (20-21%)");
    }
    
    // Impossible combinations
    if (temp > 45 && smoke < 10 && humidity > 80) {
        sr.warnings.push_back("⚠️  ANOMALY: High temp but low smoke + high humidity (sensor failure?)");
    }
    
    if (humidity == 100 && temp > 30) {
        sr.warnings.push_back("⚠️  FLOODING RISK: 100% humidity + high temperature");
    }
    
    // Array capacity warning
    if (reading_count >= 95) {
        sr.warnings.push_back("⚠️  CAPACITY WARNING: Array at " + to_string(reading_count) + "/100 (5 slots remaining)");
    } else if (reading_count >= 100) {
        sr.warnings.push_back("❌ CRITICAL: Array FULL - cannot add more readings!");
        sr.warnings.push_back("   ACTION: Clear old data or expand storage capacity.");
    }
    
    return sr;
}

void print_sensor_warnings(const SensorReading& sr) {
    if (sr.warnings.empty()) {
        cout << "  ✓ All readings within normal parameters." << endl;
    } else {
        for (const auto& warning : sr.warnings) {
            cout << "  " << warning << endl;
        }
    }
}

// ============================================================
// SPECIALIZED VALIDATION FUNCTIONS
// ============================================================

int get_valid_cycles(string prompt) {
    int cycles;
    while (true) {
        cout << prompt;
        cin >> cycles;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  ❌ ERROR: Please enter a valid integer." << endl;
            continue;
        }
        
        if (cycles <= 0) {
            cout << "  ❌ ERROR: Cycles must be > 0 (minimum 1 cycle)." << endl;
            continue;
        }
        
        if (cycles > 1000) {
            cout << "  ⚠️  WARNING: Very high cycle count (" << cycles << ") may cause long execution." << endl;
            char confirm;
            cout << "  Continue? (y/n): ";
            cin >> confirm;
            if (confirm == 'y' || confirm == 'Y') {
                return cycles;
            }
            continue;
        }
        
        return cycles;
    }
}

float get_valid_fire_level(string prompt) {
    float level;
    while (true) {
        cout << prompt;
        cin >> level;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  ❌ ERROR: Please enter a valid decimal number." << endl;
            continue;
        }
        
        if (level < 0.0f || level > 1.0f) {
            cout << "  ❌ ERROR: Fire level must be between 0.0 and 1.0." << endl;
            cout << "     (0.0 = no fire, 1.0 = maximum intensity)" << endl;
            continue;
        }
        
        if (level > 0.7f) {
            cout << "  🔥 CRITICAL: Fire level > 0.7 = severe emergency!" << endl;
        } else if (level > 0.5f) {
            cout << "  🔥 WARNING: Fire level > 0.5 = active fire detected!" << endl;
        }
        
        return level;
    }
}

int get_valid_submenu(int max_options, string prompt = "Enter choice: ") {
    return get_valid_int("  " + prompt, 1, max_options);
}

// ============================================================
// OUTPUT PAGINATION & SCROLLING
// ============================================================

void print_paginated_divider() {
    cout << "  ===== (Press ENTER to continue or 'q' to return) =====" << endl;
}

void wait_for_input() {
    string dummy;
    cin.ignore();
    getline(cin, dummy);
}

void display_with_pagination(vector<string>& lines, int lines_per_screen = 20) {
    if (lines.empty()) {
        cout << "  No output to display." << endl;
        return;
    }
    
    int total_lines = lines.size();
    int current_line = 0;
    
    while (current_line < total_lines) {
        int end_line = min(current_line + lines_per_screen, total_lines);
        
        for (int i = current_line; i < end_line; i++) {
            cout << lines[i] << endl;
        }
        
        current_line = end_line;
        
        if (current_line < total_lines) {
            cout << "\n  -- Displaying lines " << (current_line - lines_per_screen + 1) << "-" << current_line 
                 << " of " << total_lines << " --" << endl;
            print_paginated_divider();
            wait_for_input();
            cout << "\n";
        }
    }
}

// ============================================================
// BOUNDARY & EDGE CASE CHECKS
// ============================================================

bool check_array_capacity(int current_size, int max_size = 100) {
    if (current_size >= max_size) {
        cout << "  ❌ CRITICAL: Array capacity FULL (" << current_size << "/" << max_size << ")" << endl;
        return false;
    }
    
    if (current_size >= max_size * 0.9f) {
        cout << "  ⚠️  WARNING: Array at " << (int)(current_size * 100.0f / max_size) 
             << "% capacity (" << current_size << "/" << max_size << ")" << endl;
    }
    
    return true;
}

void print_header(string title) {
    cout << "\n  +--------------------------------------------------+" << endl;
    cout << "  | " << title << endl;
    cout << "  +--------------------------------------------------+" << endl;
}

void print_divider() {
    cout << "  --------------------------------------------------" << endl;
}

void print_step(string step) {
    cout << "\n  >> " << step << endl;
    cout << "  --------------------------------------------------" << endl;
}

void print_success(string msg) {
    cout << "  ✓ " << msg << endl;
}

void print_error(string msg) {
    cout << "  ❌ " << msg << endl;
}

void print_warning(string msg) {
    cout << "  ⚠️  " << msg << endl;
}

void print_info(string msg) {
    cout << "  ℹ️  " << msg << endl;
}

#endif // VALIDATION_HELPERS_H
