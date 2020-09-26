#include <iostream>
#include <string>
#include <cctype>
#include <sstream>
#include <cstdlib>
#include <cstdio>

#define ESC "\e[0m"
#define AUTHOR "\e[40;38;5;82;4m"
#define ESC_AUTHOR "\e[0;24m"
#define PROMPT "\e[33;1ma\e[0m\e[34;1ms\e[0m\e[33;1mh\e[0m>"

using namespace std;

void exit() {
  cout << "Thank you for using our Aggie Shell!" << endl;
  cout << "Made By: " << AUTHOR << "Joshua Grovan"<< ESC_AUTHOR << " & " << AUTHOR << "James Ostmann" << ESC_AUTHOR << endl;
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
