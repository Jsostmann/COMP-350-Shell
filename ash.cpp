/*
  * James Ostmann & Joshua Govan
  * COMP 350 / 001
  * Nov, 24th 2020
  * Ash Shell Project Final Checkpoint
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
#include <fcntl.h>
#include <fstream>



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
  return string(getcwd(directory,500)) + " ";
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

// checks if batch file is enabled by checking to see if 1 batch file argument is passed to the program
bool batchEnabled(int argSize) {
  return argSize == 2;
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

//TODO: implement checking to see if command exists in path
bool verifyPathCommand(string command) {
  return true;
}

//TODO: implement checking to see if ">" is in commands for redirection 
int redirectionEnabled(vector<string> commands) {
  int redirectionCount = 0;
  for(int i = 0; i < commands.size(); i++) {
    if(commands.at(i).compare(">") == 0) {
      redirectionCount += 1;
    }
  }
  return redirectionCount;
}

// handles execution of commands not build in including redirection
void executeOther(vector<string> commands) {
    
  pid_t child_pid;
  int outputFile;
  string outputFileName = "";

  int redirectionFound = redirectionEnabled(commands);
  
  // if more than 1 redirection symbols found, through an error
  if(redirectionFound > 1) {
    error();
  }
  
  // if only 1 redirection symbol found and last command is a string
  if(redirectionFound == 1 && commands.back().compare(">") != 0) {
    outputFileName = commands.back();
    commands.pop_back();
    commands.pop_back();
    outputFile = open(outputFileName.c_str(), O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
  }
    
  // prepare arguments for execvp() command
  int argSize = commands.size();
  char* arguments[argSize];
  for(int i = 0; i < argSize; i++) {
    arguments[i] = (char*)commands.at(i).c_str();
  }
  arguments[argSize] = NULL;
  
  // create child process
  child_pid = fork();

  // if error print error
  if(child_pid == -1) {
    error();
      
  // if child process execute command
  } else if (child_pid == 0) {
    // if redirection enabled change stdout and stderr to point to outputfile name
    if(redirectionFound == 1) {
      dup2(outputFile, 1);
      dup2(outputFile, 2);
      close(outputFile);
    }
    if(execvp(arguments[0],arguments) == -1) {
      error();
      exit(0);
    }
      
  // if parent process, wait for child to finish
  } else if(child_pid > 0) {
    if(wait(0) == -1) {
      error();
    }
  }
}

int main(int argv, char** argc) {

  string userInput;
  ifstream batchInput;
    
  // check if batch file reading is enabled
  if(batchEnabled(argv)) {
    
    batchInput.open((char*)argc[1]);
    
    if(!batchInput) {
      error();
      exit(0);
    }

    cin.rdbuf(batchInput.rdbuf());
  }

  // main loop for program
  while (true) {
    
    // only print out prompt if in interactive mode
    if(!batchEnabled(argv)) {
      cout << PROMPT << " " << WORKING_DIRECTORY << getWorkingDirectory() << ESC << CARROT;
    }
    
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
