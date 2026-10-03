#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>

using namespace std;

string getVariableName(int num) {
  string name;

  do {
    name = char('a' + (num % 26)) + name;
    num = (num / 26) - 1;
  } while (num >= 0);

  return name;
}

int main() {
  try {
    string moduleName;
    if (!(cin >> moduleName)) {
      throw "Missing module name";
    }

    int submoduleCount;
    if (!(cin >> submoduleCount) || submoduleCount < 0) {
      throw "Invalid submodule count";
    }

    unordered_map<string, int> arity;

    for (int i = 0; i < submoduleCount; ++i) {
      string name;
      int inputCount;

      if (!(cin >> name >> inputCount) || inputCount < 0) {
        throw "Invalid submodule definition";
      }

      arity[name] = inputCount;
    }

    stack<string> values;
    string body;
    string word;
    int gateCount = 0;
    int wireCount = 0;
    int moduleInputCount = 0;

    while (cin >> word) {
      if (!word.empty() && word[0] >= '0' && word[0] <= '9') {
        int index = 0;

        for (char c : word) {
          if (c < '0' || c > '9') {
            throw "Invalid input index";
          }
          index = index * 10 + (c - '0');
        }

        if (index > moduleInputCount) {
          moduleInputCount = index;
        }

        values.push(getVariableName(index));
      } else {
        if (arity.count(word) == 0) {
          throw "Unknown submodule";
        }

        int count = arity[word];

        if (static_cast<int>(values.size()) < count) {
          throw "Not enough operands for submodule";
        }

        stack<string> orderedArgs;
        for (int i = 0; i < count; ++i) {
          orderedArgs.push(values.top());
          values.pop();
        }

        string args;
        while (!orderedArgs.empty()) {
          if (!args.empty()) {
            args += ", ";
          }
          args += orderedArgs.top();
          orderedArgs.pop();
        }

        string instance = "g" + to_string(++gateCount);
        string dest = "w" + to_string(++wireCount);

        body += "        " + word + " " + instance + "(" + args + ");\n";
        body += "        " + dest + " = " + instance + ".get_Y();\n";

        values.push(dest);
      }
    }

    if (values.size() != 1) {
      throw "Expression must leave exactly one result on the stack";
    }

    string output = values.top();
    int inputCount = moduleInputCount + 1;

    cout << "class " << moduleName << " : public Combinational {\n";
    cout << "protected:\n";

    cout << "    bool ";
    for (int i = 0; i < inputCount; ++i) {
      if (i > 0)
        cout << ", ";
      cout << getVariableName(i);
    }
    cout << ";\n";

    if (wireCount > 0) {
      cout << "    bool ";
      for (int i = 1; i <= wireCount; ++i) {
        if (i > 1)
          cout << ", ";
        cout << "w" << i;
      }
      cout << ";\n";
    }

    cout << "\npublic:\n";
    cout << "    " << moduleName << "(";

    for (int i = 0; i < inputCount; ++i) {
      if (i > 0)
        cout << ", ";
      cout << "bool " << getVariableName(i);
    }

    cout << ") : ";
    for (int i = 0; i < inputCount; ++i) {
      if (i > 0)
        cout << ", ";
      string name = getVariableName(i);
      cout << name << "(" << name << ")";
    }

    cout << " {\n";
    cout << body;
    cout << "    }\n";
    cout << "    bool get_Y() const { return " << output << "; }\n";
    cout << "};\n";
  } catch (const char *error) {
    cout << "Error: " << error << '\n';
    return 1;
  }

  return 0;
}
