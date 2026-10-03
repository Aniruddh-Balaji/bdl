#include "bdllibrary.hpp"
#include "../vcd_dumper.hpp"
using namespace bdl;

class Adder16 : public Combinational {
protected:
  bool A[16], B[16], Y[16], Cout;

public:
  Adder16(bool A[16], bool B[16], bool Cin) {
    bool carry = Cin;
    for (int i = 0; i < 16; i++) {
      this->A[i] = A[i];
      this->B[i] = B[i];
      Full_Adder fa(A[i], B[i], carry);
      Y[i] = fa.get_Y();
      carry = fa.get_Cout();
    }
    Cout = carry;
  }
  bool get_Y(int i) const { return Y[i]; }
  bool get_Cout() const { return Cout; }
};

class AddSub16 : public Combinational {
protected:
  bool A[16], B[16], Y[16], Cout;

public:
  AddSub16(bool A[16], bool B[16], bool SUB) {
    bool newB[16];
    for (int i = 0; i < 16; i++) {
      this->A[i] = A[i];
      this->B[i] = B[i];
      Xor x(B[i], SUB);
      newB[i] = x.get_Y();
    }
    Adder16 adder(A, newB, SUB);
    for (int i = 0; i < 16; i++) {
      Y[i] = adder.get_Y(i);
    }
    Cout = adder.get_Cout();
  }
  bool get_Y(int i) const { return Y[i]; };
  bool get_Cout() const { return Cout; }
};

class And16 : public Combinational {
protected:
  bool Y[16];

public:
  And16(bool A[16], bool B[16]) {
    for (int i = 0; i < 16; i++) {
      And a(A[i], B[i]);
      Y[i] = a.get_Y();
    }
  }
  bool get_Y(int i) const { return Y[i]; }
};

class Or16 : public Combinational {
protected:
  bool Y[16];

public:
  Or16(bool A[16], bool B[16]) {
    for (int i = 0; i < 16; i++) {
      Or o(A[i], B[i]);
      Y[i] = o.get_Y();
    }
  }
  bool get_Y(int i) const { return Y[i]; }
};

class Xor16 : public Combinational {
protected:
  bool Y[16];

public:
  Xor16(bool A[16], bool B[16]) {
    for (int i = 0; i < 16; i++) {
      Xor x(A[i], B[i]);
      Y[i] = x.get_Y();
    }
  }
  bool get_Y(int i) const { return Y[i]; }
};

class Not16 : public Combinational {
protected:
  bool Y[16];

public:
  Not16(bool A[16]) {
    for (int i = 0; i < 16; i++) {
      Not n(A[i]);
      Y[i] = n.get_Y();
    }
  }
  bool get_Y(int i) const { return Y[i]; }
};

class Nand16 : public Combinational {
protected:
  bool Y[16];

public:
  Nand16(bool A[16], bool B[16]) {
    for (int i = 0; i < 16; i++) {
      Nand n(A[i], B[i]);
      Y[i] = n.get_Y();
    }
  }
  bool get_Y(int i) const { return Y[i]; }
};

class Nor16 : public Combinational {
protected:
  bool Y[16];

public:
  Nor16(bool A[16], bool B[16]) {
    for (int i = 0; i < 16; i++) {
      Nor n(A[i], B[i]);
      Y[i] = n.get_Y();
    }
  }
  bool get_Y(int i) const { return Y[i]; }
};

class Inc16 : public Combinational {
protected:
  bool Y[16], Cout;

public:
  Inc16(bool A[16]) {
    bool zero[16] = {0};
    bool one[16] = {0};
    one[0] = 1;
    AddSub16 realInc(A, one, 0);
    for (int i = 0; i < 16; i++) {
      Y[i] = realInc.get_Y(i);
    }
    Cout = realInc.get_Cout();
  }
  bool get_Y(int i) const { return Y[i]; }
  bool get_Cout() const { return Cout; }
};

class Dec16 : public Combinational {
protected:
  bool Y[16], Cout;

public:
  Dec16(bool A[16]) {
    bool one[16] = {0};
    one[0] = 1;
    AddSub16 dec(A, one, 1);
    for (int i = 0; i < 16; i++) {
      Y[i] = dec.get_Y(i);
    }
    Cout = dec.get_Cout();
  }
  bool get_Y(int i) const { return Y[i]; }
  bool get_Cout() const { return Cout; }
};

class Shl16 : public Combinational {
protected:
  bool Y[16];

public:
  Shl16(bool A[16]) {
    Y[0] = 0;
    for (int i = 1; i < 16; i++) {
      Y[i] = A[i - 1];
    }
  }
  bool get_Y(int i) const { return Y[i]; }
};

class Shr16 : public Combinational {
protected:
  bool Y[16];

public:
  Shr16(bool A[16]) {
    Y[15] = 0;
    for (int i = 0; i < 15; i++) {
      Y[i] = A[i + 1];
    }
  }
  bool get_Y(int i) const { return Y[i]; }
};

class Comparator16 : public Combinational {
protected:
  bool AgtB, AltB, AeqB;

public:
  Comparator16(bool A[16], bool B[16]) {
    AgtB = 0;
    AltB = 0;
    AeqB = 1;
    for (int i = 14; i >= 0; i -= 2) {
      if (AeqB) {
        Comparator C(A[i + 0], A[i], B[i + 1], B[i]);
        AgtB = C.get_AgtB();
        AltB = C.get_AltB();
        AeqB = C.get_AeqB();
      }
    }
  }
  bool get_AeqB() const { return AeqB; }
  bool get_AltB() const { return AltB; }
  bool get_AgtB() const { return AgtB; }
};

