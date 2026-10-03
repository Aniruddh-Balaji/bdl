#include<iostream>
#include<vector>
#include<string>
#include<cmath>
#include<thread>
#include<chrono>
using namespace std;
namespace bdl{
class Circuits{
    public:
        Circuits(){}
        virtual ~Circuits(){}
};
class Combinational:public Circuits{        
    public:
        virtual ~Combinational(){}
        Combinational(){}
        bool get_Y(){return 0;};
};
class Nand:public Combinational{
    protected:
        bool A;
        bool B;
        bool Y;
    public:
        Nand(bool A,bool B){
            this->A=A;
            this->B=B;
            //Y=!(A&B);
            if(A==1 && B==1){
                Y=0;
            }
            else{
                Y=1;
            }
        }
        bool get_Y() const{return Y;}
        virtual ~Nand(){/*cout<<"Nand Destroyed\n";*/}
};
class Nor: public Combinational{
    protected:
        bool A;
        bool B;
        bool Y;
    public:
        Nor(bool A,bool B){
            this->A=A;
            this->B=B;
            //Y=!(A|B);
            if(A==0 && B==0){
                Y=1;
            }
            else{
                Y=0;
            }
        }
        bool get_Y() const{return Y;}
        virtual ~Nor(){/*cout<<"Nor destroyed\n";*/}
};
class Not: public Combinational{
    protected:
        bool A;
        bool Y;
    public:
        Not(bool A){
            this->A=A;
            Nand n1(A,A);
            Y=n1.get_Y();
        }
        bool get_Y() const{return Y;}
        virtual ~Not(){/*cout<<"Nor destroyed\n";*/}
};
class And: public Combinational{
    protected:
        bool A;
        bool B;
        bool Y;
    public:
        And(bool A,bool B){
            this->A=A;
            this->B=B;
            Nand n1(A,B);
            Nand n2(n1.get_Y(),n1.get_Y());
            this->Y=n2.get_Y();
        }
        bool get_Y() const{return Y;}
        virtual ~And(){/*cout<<"And destroyed\n";*/}
};
class Or: public Combinational{
    protected:
        bool A;
        bool B;
        bool Y;
    public:
        Or(bool A,bool B){
            this->A=A;
            this->B=B;
            Nor n1(A,B);
            Nor n2(n1.get_Y(),n1.get_Y());
            Y=n2.get_Y();
        }
        bool get_Y() const{return Y;}
        virtual ~Or(){/*cout<<"Or destroyed\n";*/}
};
class Xor: public Combinational{
    protected:
        bool A;
        bool B;
        bool Y;
    public:
        Xor(bool A,bool B){
            this->A=A;
            this->B=B;
            Nand n1(A,A);
            Nand n2(n1.get_Y(),B);
            Nand n3(B,B);
            Nand n4(A,n3.get_Y());
            Nand n5(n2.get_Y(),n4.get_Y());
            Y=n5.get_Y();
        }
        bool get_Y() const{return Y;}
        virtual ~Xor(){/*cout<<"Xor destroyed\n";*/}
};
class Xnor: public Combinational{
    protected:
        bool A;
        bool B;
        bool Y;
    public:
        Xnor(bool A,bool B){
            this->A=A;
            this->B=B;
            Nand n1(A,A);
            Nand n2(n1.get_Y(),B);
            Nand n3(B,B);
            Nand n4(A,n3.get_Y());
            Nand n5(n2.get_Y(),n4.get_Y());
            Nand n6(n5.get_Y(),n5.get_Y());
            Y=n6.get_Y();
        }
        bool get_Y() const{return Y;}
        virtual ~Xnor(){/*cout<<"Xor destroyed\n";*/}
};
class Full_Adder: public Combinational{
    protected:
        bool A;
        bool B;
        bool Cin;
        bool Y;
        bool Cout;
    public:
        Full_Adder(bool A,bool B,bool Cin){
            this->A=A;
            this->B=B;
            this->Cin=Cin;
            Xor n1(A,B);
            Not n2(n1.get_Y());
            Not n3(Cin);
            And n4(n1.get_Y(),n3.get_Y());
            And n5(n2.get_Y(),Cin);
            Or l5(n4.get_Y(),n5.get_Y());
            Y=l5.get_Y();
            And n6(A,B);
            And n7(Cin,B);
            And n8(Cin,A);
            Or n9(n6.get_Y(),n7.get_Y());
            Or n10(n9.get_Y(),n8.get_Y());
            Cout=n10.get_Y();
        }
        bool get_Y() const{return Y;}
        bool get_Cout() const{return Cout;}
        virtual ~Full_Adder(){/*cout<<"Full adeer destroyed\n";*/}
};
class Ripplecarryadder4b : public Combinational{
    protected:
        bool Cin;
        vector<bool> Inp1;
        vector<bool> Inp2;
        vector<bool> Out;
        bool Cout;
    public:
        Ripplecarryadder4b(vector <bool> Inp1,vector<bool> Inp2,bool Cin){
            this->Inp1=Inp1;
            this->Inp2=Inp2;
            this->Cin=Cin;
            Full_Adder f1(Inp1[0],Inp2[0],Cin);
            Full_Adder f2(Inp1[1],Inp2[1],f1.get_Cout());
            Full_Adder f3(Inp1[2],Inp2[2],f2.get_Cout());
            Full_Adder f4(Inp1[3],Inp2[3],f3.get_Cout());
            Out[0]=f1.get_Y();
            Out[1]=f2.get_Y();
            Out[2]=f3.get_Y();
            Out[3]=f4.get_Y();
            Cout=f4.get_Cout();    
        }
        virtual ~Ripplecarryadder4b(){}
        vector <bool> get_Y()const{return Out;}
        bool get_Cout()const{return Cout;}
};
class Mux:public Combinational{
    protected:
         bool A;
         bool B;
         bool S;
         bool Y;
    public:
         Mux(bool A,bool B,bool S){
             this->A=A;
             this->B=B;
             this->S=S;
             Not n1(S);
             And n2(n1.get_Y(),A);
             And n3(S,B);
             Or n4(n2.get_Y(),n3.get_Y());
             this->Y=n4.get_Y();
        }
        bool get_Y() const{return Y;}
        virtual ~Mux(){/*cout<<"Mux Destroyed\n;*/}
};
class Encoder:public Combinational{
     protected:
         bool A3;
         bool A2;
         bool A1;
         bool A0;
         bool Y1;
         bool Y0;
    public:
         Encoder(bool A3,bool A2,bool A1,bool A0){
             this->A3=A3;
             this->A2=A2;
             this->A1=A1;
             this->A0=A0;
             //Y1
             Or n1(A3,A2);
             this->Y1=n1.get_Y();
             //Y0
             Or n2(A1,A3);
             this->Y0=n2.get_Y();   
         }
        bool get_Y1() const{return Y1;}
        bool get_Y0() const{return Y0;}
        virtual ~Encoder(){/*cout<<"Encode Destroyed\n;*/}
};
class Decoder: public Combinational{
    protected:
        bool A1;
        bool A0;
        bool Y3;
        bool Y2;
        bool Y1;
        bool Y0;
    public:
        Decoder(bool A1,bool A0){
            this->A1=A1;
            this->A0=A0;
            Not n1(A1);
            Not n2(A0);
            And n3(A1,A0);
            And n4(n1.get_Y(),A0);
            And n5(A1,n2.get_Y());
            And n6(n1.get_Y(),n2.get_Y());
            Y0=n6.get_Y();
            Y1=n4.get_Y();
            Y2=n5.get_Y();
            Y3=n3.get_Y();
        }
        bool get_Y3() const{return Y3;}
        bool get_Y2() const{return Y2;}
        bool get_Y1() const{return Y1;}
        bool get_Y0() const{return Y0;}
        ~Decoder(){/*cout<<"Decoder destroyed\n";*/}
};
class Comparator:public Combinational{
    protected:
        bool A1;
        bool A0;
        bool B1;
        bool B0;
        bool AgtB;
        bool AltB;
        bool AeqB;
    public:
        Comparator(bool A1,bool A0,bool B1,bool B0){
            this->A1=A1;
            this->A0=A0;
            this->B1=B1;
            this->B0=B0;
            Not n1(B1);
            And n2(A1,n1.get_Y());
            Xor n3(A1,B1);
            Not n4(n3.get_Y());
            Not n5(B0);
            And n6(A0, n5.get_Y());
            And n7(n4.get_Y(),n6.get_Y());
            Or n8(n2.get_Y(),n7.get_Y());
            this->AgtB=n8.get_Y();
            Xor n9(A1,B1);
            Xor n10(A0,B0);
            Not n11(n9.get_Y());
            Not n12(n10.get_Y());
            And n13(n11.get_Y(),n12.get_Y());
            this->AeqB=n13.get_Y();
            if(AeqB==0 && AgtB==0){
                this->AltB=1;
            }
            else{
                this->AltB=0;
            }
        }
        ~Comparator(){/*cout<<"Comparator\n";*/}
        bool get_AeqB() const{return AeqB;}
        bool get_AltB() const{return AltB;}
        bool get_AgtB() const{return AgtB;}
};
class Demux : public Combinational{
    protected:
        bool A;
        bool S;
        bool Y0;
        bool Y1;
    public:
        Demux(bool A,bool S){
            this->A=A;
            this->S=S;
            Not n1(S);
            And n2(S,A);
            And n3(n1.get_Y(),A);
            Y0=n3.get_Y();
            Y1=n2.get_Y();
        }
        ~Demux(){
            /*cout<<"Demux destroyed"<<endl;*/}
        bool get_Y0() const{return Y0;}
        bool get_Y1() const{return Y1;}
};

class Multiplier: public Combinational{
    protected:
        bool A;
        bool B;
        bool Y0;
        bool Y1;
        bool Cin;
    public:
        Multiplier(bool A,bool B,bool Cin){
            this->A=A;
            this->B=B;
            this->Cin=Cin;
            Xor n1(Cin,B);
            And n2(n1.get_Y(),A);
            Y0=n2.get_Y();
            And n3(A,B);
            And n4(n3.get_Y(),Cin);
            Y1=n4.get_Y();
        }
        virtual ~Multiplier(){/*cout<<"Multiplier Destroyed\n";*/}
        bool get_Y0() const{return Y0;}
        bool get_Y1() const{return Y1;}
};
/*class ParityGenerator: public Combinational{
    //even number of 1s
    protected:
        vector<bool>inp;
        vector<bool>out;
    public:
        ParityGenerator(vector<bool> &inp){
            this->inp=inp;
            bool p=0;
            if(inp.size()==1){
                if(inp[0]==0){
                    p=0;
                }
                else{
                    p=1;
                }
            }
            for(int i=0;i<inp.size()-1;i++){
                Xor n1(inp[i],inp[i+1]);
                p+=n1.get_Y();
            }
            out=inp;
            out.push_back(op);
        }
        vector <bool> 
*/
class Sequential : public Circuits{
      protected:
          bool Clk;
          bool Rst;
      public:
          virtual void Clock(int S){
              while(1){
                Clk=0;
                std::this_thread::sleep_for(std::chrono::milliseconds(S));
                Clk=1;
                std::this_thread::sleep_for(std::chrono::milliseconds(S));
              }
          }
          virtual void Reset(int S){
              Rst=1;
              std::this_thread::sleep_for(std::chrono::milliseconds(S));
              Rst=0;
          }
          virtual ~Sequential(){}
};
class Dlatch:public Sequential{
    protected:
        bool D;
        bool Q;
    public:
        Dlatch(int S,int R){
                Q=0;
                std::thread ResetThread(&Sequential::Reset,this,R);
                std::thread ClockThread(&Sequential::Clock,this,S);
                while(1){
                        cin>>D;
                        if(Clk==1 && Rst==0){
                            Q=D;
                        }
                        cout<<"Reset= "<<Rst<<" ";
                        cout<<"CLK= "<<Clk<<" ";
                        cout<<"Q= "<<Q<<endl;
                 }
                ResetThread.join();
                ClockThread.join();
        }
        virtual ~Dlatch(){}
        bool get_Q (){return Q;}
};
class Dlatch2 : public Sequential{
    protected:
        bool D;
        bool Q;
        bool Qn;
    public:
        Dlatch2(){
            Q=0;
            Qn=1;
        }
        void Setvalue(bool D,bool en){ 
            Nand n1(D,en);
            Nand n2(n1.get_Y(),en);
            for(int i=0;i<10;i++){
                Nand n3(Qn,n1.get_Y());
                Nand n4(Q,n2.get_Y());
                bool next_Q = n3.get_Y();
                bool next_Qn = n4.get_Y();
                if(Q==next_Q && Qn==next_Qn){
                    break;}
                Q=next_Q;
                Qn=next_Qn;
            }
        }
        bool get_Q()const{return Q;}
        bool get_Qn()const{return Qn;}
        virtual ~Dlatch2(){}
}; 
class Dflipflop : public Sequential{
    protected:
        bool D;
        bool Q;
        bool Qn;
        Dlatch2 d1;
        Dlatch2 d2;
    public:
        Dflipflop(){Q=0;Qn=1;}
        void Setvalue(bool D,bool Clk){
            this->D=D;
            Not n1(Clk);
            d1.Setvalue(D,n1.get_Y());
            d2.Setvalue(d1.get_Q(),Clk);
            Q=d2.get_Q();
            Qn=d2.get_Qn();
        }
        virtual ~Dflipflop(){}
        bool get_Q()const{return Q;}
        bool get_Qn()const{return Qn;}
};
class Counter : public Sequential{
    protected:
        Dflipflop f1;
    public:
        Counter(){}
        void Setvalue(bool Clk){
            Not n1(f1.get_Q());
            f1.Setvalue(n1.get_Y(),Clk);
        }
        virtual ~Counter(){}
        bool get_Q()const{return f1.get_Q();}
};
class Counter3b : public Sequential{
    protected:
        Dflipflop f1;
        Dflipflop f2;
        Dflipflop f3;
    public:
        Counter3b(){}
        void Setvalue(bool Clk){
            Not n1(f1.get_Q());
            Xor n2(f2.get_Q(),f1.get_Q());
            And n3(f1.get_Q(),f2.get_Q());
            Xor n4(n3.get_Y(),f3.get_Q());
            f1.Setvalue(n1.get_Y(),Clk);
            f2.Setvalue(n2.get_Y(),Clk);
            f3.Setvalue(n4.get_Y(),Clk);
        }
        virtual ~Counter3b(){}
        bool get_Q0()const{return f1.get_Q();}
        bool get_Q1()const{return f2.get_Q();}
        bool get_Q2()const{return f3.get_Q();}
};}
//int main(){
   /* for(int i=0;i<2;i++){
        for(int j=0;j<2;j++){
            Nand n1(i,j);
            cout<<"Nand Gate: "<<"A: "<<i<<" B: "<<j<<" Y: "<<n1.get_Y()<<endl;
            Nor n2(i,j);
            cout<<"Nor Gate: "<<"A: "<<i<<" B: "<<j<<" Y: "<<n2.get_Y()<<endl;
            Or n3(i,j);
            cout<<"or Gate: "<<"A: "<<i<<" B: "<<j<<" Y: "<<n3.get_Y()<<endl;
            And n4(i,j);
            cout<<"And Gate: "<<"A: "<<i<<" B: "<<j<<" Y: "<<n4.get_Y()<<endl;
            Xor n5(i,j);
            cout<<"Xor Gate: "<<"A: "<<i<<" B: "<<j<<" Y: "<<n5.get_Y()<<endl;
            Decoder d(i,j);
            cout<<"Decoder: "<<"A1: "<<i<<" A0: "<<j<<" y3: "<<d.get_Y3()<<" y2: "<<d.get_Y2()<<" y1: "<<d.get_Y1()<<" y0: "<<d.get_Y0()<<endl;
            Demux dm(i,j);
            cout<<"Demux: "<<"A: "<<i<<"S: "<<j<<"Y0: "<<dm.get_Y0()<<"Y1: "<<dm.get_Y1()<<endl;
            for(int k=0;k<2;k++){
                Full_Adder n6(i,j,k);
                cout<<"Full adder: "<<"A: "<<i<<" B: "<<j<<"Cin: "<<k<<" Y: "<<n6.get_Y()<<"Cout: "<<n6.get_Cout()<<endl;
                Mux n7(i,j,k);
                cout<<"Mux: "<<" A: "<<i<<" B: "<<j<<" S: "<<k<<" Y: "<<n7.get_Y()<<endl;
                Multiplier m(i,j,k);
                cout<<"Multiplier: "<<"A: "<<i<<" B: "<<j<<" Cin: "<<k<<" Y0: "<<m.get_Y0()<<" Y1: "<<m.get_Y1()<<endl;
                for(int ss=0;ss<2;ss++){
                    Encoder n8(i,j,k,ss);
                    cout<<"Encoder: "<<" A3: "<<i<<" A2: "<<j<<" A1: "<<k<<" A0: "<<ss<<" Y1: "<<n8.get_Y1()<<" Y0: "<<n8.get_Y0()<<endl;
                    Comparator c(i,j,k,ss);
                    cout<<"Comparator: "<<" A1: "<<i<<" A1: "<<j<<" B1: "<<k<<" B0: "<<ss<<" A=B: "<<c.get_AeqB()<<" A>B "<<c.get_AgtB()<<"A<B"<<c.get_AltB()<<endl;
                }
            }
       }
       Not n9(i);
       cout<<"Not Gate: "<<"A: "<<i<<" Y: "<<n9.get_Y()<<endl;
    }*/
//}

