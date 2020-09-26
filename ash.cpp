#include <iostream>
#include <string>
#include <cctype>
#include <sstream>
#include <cstdlib>
#include <cstdio>

#define ESC "\e[0m"
#define CUSTOM "\e[33;1ma\e[0m\e[34;1ms\e[0m\e[33;1mh\e[0m> "

using namespace std;

void exit() {
    cout << "Thank you for using our aggie shell" << endl;
    exit(0);
}

string readInput() {
    stringstream buffer;
    bool spaceToogle = false;
    while (true) {
        char c = cin.get();
        
        if (c == '\n') break;
        
        if (!isspace(c) ||(isspace(c) && !spaceToogle))
            buffer << c;
        if (isspace(c))
            spaceToogle = true;
        else
            spaceToogle = false;
    }
    
    return buffer.str();
}

int main() {
    
    string userInput;
    
    while (true) {
        cout << CUSTOM;
        
        userInput = readInput();
        
        if(userInput == "exit") {
            exit();
        }
        
    }
    return 0;
}
