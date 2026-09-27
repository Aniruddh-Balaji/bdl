#include<iostream>
#include<vector>
#include<string>
#include<cmath>
#include<thread>
#include<chrono>
using namespace std;
class Circuits{
    protected:
        int a;
    public:
        Circuits(){}
        virtual ~Circuits(){}
};
class Combinational:public Circuits{
    protected:
        int a;
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
            Or n2(A1,n1.get_Y());
            And n3(B0,A0);
            And n4(n2.get_Y(),n3.get_Y());
            And n5(B1,A1);
            Or n6(n5.get_Y(),n4.get_Y());
            AgtB=n6.get_Y();
            Xor n7(A1,B1);
            Xor n8(A0,B0);
            Not n9(n7.get_Y());
            Not n10(n8.get_Y());
            And n11(n9.get_Y(),n10.get_Y());
            AeqB=n11.get_Y();
            if(AeqB==0 && AgtB==0){
                AltB==1;
            }
        }
        ~Comparator(){/*cout<<"Comparator\n";*/}
        bool get_AeqB() const{return AgtB;}
        bool get_AltB() const{return AltB;}
        bool get_AgtB() const{return AeqB;}
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


/*class Encoder:public Combinational{
    protected:
        vector<bool>A;
        vector<bool>Y;
    public:
        Encoder(vector<bool> A){
            this->A=A;
            for(int i=0;i<log2(A.size());i++){
                Y.push_back(0);
            }
            int p=0;
            int c=0;
            for(int i=0;i<A.size();i++){

            +if(A[i]==1){
                    p+=1;
                    c=i;
                }
                if(p>1){
                    break;
                }
            }
            if(p==1){*/
/*
class Stream_Mux{
    protected:
        vector<bool>A;
        vector<bool>B;
        vector<bool>S;
        vector<bool>Y;
    public:
        Mux(vector<bool>A,vector<bool>B,vector<bool>S){
            if(A.size()!=B.size()){
                throw"Cannot evaluate because your A and B don't match in size\n";
            else{
                if(2**(C.size())<A.size()){
                    throw"not enough values in your selction bits(S)\n";
                }
                else{
                    this->A=A;
                    this->B=B;
                    this->S=S;
                    this->Y=Y;
*/
/*class D_latch:public Combinational{
    protected:
        bool D;
        bool Q;
        bool ck;
    public:
        D_latch(){
            while(1){
        
};*/
/*class Sequential:public Circuit{
      protected:
          bool Clk;
          bool Rst;
          bool En;
          vector<bool> Input;
          vector<bool> Output;
      public:
          virtual void Clock(int S){
              Clk=0;
              sleep_for(milliseconds(S));
              Clk=1;
              sleep_for(milliseconds(S));
          }
          virtual void Reset(int S){
              Reset=1;
              sleep_for(milliseconds(S));
              Reset=0;
          }
          Sequential(vector<bool> &Input,vector<bool> &Output){
              this->Input=Input;
              this->Output=Output;
          }
          virtual ~Sequential(){}
};
class D_latch:public Sequential{
    protected:
        vector <int> D;
        vector <int> Q;
    public:
        D_latch(bool start,vector<int> D,int S0,int S1){
            int sum=0;
            Q=new
            for(int i=0;i<D.size();i++){
                sum+=D[i];
            }
            Reset(S0);
            for(int i=0;i<sum*2*S1;i++){
                Sequential::Clock(S1);
                if(Clk==1){
                    Q[i]=D[i];
                }
            }
        }*/

            




int main(){
    for(int i=0;i<2;i++){
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
    }
}

