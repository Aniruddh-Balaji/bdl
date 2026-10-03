#include <chrono>
#include <cmath>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
using namespace std;
namespace bdl {
class Circuits {
public:
  Circuits() {}
  virtual ~Circuits() {}
};
class Combinational : public Circuits {
public:
  virtual ~Combinational() {}
  Combinational() {}
  virtual bool get_Y() = 0;
};
class Nand : public Combinational {
protected:
  bool& A;
  bool& B;

public:
  Nand(bool& A, bool& B) : A(A), B(B) {}
  bool get_Y() { return !(A && B); }
  virtual ~Nand() {}
};
class Nor : public Combinational {
protected:
  bool& A;
  bool& B;

public:
  Nor(bool& A, bool& B) : A(A), B(B) {}
  bool get_Y() { return !(A || B); }
  virtual ~Nor() {}
};
class Not : public Combinational {
protected:
  Nand n1;

public:
  Not(bool& A) : n1(A,A) {}
  bool get_Y() { return n1.get_Y(); }
  virtual ~Not() {}
};
class And : public Combinational {
protected:
  Nand n1, n2;
  bool w;

public:
  And(bool& A, bool& B) : n1(A,B), n2(w,w) {}
  bool get_Y() {
    w = n1.get_Y();
    return n2.get_Y();
  }
  virtual ~And() {}
};
class Or : public Combinational {
protected:
  Nor n1, n2;
  bool w;

public:
  Or(bool& A, bool& B) : n1(A,B), n2(w,w) {}
  bool get_Y() {
    w = n1.get_Y();
    return n2.get_Y();
  }
  virtual ~Or() {}
};
class Xor : public Combinational {
protected:
  Nand n1, n2, n3, n4, n5;
  bool w1, w2, w3, w4;

public:
  Xor(bool& A, bool& B)
    : n1(A,B), n2(w1,B), n3(B,B), n4(A,w3), n5(w2,w4) {}
  bool get_Y() {
    w1 = n1.get_Y();
    w2 = n2.get_Y();
    w3 = n3.get_Y();
    w4 = n4.get_Y();
    return n5.get_Y();
  }
  virtual ~Xor() {}
};
class Xnor : public Combinational {
protected:
  Nand n1, n2, n3, n4, n5, n6;
  bool w1, w2, w3, w4, w5;

public:
  Xnor(bool& A, bool& B)
    : n1(A,B), n2(w1,B), n3(B,B), n4(A,w3), n5(w2,w4), n6(w5,w5) {}
  bool get_Y() {
    w1 = n1.get_Y();
    w2 = n2.get_Y();
    w3 = n3.get_Y();
    w4 = n4.get_Y();
    w5 = n5.get_Y();
    return n6.get_Y();
  }
  virtual ~Xnor() {}
};
class Full_Adder : public Combinational {
protected:
  bool& A;
  bool& B;
  bool& Cin;
  Xor n1;
  Not n2, n3;
  And n4, n5;
  Or l5;
  And n6, n7, n8;
  Or n9, n10;
  bool s1, s2, s3, s4, s5, s6, s7, s8, s9;

public:
  Full_Adder(bool& A, bool& B, bool& Cin)
    : A(A), B(B), Cin(Cin),
      n1(A, B), n2(s1), n3(Cin), n4(s1, s3), n5(s2, Cin), l5(s4, s5),
      n6(A, B), n7(Cin, B), n8(Cin, A), n9(s6, s7), n10(s8, s9) {}
  bool get_Y() {
    s1 = n1.get_Y();
    s2 = n2.get_Y();
    s3 = n3.get_Y();
    s4 = n4.get_Y();
    s5 = n5.get_Y();
    return l5.get_Y();
  }
  bool get_Cout() {
    s1 = n1.get_Y();
    s2 = n2.get_Y();
    s6 = n6.get_Y();
    s7 = n7.get_Y();
    s8 = n8.get_Y();
    s9 = n9.get_Y();
    return n10.get_Y();
  }
  virtual ~Full_Adder() {}
};

class Ripplecarryadder4b : public Combinational {
protected:
  bool& Cin;
  vector<bool>& Inp1;
  vector<bool>& Inp2;
  vector<bool>& Out;
  bool a0, a1, a2, a3;
  bool b0, b1, b2, b3;

  Full_Adder f1, f2, f3, f4;
  bool c1, c2, c3;

public:
  Ripplecarryadder4b(vector<bool>& Inp1, vector<bool>& Inp2, bool& Cin, vector<bool>& Out)
    : Cin(Cin), Inp1(Inp1), Inp2(Inp2), Out(Out),
      f1(a0, b0, Cin),
      f2(a1, b1, c1),
      f3(a2, b2, c2),
      f4(a3, b3, c3) {}
        
