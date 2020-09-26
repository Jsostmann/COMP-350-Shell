#include <iostream>
#include <string>
#include <cctype>
#include <sstream>
#include <cstdlib>
#include <cstdio>

#define PROMPT "\e[33;1ma\e[0m\e[34;1ms\e[0m\e[33;1mh\e[0m>"

using namespace std;

void exit() {
  cout << "Thank you for using our aggie shell" << endl;
  cout << "Made By: " << "Joshua Grovan, & James Ostmann" << endl;
  exit(0);
}

string getInput(string answer) {
  getline(cin, answer);
  return answer;
}

// TODO: implement parser for individual commmands
void parseCommands(string userInput){
  string parse;
  bool space = false;
  for(int i = 0; i < userInput.length(); i++) {
        
    if(space && isspace(userInput.at(i))) {
      space = false;
    }
    if(isspace(userInput.at(i))) {
      space = true;
    }
    if(space) {
      parse += userInput.at(i);
    }
  }
  cout << parse << parse.length() << endl;
}

int main(int argv, char** argc) {
  string userInput;
    
  while (true) {
    cout << PROMPT;
    userInput = getInput(userInput);
    
    if(userInput == "exit") {
      exit();
    }
        
  }
  return 0;
}
