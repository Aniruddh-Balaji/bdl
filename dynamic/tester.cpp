#include <iostream>
#include <string>
#include <stack>
#include <unordered_map>
#include <functional>
#include "bdllibrary.hpp"

using namespace std;
using namespace bdl;

bool parse_int(const string& str, int& out_val) {
    if (str.empty()) return 0;
    int temp = 0;
    for (char c : str) {
        if (c < '0' || c > '9') return 0;
        temp = temp * 10 + (c - '0');
    }
    out_val = temp;
    return 1;
}

int main() {
    unordered_map<string, int> modinputs;
    modinputs["Not"] = 1; modinputs["And"] = 2; modinputs["Or"] = 2;
    modinputs["Nand"] = 2; modinputs["Nor"] = 2; modinputs["Xor"] = 2; modinputs["Xnor"] = 2;

    cout << "Modules loaded/available:\n";
    for (const auto& pair : modinputs) {
        cout << "  - " << pair.first << " (" << pair.second << " inputs)\n";
    }
    cout << "\n";

    string line;
    cout << "Enter expression: ";
    if (!getline(cin, line) || line.empty()) return 0;

    string expr[100]; 
    int expr_count = 0;
    string current_token = "";
    
    for (size_t i = 0; i <= line.length(); i++) {
        if (i < line.length() && line[i] != ' ') {
            current_token += line[i];
        } else {
            if (!current_token.empty() && expr_count < 100) {
                expr[expr_count++] = current_token;
                current_token = "";
            }
        }
    }

    int active_pins[32];
    int pin_count = 0;

    for (int i = 0; i < expr_count; i++) {
        int val;
        if (parse_int(expr[i], val)) {
            bool exists = 0;
            for (int j = 0; j < pin_count; j++) {
                if (active_pins[j] == val) { exists = 1; break; }
            }
            if (!exists && pin_count < 32) {
                active_pins[pin_count++] = val;
            }
        }
    }

    if (pin_count == 0) { 
        cout << "Error: No input pins found.\n"; 
        return 0; 
    }

    while (1) {
        bool pin_values[32] = {0};
        bool quit_flag = 0;
        
        cout << "\nEnter pin values (q to quit):\n";
        for (int i = 0; i < pin_count; i++) {
            string user_input;
            cout << "  Pin " << active_pins[i] << " (0/1): ";
            cin >> user_input;
            
            if (user_input == "q" || user_input == "exit") {
                quit_flag = 1;
                break;
            }
            pin_values[i] = (user_input == "1");
        }
        cin.ignore(); 
        
        if (quit_flag) break;

        stack<bool> st;
        bool error = 0;

        for (int k = 0; k < expr_count; k++) {
            string t = expr[k];
            int pin_idx;
            
            if (parse_int(t, pin_idx)) {
                int mapped_idx = -1;
                for (int i = 0; i < pin_count; i++) {
                    if (active_pins[i] == pin_idx) { mapped_idx = i; break; }
                }
                st.push(pin_values[mapped_idx]);
            } else if (modinputs.count(t)) {
                int count = modinputs[t];
                if (st.size() < (size_t)count) { 
                    cout << "Error: Bad layout for " << t << "\n"; 
                    error = 1; 
                    break; 
                }
                
                bool a = 0, b = 0;
                if (count == 1) {
                    a = st.top(); st.pop();
                } else if (count == 2) {
                    b = st.top(); st.pop();
                    a = st.top(); st.pop();
                }

                bool res = 0;
                if (t == "Not")  { Not g(a); res = g.get_Y(); }
                if (t == "And")  { And g(a, b); res = g.get_Y(); }
                if (t == "Or")   { Or g(a, b); res = g.get_Y(); }
                if (t == "Nand") { Nand g(a, b); res = g.get_Y(); }
                if (t == "Nor")  { Nor g(a, b); res = g.get_Y(); }
                if (t == "Xor")  { Xor g(a, b); res = g.get_Y(); }
                if (t == "Xnor") { Xnor g(a, b); res = g.get_Y(); }

                st.push(res);
            } else {
                cout << "Error: Unknown gate " << t << "\n"; 
                error = 1; 
                break;
            }
        }

        if (!error && st.size() == 1) {
            cout << "Result: " << st.top() << "\n";
        } else if (!error) {
            cout << "Error: Invalid reduction structure.\n";
        }
    }

    return 0;
}
