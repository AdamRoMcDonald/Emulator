#include <iostream>
#include <unistd.h>   // For fork(), exec(), chdir()
#include <sys/wait.h> // For wait()
#include <dirent.h>   // For opendir(), readdir()
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>    // For strcmp(), strtok()
#include <fstream>    // For viewing files

void CD(const char* filePath) {
    if (chdir(filePath) != 0) {
        perror("chdir() error");
    }
}

void LS() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        std::cout << cwd << std::endl;

        DIR* curDir = opendir(cwd);
        if (curDir == nullptr) {
            perror("opendir() error");
            return;
        }

        struct dirent* FILE;
        while ((FILE = readdir(curDir)) != nullptr) {
            std::cout << FILE->d_name << std::endl;
        }
        closedir(curDir);
    } else {
        perror("getcwd() error");
    }
}

void PS() {
    if (fork() == 0) {
        execl("/bin/ps", "ps", "-e", nullptr);
        perror("execl() error");
        exit(1);
    } else {
        wait(nullptr);
    }
}

void VI() {
    if (fork() == 0) {
        execl("/usr/bin/vi", "vi", nullptr);
        perror("execl() error");
        exit(1);
    } else {
        wait(nullptr);
    }
}

void CAT(const char* filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: File not found or could not be opened!" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }
    file.close();
}

void RM(const char* filename) {
    if (remove(filename) != 0) {
        perror("Error deleting file");
    } else {
        std::cout << "File deleted successfully!" << std::endl;
    }
}

int main() {
    char input[1024];
    char* args[64];

    while (true) {
        // print prompt
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        std::cout << cwd << " $ ";

        // read input hopefull
        std::cin.getline(input, sizeof(input));

        char* token = strtok(input, " ");
        int i = 0;
        while (token != nullptr) {
            args[i++] = token;
            token = strtok(nullptr, " ");
        }
        args[i] = nullptr;

        // Handle commands
        if (strcmp(args[0], "cd") == 0) {
            CD(args[1]);
        } else if (strcmp(args[0], "ls") == 0) {
            LS();
        } else if (strcmp(args[0], "ps") == 0) {
            PS();
        } else if (strcmp(args[0], "vi") == 0) {
            VI();
        } else if (strcmp(args[0], "cat") == 0) {
            CAT(args[1]);
        } else if (strcmp(args[0], "rm") == 0) {
            RM(args[1]);
        } else if (strcmp(args[0], "exit") == 0) {
            break;
        } else {
            std::cerr << "Unknown command: " << args[0] << std::endl;
        }
    }

    return 0;
}
