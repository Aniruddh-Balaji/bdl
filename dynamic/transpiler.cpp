#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>
using namespace std;

string wirename(int n) {
  string name;
  do {
    name = char('a' + (n % 26)) + name;
    n = n / 26 - 1;
  } while (n >= 0);
  return name;
}

int main() {
  try {
    string modname;
    if (!(cin >> modname))
      throw "missing module name";
    int submodcount;
    if (!(cin >> submodcount) || submodcount < 0)
      throw "invalid submodule count";

    unordered_map<string, int> modinputs;
    for (int i = 0; i < submodcount; i++) {
      string mod;
      int inputs;
      if (!(cin >> mod >> inputs) || inputs < 0)
        throw "invalid submodule definition";
      modinputs[mod] = inputs;
    }

    stack<string> val;
    string protectedGates;
    string initList;
    string evalCode;
    string word;
    int gates = 0, wires = 0, inputcount = 0;

    while (cin >> word) {
      if (word.size() && word[0] >= '0' && word[0] <= '9') {
        int index = 0;
        for (char c : word) {
          if (c < '0' || c > '9')
            throw "invalid input index";
          index = index * 10 + c - '0';
        }
        if (index > inputcount)
          inputcount = index;
        val.push(wirename(index));
        continue;
      } else {
        if (!modinputs.count(word))
          throw "submodule not found";
        int count = modinputs[word];
        if (val.size() < count)
          throw "not enough inputs for submodule";
        stack<string> orderargs;
        for (int i = 0; i < count; i++) {
          orderargs.push(val.top());
          val.pop();
        }

        string args;
        while (orderargs.size()) {
          if (args.size())
            args += ", ";
          args += orderargs.top();
          orderargs.pop();
        }
        string inst = 'g' + to_string(++gates);
        string dest = 'w' + to_string(++wires);

        protectedGates += "  " + word + " " + inst + ";\n";
        
        if (initList.size())
          initList += ", ";
        initList += inst + "(" + args + ")";

        evalCode += "    " + dest + " = " + inst + ".get_Y();\n";
        val.push(dest);
      }
    }

    if (val.size() != 1)
      throw "expression must leave one expr on stack";

    string output = val.top();
    cout << "class " << modname << " : public Combinational {\n";
    cout << "protected:\n";
    cout << "  bool &";
    for (int i = 0; i <= inputcount; i++) {
      if (i > 0)
        cout << ", &";
      cout << wirename(i);
    }
    cout << ";\n";

    if (wires > 0) {
      cout << "  bool ";
      for (int i = 1; i <= wires; i++) {
        if (i > 1)
          cout << ", ";
        cout << "w" << i;
      }
      cout << ";\n";
    }
    cout << protectedGates;

    cout << "\npublic:\n";
    cout << "  " << modname << "(";
    for (int i = 0; i <= inputcount; i++) {
      if (i > 0)
        cout << ", ";
      cout << "bool& " << wirename(i);
    }

    cout << ") : ";
    for (int i = 0; i <= inputcount; i++) {
      string name = wirename(i);
      cout << name << "(" << name << "), ";
    }
    cout << initList << " {}\n\n";

    cout << "  bool get_Y() {\n";
    cout << evalCode;
    cout << "    return " << output << ";\n";
    cout << "  }\n";
    cout << "};\n";
  } catch (const char* e) {
    cout << "Error: " << e << '\n';
  }
}
