#include <iostream>
#include <unistd.h>   
#include <sys/wait.h> 
#include <dirent.h>   
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>    
#include <fstream>  
#include <string.h>

//1. CD 




void CD(const char* filePath) {
    //In c, successful processes will often return 0
    //Here, if chdir (the c keyword/call to use Linux's "cd" command) is unsuccessful it will print an error message-
    // - just like a Linux shell would. 
    if (chdir(filePath) != 0) {
        //exact copy of Linux shell error message
        fprintf(stderr, "bash: cd: %s: No such file or directory\n", filePath);
    }
}

void LS() {
    //cwd means current working directory
    char cwd[512];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        std::cout << cwd << std::endl;
        //Here we check to see if our directory has any contents. 
        DIR* curDir = opendir(cwd);
        if (curDir == nullptr) {
            return;
        }
        //Here we read through the contents of our directory. 
        struct dirent* FILE;
        while ((FILE = readdir(curDir)) != nullptr) {
            std::cout << FILE->d_name << std::endl;
        }
        closedir(curDir);
    } 
}


void VI() {
    //Here we can see a new process being forked.
    //A child fork() will be equal to zero, indicating that it is a new process ready to run
    if (fork() == 0) {
        //Here, execl replaces the CURRENT process image with a new image specified by the path, thus opening VI.
        execl("/usr/bin/vi", "vi", nullptr);
        exit(1);
    } else {
        wait(nullptr);
    }
}


//Deleting file method

void RM(const char* fileName) {
    //Here we have another example of a successful process returning 0. 
    //"remove" is c's call to the rm command in Linux shell
    if (remove(fileName) != 0) {
        fprintf(stderr, "bash: cd: %s: No such file or directory\n", fileName);
    } 
}

//Viewing file method

void CAT(const char* fileName) {
    //ifstream is a type of streamable class which allows us to operate on or "stream" the contents of a file. 
    std::ifstream file(fileName);
    //If file is false then the file doesn't exist. 
    if (!file) {
        fprintf(stderr, "bash: cd: %s: No such file or directory\n", fileName);
        return;
    }
    
    std::string singleL;
    //Here this loop gets each line from the streamable file object, stores them temporarily in singleL and prints.
    //Cannot just print out the ifstream object because it will come back as a weird code, it's a class.
    while (std::getline(file, singleL)) {
        std::cout << singleL << std::endl;
    }
    //Closing file
    file.close();
}

//View running processes method

void PS() {
    if (fork() == 0) {
        //Here we again see a new child process replacing the current image with a new one found at the path.
        execl("/bin/ps","ps","-e", nullptr);
        exit(1);
    } else {wait(nullptr);}
}


//main driver method

int main() {
    char input[512];
    char* args[64];

    while (true) {
        // print prompt
        char cwd[512];
        getcwd(cwd, sizeof(cwd));
        //This prints out the current working directory, making it look like user is in a real shell. 
        std::cout << cwd << " $ ";

        // read input hopefully
        //.getline() stores the line in "input"
        std::cin.getline(input, sizeof(input));
          
        //strtok divides input up into substrings, dividing where there are spaces. In other words it "tokenizes"
        //So "cd test.txt" would break up "cd" and "test.txt" into substrings so they can be used.
        char* token = strtok(input, " ");
        int i = 0;
        
        //This here adds these substrings into args
        while (token != nullptr) {
            args[i++] = token;
            token = strtok(nullptr, " ");
        }
        args[i] = nullptr;

        // handle user input commands, tried to make it a switch but it wouldn't work, something about non-convertable types. 
        //strcmp compares two strings, if they are identical it returns 0
        if (strcmp(args[0], "cd") == 0) {
            //The syntax for a cd request is : "cd userInput." So I check the second location in args for the user input to give CD() method.
            CD(args[1]);
        } else if (strcmp(args[0], "ls") == 0) {
            LS();
        } else if (strcmp(args[0], "vi") == 0) {
            VI();
        } else if (strcmp(args[0], "ps") == 0) {
            PS();
        } else if (strcmp(args[0], "cat") == 0) {
            CAT(args[1]);
        } else if (strcmp(args[0], "rm") == 0) {
            RM(args[1]);
        //breaks from the while loop and the program then returns zero. Having it return 0 in the if statement might cause memory leak as while loop cuts off mid-function. 
        } else if (strcmp(args[0], "exit") == 0) {
            break;
        } else {
            std::cerr << args[0] << ": command not found " << std::endl;
        }
    }
    //Return zero, shut off. 
    return 0;
}
