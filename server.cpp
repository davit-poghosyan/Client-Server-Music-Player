#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>

#define PORT 8080
#define BUFFER_SIZE 1024

std::string musicDirectory = "music/";

std::string getMp3List() {
    std::stringstream mp3List;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(musicDirectory.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            std::string filename = ent->d_name;
            if (filename.find(".mp3") != std::string::npos) {
                mp3List << filename << std::endl;
            }
        }
        closedir(dir);
    }
    return mp3List.str();
}

void sendMp3File(int clientSocket, const std::string& filename) {
    std::string filePath = musicDirectory + filename;
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }
    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    
    char* buffer = new char[BUFFER_SIZE];
    while (!file.eof()) {
        file.read(buffer, BUFFER_SIZE);
        send(clientSocket, buffer, file.gcount(), 0);
    }
    delete[] buffer;
}

int main() {
    int serverSocket, newSocket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    if (bind(serverSocket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    
    if (listen(serverSocket, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    
    while (true) {
        std::cout << "Server is listening..." << std::endl;
        
        if ((newSocket = accept(serverSocket, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }
        
        std::cout << "Connection accepted" << std::endl;
        
        std::string mp3List = getMp3List();
        std::cout << mp3List << std::endl;
        send(newSocket, mp3List.c_str(), mp3List.size(), 0);
        
        char buffer[BUFFER_SIZE] = {0};
        recv(newSocket, buffer, BUFFER_SIZE, 0);
        std::string requestedFile(buffer);
        
        sendMp3File(newSocket, requestedFile);
        
        close(newSocket);
    }
    
    return 0;
}
