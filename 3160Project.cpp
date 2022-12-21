#include <cctype>
#include <iostream>
#include <iterator>
#include <sstream>
#include <unordered_map>
#include <vector>
using namespace std;

bool isIdentifier(const string &str) {
  if (!isalpha(str[0]) && str[0] != '_')
    return false;

  for (int i = 1; i < str.size(); i++)
    if (!isalpha(str[i]) && !isdigit(str[i]) && str[i] != '_')
      return false;

  return true;
}

bool isLiteral(const string &str) {
  if (str.empty() || !isdigit(str[0]))
    return false;

  for (int i = 1; i < str.size(); i++)
    if (!isdigit(str[i]))
      return false;

  return true;
}

void parseAssignment(const vector<string> &tokens,
                     unordered_map<string, int> &variables) {

  if (tokens.size() != 3 || tokens[1] != "=") {
    cout << "Syntax Error!" << endl;
    return;
  }

  if (!isIdentifier(tokens[0])) {
    cout << "Invalid identifier: " << tokens[0] << endl;
    return;
  }

  int value = 0;
  if (isLiteral(tokens[2]))
    value = stoi(tokens[2]);
  else if (isIdentifier(tokens[2])) {
    if (variables.count(tokens[2]) == 0) {
      cout << "Uninitialized variable: " << tokens[2] << endl;
      return;
    }

    value = variables[tokens[2]];
  } else {
    cout << "Invalid literal or identifier: " << tokens[2] << endl;
    return;
  }

  variables[tokens[0]] = value;
}

int parseExp(const vector<string> &tokens, int start_index,
             unordered_map<string, int> &variables);

int parseFact(const vector<string> &tokens, int start_index,
              unordered_map<string, int> &variables) {
  if (tokens[start_index] == "(") {
    int value = parseExp(tokens, start_index + 1, variables);
    if (tokens[start_index + 2] != ")") {
      cout << "Syntax Error!" << endl;
      return 0;
    }

    return value;
  } else if (tokens[start_index] == "+")
    return parseFact(tokens, start_index + 1, variables);
  else if (tokens[start_index] == "-")
    return -parseFact(tokens, start_index + 1, variables);
  else if (isLiteral(tokens[start_index]))
    return stoi(tokens[start_index]);
  else if (isIdentifier(tokens[start_index])) {
    if (variables.count(tokens[start_index]) == 0) {
     cout << "Uninitialized variable: " << tokens[start_index] << endl;
      return 0;
    }

    return variables[tokens[start_index]];
  } else {
    cout << "Invalid literal or identifier: " << tokens[start_index] << endl;
    return 0;
  }
}

int parseTerm(const vector<string> tokens, int start_index,
              unordered_map<string, int> &variables) {
  int value = parseFact(tokens, start_index, variables);
  int i = start_index + 1;
  while (i < tokens.size()) {
    if (tokens[i] == "*")
      value *= parseFact(tokens, i + 1, variables);
    else
      break;

    i += 2;
  }

  return value;
}

int parseExp(const vector<string> &tokens, int start_index,
             unordered_map<string, int> &variables) {
  int value = parseTerm(tokens, start_index, variables);
  int i = start_index + 1;
  while (i < tokens.size()) {
    if (tokens[i] == "+")
      value += parseTerm(tokens, i + 1, variables);
    else if (tokens[i] == "-")
      value -= parseTerm(tokens, i + 1, variables);
    else
      break;

    i += 2;
  }

  return value;
}

int main(int argc, char **argv) {

  if (argc < 2) {
    cout << "Error: No input provided!" << endl;
    return 1;
  }

  unordered_map<string, int> variables;

  string line(argv[1]);

  istringstream iss(line);
  vector<string> tokens{istream_iterator<string>{iss},
                        istream_iterator<string>{}};

  
  if (tokens.size() >= 3 && tokens[1] == "=")
    parseAssignment(tokens, variables);
  else
    cout << parseExp(tokens, 0, variables) << endl;

 
  cout << "Variable values:" << endl;
  for (const auto &[key, value] : variables)
    cout << key << " = " << value << endl;

  return 0;
}
