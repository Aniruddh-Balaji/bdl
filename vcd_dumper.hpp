/* This file is made to help the user write a testbench using the functions in
 * this class this file then creates a vcd file from the written code which can
 * be viewed using gtkwave which produces some nice waveforms this is more
 * visually appealing compared to a simple cli output*/
#include <fstream> //used for file operations
#include <string>
#include <vector>
using namespace std;
// This is the only class in the file this is a .hpp file (c++ header file)
namespace vcd {
class Vcdwriter {
private:
  // The struct signal has fields signal name , width(no of bits) , id , and
  // value
  struct Signal {
    string name;
    int width; // specifies no of bits
    char id; // id is a charecter that helps the vcd file/gtkwave keep track of
             // what value changed instead of using long names like flipflop we
             // use chars like A,B etc to denote each signal
    unsigned long long current_val;
    unsigned long long last_val;
  };
  // file object
  ofstream file;
  vector<Signal> signals; // array of signal structs stored in signals
  bool header_written =
      0; // bit keeps track if header was written currently false
  // Converts numbers to binary strings for GTKWave
  string to_vcd_binary(int width, unsigned long long val, char id) {
    // if width is 1 we just return the value concatenates with id
    if (width == 1) {
      if (val == 0) {
        return "0" + string(1, id);
      } else {
        return "1" + string(1, id);
      }
    }
    string bits = "";
    // since gtkwave recognises only binary numbers in format bxxxx this loop
    // converts decimals to binary and then concatenates id. conversion using
    // repeated division remainder gives binary bits
    for (int i = 0; i < width; i++) {
      if (val % 2 == 0) {
        bits = "0" + bits;
      } else {
        bits = "1" + bits;
      }
      val = val / 2;
    }
    return "b" + bits + " " + string(1, id); // string(a,b) makes a string like
                                             // "b"*a or b repeats a times
  }

public:
  // opens the file upon construction
  Vcdwriter(string filename) { file.open(filename); }
  // closes the file if it is open on destruction of object
  virtual ~Vcdwriter() {
    if (file.is_open()) {
      file.close();
    }
  }
  // Use this in testbench to create your new signal
  int Addsignal(string name, int width) {
    Signal sig;
    sig.name = name;
    sig.width = width;
    sig.id = 'A' + signals.size(); // Generates unique ids for each signal
                                   // starting from A and moving upwards
    sig.current_val = 0;
    sig.last_val = 0;
    signals.push_back(sig);
    return signals.size() - 1; // return current size of vector signals
  }
  // use to change signal value
  void Setvalue(int id, unsigned long long val) {
    signals[id].current_val = val;
  }
  // This adds the headers definition of signals and then dumps each signals
  // value at the give time
  void Writetimestamp(unsigned long long time_ns) {
    // write header (this defines the time scale for simulation
    if (header_written == false) {
      file << "$timescale 1ns $end\n";
      file << "$scope module top $end\n"; // this puts this string into the vcd
                                          // dump file
      for (size_t i = 0; i < signals.size(); i++) {
        file << "$var wire " << signals[i].width << " " << signals[i].id << " "
             << signals[i].name; // writes lines of the formm  width id name {1
                                 // A Clk} in the .vcd file
        if (signals[i].width > 1) {
          file << " [" << signals[i].width - 1 << ":0]";
        }
        file << " $end\n";
      }
      file << "$upscope $end\n";
      file << "$enddefinitions $end\n";
      file << "#" << time_ns << "\n";
      file << "$dumpvars\n";
      // adds lines of the form b101A or 1A
      for (size_t i = 0; i < signals.size(); i++) {
        file << to_vcd_binary(signals[i].width, signals[i].current_val,
                              signals[i].id)
             << "\n";
        signals[i].last_val = signals[i].current_val;
      }
      file << "$end\n";
      header_written = 1;
      return;
    }
    // Step 2:Write changed values on subsequent runs
    bool printed_time = 0;
    // it only prints if the current value has changed since the last run that
    // is why we have the if statement it optimises file size
    for (size_t i = 0; i < signals.size(); i++) {
      if (signals[i].current_val != signals[i].last_val) {
        if (printed_time == false) {
          file << "#" << time_ns << "\n";
          printed_time = true;
        }
        file << to_vcd_binary(signals[i].width, signals[i].current_val,
                              signals[i].id)
             << "\n";
        signals[i].last_val = signals[i].current_val;
      }
    }
  }
};
} // namespace vcd
/*This is a sample vcd file given below to help you the user  understand the
program and write testbenches dump.vcd $timescale 1ns $end $scope module top
$end $var wire 1 A clk $end $var wire 3 B D [2:0] $end $var wire 3 C Q [2:0]
$end $upscope $end $enddefinitions $end #10 $dumpvars 0A b000B b000C $end #20 1A
b000B
b001C
$end
...
*/