class Multiplier16 : public Combinational {
protected:
  bool Y[16];

public:
  Multiplier16(bool A[16], bool B[16]) {
    bool result[16] = {0}, shiftedA[16];
    for (int i = 0; i < 16; i++) {
      shiftedA[i] = A[i];
    }
    for (int j = 0; j < 16; j++) {
      bool partial[16];
      for (int i = 0; i < 16; i++) {
        And a(shiftedA[i], B[j]);
        partial[i] = a.get_Y();
      }
      Adder16 add(result, partial, 0);
      for (int i = 0; i < 16; i++) {
        result[i] = add.get_Y(i);
      }
      bool nextA[16];
      nextA[0] = 0;
      for (int i = 1; i < 16; i++) {
        nextA[i] = shiftedA[i - 1];
      }
      for (int i = 0; i < 16; i++) {
        shiftedA[i] = nextA[i];
      }
    }
    for (int i = 0; i < 16; i++) {
      Y[i] = result[i];
    }
  }
  bool get_Y(int i) const { return Y[i]; }
};

class Mux16 : public Combinational {
protected:
  bool Y[16];

public:
  Mux16(bool D[16][16], bool S[4]) {
    for (int i = 0; i < 16; i++) {
      bool L1[8], L2[4], L3[2];
      for (int j = 0; j < 8; j++) {
        Mux m(D[2 * j][i], D[2 * j + 1][i], S[0]);
        L1[j] = m.get_Y();
      }
      for (int j = 0; j < 4; j++) {
        Mux m(L1[2 * j], L1[2 * j + 1], S[1]);
        L2[j] = m.get_Y();
      }
      for (int j = 0; j < 2; j++) {
        Mux m(L2[2 * j], L2[2 * j + 1], S[2]);
        L3[j] = m.get_Y();
      }
      Mux m(L3[0], L3[1], S[3]);
      Y[i] = m.get_Y();
    }
  }
  bool get_Y(int i) const { return Y[i]; }
};

// ALU
// Opcodes:
// 0000 = ADD
// 0001 = SUB
// 0010 = AND
// 0011 = OR
// 0100 = XOR
// 0101 = NOT
// 0110 = NAND
// 0111 = NOR
// 1000 = INC
// 1001 = DEC
// 1010 = MUL
// 1011 = SHL
// 1100 = SHR
// 1101 = PASS A
// 1110 = PASS B
// 1111 = 0

class ALU16 : public Combinational {
protected:
  bool A[16]={}, B[16]={}, Y[16]={}, opcode[4]={}, Equal, Lesser, Greater, Carry, Zero;

public:
  ALU16(bool A[16], bool B[16], bool opcode[4]) {
    for (int i = 0; i < 16; i++) {
      this->A[i] = A[i];
      this->B[i] = B[i];
    }
    for (int i = 0; i < 4; i++) {
      this->opcode[i] = opcode[i];
    }
    AddSub16 add(A, B, 0);
    AddSub16 sub(A, B, 1);
    And16 andgate(A, B);
    Or16 orgate(A, B);
    Not16 notgate(A);
    Xor16 xorgate(A, B);
    Nand16 nandgate(A, B);
    Nor16 norgate(A, B);
    Inc16 inc(A);
    Dec16 dec(A);
    Multiplier16 mul(A, B);
    Shl16 shl(A);
    Shr16 shr(A);
    Comparator16 cmp(A, B);

    bool Data[16][16];
    for (int i = 0; i < 16; i++) {
      Y[i] = 0;
      Data[0][i] = add.get_Y(i);
      Data[1][i] = sub.get_Y(i);
      Data[2][i] = andgate.get_Y(i);
      Data[3][i] = orgate.get_Y(i);
      Data[4][i] = xorgate.get_Y(i);
      Data[5][i] = notgate.get_Y(i);
      Data[6][i] = nandgate.get_Y(i);
      Data[7][i] = norgate.get_Y(i);
      Data[8][i] = inc.get_Y(i);
      Data[9][i] = dec.get_Y(i);
      Data[10][i] = mul.get_Y(i);
      Data[11][i] = shl.get_Y(i);
      Data[12][i] = shr.get_Y(i);
      Data[13][i] = A[i];
      Data[14][i] = B[i];
      Data[15][i] = 0;
    }
    Mux16 aluMux(Data, opcode);
    for (int i = 0; i < 16; i++) {
      Y[i] = aluMux.get_Y(i);
    }
    Zero = 1;
    for (int i = 0; i < 16; i++) {
      if (Y[i] == 1) {
        Zero = 0;
        break;
      }
    }
    Carry = 0;
    if (!opcode[3] && !opcode[2] && !opcode[1] && !opcode[0]) {
      Carry = add.get_Cout();
    } else if (!opcode[3] && !opcode[2] && !opcode[1] && opcode[0]) {
      Carry = sub.get_Cout();
    } else if (opcode[3] && !opcode[2] && !opcode[1] && !opcode[0]) {
      Carry = inc.get_Cout();
    } else if (opcode[3] && !opcode[2] && !opcode[1] && opcode[0]) {
      Carry = dec.get_Cout();
    }
    Equal = cmp.get_AeqB();
    Lesser = cmp.get_AltB();
    Greater = cmp.get_AgtB();
  }
  bool get_Y(int i) const { return Y[i]; }
  bool get_Carry() const { return Carry; }
  bool get_Zero() const { return Zero; }
  bool get_Equal() const { return Equal; }
  bool get_Lesser() const { return Lesser; }
  bool get_Greater() const { return Greater; }
};