  bool get_Y() {
    a0 = Inp1[0]; a1 = Inp1[1]; a2 = Inp1[2]; a3 = Inp1[3];
    b0 = Inp2[0]; b1 = Inp2[1]; b2 = Inp2[2]; b3 = Inp2[3];
    c1 = f1.get_Cout();
    c2 = f2.get_Cout();
    c3 = f3.get_Cout();
    Out[0] = f1.get_Y();
    Out[1] = f2.get_Y();
    Out[2] = f3.get_Y();
    Out[3] = f4.get_Y();
    return 0;
  }
  bool get_Cout() {
    a0 = Inp1[0]; a1 = Inp1[1]; a2 = Inp1[2]; a3 = Inp1[3];
    b0 = Inp2[0]; b1 = Inp2[1]; b2 = Inp2[2]; b3 = Inp2[3];
    c1 = f1.get_Cout();
    c2 = f2.get_Cout();
    c3 = f3.get_Cout();
    return f4.get_Cout();
  }
  virtual ~Ripplecarryadder4b() {}
};

class Mux : public Combinational {
protected:
  bool& A;
  bool& B;
  bool& S;
  Not n1;
  And n2, n3;
  Or n4;
  bool w1, w2, w3;

public:
  Mux(bool& A, bool& B, bool& S)
    : A(A), B(B), S(S), n1(S), n2(w1, A), n3(S, B), n4(w2, w3) {}
  bool get_Y() {
    w1 = n1.get_Y();
    w2 = n2.get_Y();
    w3 = n3.get_Y();
    return n4.get_Y();
  }
  virtual ~Mux() {}
};

class Encoder : public Combinational {
protected:
  bool& A3;
  bool& A2;
  bool& A1;
  bool& A0;
  Or n1, n2;

public:
  Encoder(bool& A3, bool& A2, bool& A1, bool& A0)
    : A0(A0), A1(A1), A2(A2), A3(A3), n1(A3, A2), n2(A1, A3) {}
  bool get_Y() { return 0; }
  bool get_Y1() { return n1.get_Y(); }
  bool get_Y0() { return n2.get_Y(); }
  virtual ~Encoder() {}
};

class Decoder : public Combinational {
protected:
  bool& A1;
  bool& A0;
  Not n1, n2;
  And n3, n4, n5, n6;
  bool w1, w2;

public:
  Decoder(bool& A1, bool& A0)
    : A0(A0), A1(A1),
      n1(A1), n2(A0), n3(A1, A0), n4(w1, A0), n5(A1, w2), n6(w1, w2) {}
  bool get_Y() { return 0; }
  bool get_Y3() { return n3.get_Y(); }
  bool get_Y2() { return n5.get_Y(); }
  bool get_Y1() { return n4.get_Y(); }
  bool get_Y0() { return n6.get_Y(); }
  virtual ~Decoder() {}
};

class Comparator : public Combinational {
protected:
  bool& A1;
  bool& A0;
  bool& B1;
  bool& B0;
  Not n1, n4, n5, n11, n12;
  And n2, n6, n7, n13;
  Xor n3, n9, n10;
  Or n8;
  bool w1, w2, w3, w4, w5, w6, w7, w8, w9, w10, w11, w12;

public:
  Comparator(bool& A1, bool& A0, bool& B1, bool& B0)
    : A0(A0), A1(A1), B0(B0), B1(B1), n1(B1), n2(A1, w1), n3(A1, B1),
      n4(w2), n5(B0), n6(A0, w3), n7(w2, w4), n8(w5, w6), n9(A1, B1),
      n10(A0, B0), n11(w7), n12(w8), n13(w9, w10) {}
  bool get_Y() { return 0; }
  bool get_AgtB() {
    w1 = n1.get_Y();
    w2 = n3.get_Y();
    w3 = n5.get_Y();
    w4 = n6.get_Y();
    w5 = n2.get_Y();
    w6 = n7.get_Y();
    return n8.get_Y();
  }
  bool get_AeqB() {
    w7 = n9.get_Y();
    w8 = n10.get_Y();
    w9 = n11.get_Y();
    w10 = n12.get_Y();
    return n13.get_Y();
  }
  bool get_AltB() {
    bool eq = get_AeqB();
    bool gt = get_AgtB();
    return (eq == 0 && gt == 0);
  }
  virtual ~Comparator() {}
};

class Demux : public Combinational {
protected:
  bool& A;
  bool& S;
  Not n1;
  And n2, n3;
  bool w1;

public:
  Demux(bool& A, bool& S)
    : A(A), S(S), n1(S), n2(S, A), n3(w1, A) {}
  bool get_Y() { return 0; }
  bool get_Y0() {
    w1 = n1.get_Y();
    return n3.get_Y();
  }
  bool get_Y1() { return n2.get_Y(); }
  virtual ~Demux() {}
};

class Multiplier : public Combinational {
protected:
  bool& A;
  bool& B;
  bool& Cin;
  Xor n1;
  And n2, n3, n4;
  bool w1, w2;

public:
  Multiplier(bool& A, bool& B, bool& Cin)
    : A(A), B(B), Cin(Cin), n1(Cin, B), n2(w1, A), n3(A, B), n4(w2, Cin) {}
  bool get_Y() { return 0; }
  bool get_Y0() {
    w1 = n1.get_Y();
    return n2.get_Y();
  }
  bool get_Y1() {
    w2 = n3.get_Y();
    return n4.get_Y();
  }
  virtual ~Multiplier() {}
};


class Sequential : public Circuits {
protected:
  bool Clk;
  bool Rst;

public:
  virtual void Clock(int S) {
    while (1) {
      Clk = 0;
      std::this_thread::sleep_for(std::chrono::milliseconds(S));
      Clk = 1;
      std::this_thread::sleep_for(std::chrono::milliseconds(S));
    }
  }
  virtual void Reset(int S) {
    Rst = 1;
    std::this_thread::sleep_for(std::chrono::milliseconds(S));
    Rst = 0;
  }
  virtual ~Sequential() {}
};

class Dlatch : public Sequential {
protected:
  bool D;
  bool Q;

public:
  Dlatch(int S, int R) {
    Q = 0;
    std::thread ResetThread(&Sequential::Reset, this, R);
    std::thread ClockThread(&Sequential::Clock, this, S);
    while (1) {
      cin >> D;
      if (Clk == 1 && Rst == 0) {
        Q = D;
      }
      cout << "Reset= " << Rst << " ";
      cout << "CLK= " << Clk << " ";
      cout << "Q= " << Q << endl;
    }
    ResetThread.join();
    ClockThread.join();
  }
  virtual ~Dlatch() {}
  bool get_Q() { return Q; }
};

class Dlatch2 : public Sequential {
protected:
  bool Q;
  bool Qn;
  bool w1, w2;

public:
  Dlatch2() {
    Q = 0;
    Qn = 1;
    w1 = false;
    w2 = false;
  }
  void Setvalue(bool& D, bool& en) {
    Nand n1(D, en);
    w1 = n1.get_Y();
    
    Nand n2(w1, en);
    w2 = n2.get_Y();

    for (int i = 0; i < 10; i++) {
      Nand n3(Qn, w1);
      Nand n4(Q, w2);
      bool next_Q = n3.get_Y();
      bool next_Qn = n4.get_Y();
      if (Q == next_Q && Qn == next_Qn) {
        break;
      }
      Q = next_Q;
      Qn = next_Qn;
    }
  }
  bool get_Q() { return Q; }
  bool get_Qn() { return Qn; }
  virtual ~Dlatch2() {}
};

class Dflipflop : public Sequential {
protected:
  bool Q;
  bool Qn;
  Dlatch2 d1;
  Dlatch2 d2;
  bool clk_bar;
  bool d1_q;

public:
  Dflipflop() {
    Q = 0;
    Qn = 1;
    clk_bar = false;
    d1_q = false;
  }
  void Setvalue(bool& D, bool& ClkSignal) {
    Not n1(ClkSignal);
    clk_bar = n1.get_Y();
    
    d1.Setvalue(D, clk_bar);
    d1_q = d1.get_Q();
    
    d2.Setvalue(d1_q, ClkSignal);
    Q = d2.get_Q();
    Qn = d2.get_Qn();
  }
  bool get_Q() { return Q; }
  bool get_Qn() { return Qn; }
  virtual ~Dflipflop() {}
};

class Counter : public Sequential {
protected:
  Dflipflop f1;
  bool f1_q;
  bool q_bar;

public:
  Counter() {
    f1_q = false;
    q_bar = false;
  }
  void Setvalue(bool& ClkSignal) {
    f1_q = f1.get_Q();
    Not n1(f1_q);
    q_bar = n1.get_Y();
    
    f1.Setvalue(q_bar, ClkSignal);
  }
  virtual ~Counter() {}
  bool get_Q() { return f1.get_Q(); }
};

class Counter3b : public Sequential {
protected:
  Dflipflop f1;
  Dflipflop f2;
  Dflipflop f3;
  bool f1_q, f2_q, f3_q;
  bool n1_out, n2_out, n3_out, n4_out;

public:
  Counter3b() {
    f1_q = false; f2_q = false; f3_q = false;
    n1_out = false; n2_out = false; n3_out = false; n4_out = false;
  }
  void Setvalue(bool& ClkSignal) {
    f1_q = f1.get_Q();
    f2_q = f2.get_Q();
    f3_q = f3.get_Q();
    
    Not n1(f1_q);
    Xor n2(f2_q, f1_q);
    And n3(f1_q, f2_q);
    
    n1_out = n1.get_Y();
    n2_out = n2.get_Y();
    n3_out = n3.get_Y();
    
    Xor n4(n3_out, f3_q);
    n4_out = n4.get_Y();
    
    f1.Setvalue(n1_out, ClkSignal);
    f2.Setvalue(n2_out, ClkSignal);
    f3.Setvalue(n4_out, ClkSignal);
  }
  virtual ~Counter3b() {}
  bool get_Q0() { return f1.get_Q(); }
  bool get_Q1() { return f2.get_Q(); }
  bool get_Q2() { return f3.get_Q(); }
};
} // namespace bdl
