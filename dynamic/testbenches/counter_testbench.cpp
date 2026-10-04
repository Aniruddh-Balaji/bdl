#include "../../vcd_dumper.hpp"
#include "../bdllibrary.hpp"
using namespace bdl;
using namespace std;
using namespace vcd;

int main() {
  Counter3b c;
  Vcdwriter v("dump.vcd");
  int a = v.Addsignal("Clk", 1);
  int b = v.Addsignal("Q", 3);
  int t = 0;

  bool clk_val = false;

  for (int i = 0; i < 16; i++) {
    clk_val = false;
    c.Setvalue(clk_val);
    v.Setvalue(a, 0);
    v.Setvalue(b, c.get_Q0() + 2 * c.get_Q1() + 4 * c.get_Q2());
    v.Writetimestamp(t);
    t += 5;

    clk_val = true;
    c.Setvalue(clk_val);
    v.Setvalue(a, 1);
    v.Setvalue(b, c.get_Q0() + 2 * c.get_Q1() + 4 * c.get_Q2());
    v.Writetimestamp(t);
    t += 5;
  }
  v.Finish(t);
};
