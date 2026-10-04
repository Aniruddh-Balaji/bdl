#include "../proc.hpp"
#include<cstdint>
#include<cstring>
using namespace bdl;
using namespace std;
using namespace vcd;
void to_binary(int i,bool v[],int size=16){
    memset(v,0,size*sizeof(bool));
    int c=0;
    while(i){
        v[c]=i%2;
        i/=2;
        c++;
    }
}
int main(){
    Vcdwriter v1("dump.vcd");
    int n1=v1.Addsignal("A",16);
    int n2=v1.Addsignal("B",16);
    int n3=v1.Addsignal("Y",16);
    int n4=v1.Addsignal("Opcode",4);
    int n5=v1.Addsignal("Equal",1);
    int n6=v1.Addsignal("Greater",1);
    int n7=v1.Addsignal("Lesser",1);
    int n8=v1.Addsignal("Zero",1);
    int n9=v1.Addsignal("Carry",1);
    int OP;
    int A;
    int B;
    bool a[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    bool b[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    bool op[4]={0,0,0,0};
    int t=0;
    v1.Writetimestamp(t);
    while(1){
	cout<<endl<<endl<<"--Welcome to Digital Electronics Circuits Designer--"<<endl<<endl;
	cout<<"Enter an opcode two inputs A and B ranging from 0-65535"<<endl<<endl;
	cout<<"Opcode matching:"<<endl;
	cout<<"0 - ADD"<<endl<<"1 - SUB"<<endl;
	cout<<"2 - AND"<<endl<<"3 - OR"<<endl;
	cout<<"4 - XOR"<<endl<<"5 - NOT (single input)"<<endl;
	cout<<"6 - NAND"<<endl<<"7 - NOR"<<endl;
	cout<<"8 - Increment (single input)"<<endl<<"9 - Decrement (single input)"<<endl;
	cout<<"10 - Multiplier"<<endl<<"11 - Shift left (single input)"<<endl;
	cout<<"12 - Shift right"<<endl<<"13 - Pass A (will just output A only regardless of B)"<<endl;
	cout<<"14 - Pass B (will just output B regardless of A)"<<endl<<endl<<"For opcodes with single input marked, put the desired input in A"<<endl<<endl;
        cout<<"enter opcode: ";
        cin>>OP;
        if(OP>=15 || OP<0 ){
            break;
        }
        else{
            cout<<"enter operand A: ";
            cin>>A;
            cout<<"enter operand B: ";
            cin>>B;
            to_binary(OP,op,4);
            to_binary(A,a);
            to_binary(B,b);
            ALU16 ALU(a,b,op);
            v1.Setvalue(n1,A);
            v1.Setvalue(n2,B);
            int s=0;
            for(int i=0;i<16;i++){
                s+=(ALU.get_Y(i)*(int)pow(2,i));
            }
            t+=5;
            v1.Setvalue(n3,s);
            v1.Setvalue(n4,OP);
            v1.Setvalue(n5,ALU.get_Equal());
            v1.Setvalue(n6,ALU.get_Greater());
            v1.Setvalue(n8,ALU.get_Zero());
            v1.Setvalue(n7,ALU.get_Lesser());
            v1.Setvalue(n9,ALU.get_Carry());
            v1.Writetimestamp(t);
        }
    }
    v1.Finish(t+5);
    v1.Writetimestamp(t);
    v1.Writetimestamp(t+5);
};
