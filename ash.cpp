/*
  * James Ostmann & Joshua Govan
  * COMP 350 / 001
  * Sep, 26th 2020
  * Ash Shell Project Checkpoint 2
*/
#include <iostream>
#include <string>
#include <stdio.h>
#include <cctype>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <vector>
#include <stdlib.h>
#include <sys/wait.h>

// styles for prompt
#define ESC "\e[0m"
#define AUTHOR "\e[40;38;5;82;4m"
#define ESC_AUTHOR "\e[0;24m"
#define CARROT ">"
#define PROMPT "\e[33;1ma\e[0m\e[34;1ms\e[0m\e[33;1mh\e[0m"
#define WORKING_DIRECTORY "\e[92m"
#define PATH "\e[93m"
using namespace std;

// error message
char error_message[30] = "An error has occurred\n"; 
string currentPath = "";

// parses the current userInput and adds each space separated value to a vector
vector<string> parseCommands(string input) {
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
string getWorkingDirectory() {
  char * directory = NULL;
  return string(getcwd(directory,500));
}

// changes the working directory
int changeDirectory(string userInput) {
    int rc = chdir(userInput.c_str());
    return rc;
}

// changes the current path
void changePath(vector<string> commands) {
  currentPath.erase(currentPath.begin(),currentPath.end());
  for(int i = 1; i < commands.size(); i++) {
    currentPath += commands.at(i) + " ";
  }
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
  write(STDERR_FILENO, error_message, strlen(error_message));
}

// TODO: implement parser for individual commmands
void parse_Commands(string userInput){
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

// checks for a built in command and executes it if it is and returns true. otherwise it just retruns false
bool checkBuiltIn(vector<string> commands) {

  bool executed = true;

  string temp_command = commands.at(0);
    
   // handles build in cd command 
  if(temp_command.compare("cd") == 0) {
    if(commands.size() > 1 && commands.size() < 3) {
      int rc = changeDirectory(commands.at(1));
      if(rc != 0) {
        error();
      }
    } else {
      error();
    }

  // hndles built in path command
  } else if(temp_command.compare("path") == 0) {
    changePath(commands);
    
  // handles built in exit command
  } else if(temp_command.compare("exit") == 0) {
    if(commands.size() < 2) {
      exit();
    } else {
      error();
    }

  // handles not a built in command and return false
  } else {
    return false;
  }
  // was a built in command so return true;
  return true;
}


// gets commands from input vector and puts them in arguments array for execv()
void setArguments(char *arguments[], vector<string> commands) {
  for(int i = 1; i < commands.size(); i++) {
    arguments[i] = (char*)commands.at(i).c_str();
  }
}

//TODO: implement checking to see if command exists in path
bool verifyPathCommand(string command) {
  return true;
}

//TODO: implement checking to see if ">" is in commands for redirection 
bool redirectionEnabled(vector<string> commands) {
  return true;
}

// TODO: finish implementing executeOther for execv() commands
void executeOther(vector<string> commands) {
  pid_t child_pid;
  int status;
  
  child_pid = fork();
  char* arguments[commands.size() + 1];

  // add path to command
  string test = "/bin/";
  test += (char*)commands.at(0).c_str();
  arguments[0] = (char*)test.c_str();
  arguments[commands.size()] = NULL;
  setArguments(arguments, commands);

  // if child process execute command
  if(child_pid == 0) {
    execv(arguments[0],arguments);
    exit(0);
  } else {
    waitpid(child_pid, &status, 0);
  }
}

int main(int argv, char** argc) {

  string userInput;

  // main loop for program
  while (true) {
    
    cout << PROMPT << " " << PATH << currentPath << ESC << CARROT;
    userInput = getInput(userInput);
    
    // commands from userInput
    vector<string> commands = parseCommands(userInput);
    if(commands.size() > 0){
      bool builtIn = checkBuiltIn(commands);
      if(!builtIn) {
        executeOther(commands);
      }
    }
  }
  return 0;
}