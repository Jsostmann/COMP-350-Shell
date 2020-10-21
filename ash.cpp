/*
  * James Ostmann & Joshua Govan
  * COMP 350 / 001
  * Sep, 26th 2020
  * Ash Shell Project Checkpoint 1
*/
#include <iostream>
#include <string>
#include <cctype>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <vector>

#define ESC "\e[0m"
#define AUTHOR "\e[40;38;5;82;4m"
#define ESC_AUTHOR "\e[0;24m"
#define PROMPT "\e[33;1ma\e[0m\e[34;1ms\e[0m\e[33;1mh\e[0m>"
#define WORKING_DIRECTORY "\e[92m"
using namespace std;

string currentPath = "";

vector<string> parse_commands(string input) {
    vector<string> answer;
    string temp_command = "";
    int i = 0;
    while(i < input.length()) {
        if(isspace(input[i]) && temp_command != "") {
            answer.push_back(temp_command);
            temp_command = "";
        }
        if(!isspace(input[i])) {
            temp_command += input[i];
        }
        i++;
    }
    if(temp_command != "") {
        answer.push_back(temp_command);
    }
    return answer;
}

// gets the string value of the current working directory with max length allocated to 100 chars
string get_working_directory() {
  char * directory = NULL;
  return string(getcwd(directory,500));
}

// changes the working directory
int change_directory(string userInput) {
    int rc = chdir(userInput.c_str());
    return rc;
}

// exit function called when user enters exit command
void exit() {
  cout << "Thank you for using our Aggie Shell!" << endl;
  cout << "Made By: " << AUTHOR << "Joshua Govan"<< ESC_AUTHOR << " & " << AUTHOR << "James Ostmann" << ESC_AUTHOR << endl;
  exit(0);
}

// function to get userinput when entered from cin
string getInput(string answer) {
  getline(cin, answer);
  return answer;
}

// error function to handle all errors that are encountered
void error() {
  //write(STDERR_FILENO, promp_message, strlen(promp_message));
  exit();
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

void check_built_in(vector<string> commands) {
    
  string temp_command = commands.at(0);
    
  if(temp_command.compare("cd") == 0) {
    if(commands.size() > 1) {
      change_directory(commands.at(1));
    } else {
      cout << "ERROR: Missing Arguments for cd" << endl;
    }
  } else if(temp_command.compare("path") == 0) {
      if(commands.size() > 1) {
          cout << "Path Before: " << currentPath << endl;
          currentPath.erase(currentPath.begin(),currentPath.end());
          for(int i = 1; i < commands.size(); i++) {
              currentPath += commands.at(i) + "/";
          }
          cout << "Path After: " << currentPath << endl;
      }
  } else if(temp_command.compare("exit") == 0) {
      if(commands.size() < 2) {
          exit();
      } else {
        cout << "ERROR: No Arguments expected for exit" << endl;
      }
  }
    
}

int main(int argv, char** argc) {

  string userInput;

  while (true) {
    
    cout << WORKING_DIRECTORY << get_working_directory() << ESC << " " << PROMPT << " ";
    
    userInput = getInput(userInput);
    
    vector<string> s = parse_commands(userInput);
    if(s.size() > 0){
      check_built_in(s);
    }
      
  }

  return 0;
}
