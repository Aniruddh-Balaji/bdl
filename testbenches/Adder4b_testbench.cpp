#include "bdllibrary.hpp"
#include "vcd_dumper.hpp"
#include <string>
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
    Vcdwriter v("vcd.dump");
    int a=v.Addsignal("A",4);
    int b=v.Addsignal("B",4);
    int c=v.Addsignal("Y",5);
    int t=0;
    for(int i=0;i<16;i++){
        for(int j=0;j<16;j++){
            vector<bool> v1=to_binary(i);
            vector <bool>v2=to_binary(j);
            Ripplecarryadder4b f1(v1,v2,0);
            v.Setvalue(a,i);
            v.Setvalue(b,j);
            vector<bool>v3=f1.get_Y();
            v.Setvalue(c,v3[0]+2*v3[1]+4*v3[2]+8*v3[3]+16*f1.get_Cout());
            v.Writetimestamp(t);
            t+=5;
        }
    }
}
