#include "../bdllibrary.hpp"
#include "../../vcd_dumper.hpp"
#include <string>
#include <vector>
using namespace std;
using namespace bdl;
using namespace vcd;
vector<bool> to_binary(int i){
    vector <bool> v(4,0);
    int c=0;
    while(i){
        v[c]=i%2;
        i/=2;
        c++;
    }
    return v;
}
int main(){
    Vcdwriter v("dump.vcd");
    int a=v.Addsignal("A",4);
    int b=v.Addsignal("B",4);
    int c=v.Addsignal("Y",5);
    int t=0;

    vector<bool> v1(4,0), v2(4,0), v3(4,0);
    bool cin_val = 0;
    Ripplecarryadder4b f1(v1,v2,cin_val,v3);

    for(int i=0;i<16;i++){
        for(int j=0;j<16;j++){
            v1=to_binary(i);
            v2=to_binary(j);
            v.Setvalue(a,i);
            v.Setvalue(b,j);
            f1.get_Y();
            v.Setvalue(c,v3[0]+2*v3[1]+4*v3[2]+8*v3[3]+16*f1.get_Cout());
            v.Writetimestamp(t);
            t+=5;
        }
    }
    v.Finish(t);
}
