#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <csignal> 
#include <unistd.h>
#include <ao/ao.h>
#include <mpg123.h>


#define BITS 8

#define PORT 8080
#define BUFFER_SIZE 1024

void play(std::string& filename) {
    mpg123_handle *mh;
    unsigned char *buffer;
    size_t buffer_size;
    size_t done;
    int err;

    int driver;
    ao_device *dev;

    ao_sample_format format;
    int channels, encoding;
    long rate;

    ao_initialize();
    driver = ao_default_driver_id();
    std::cout << "Driver: " << driver << std::endl;
    mpg123_init();
    mh = mpg123_new(NULL, &err);
    buffer_size = mpg123_outblock(mh);
    buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));

    mpg123_open(mh, filename.c_str());
    mpg123_getformat(mh, &rate, &channels, &encoding);

    format.bits = mpg123_encsize(encoding) * BITS;
    format.rate = rate;
    format.channels = channels;
    format.byte_format = AO_FMT_NATIVE;
    format.matrix = 0;
    dev = ao_open_live(driver, &format, NULL);

    std::cout << "Playing: " << filename << std::endl;
    int paused = 0;

    while (mpg123_read(mh, (char*)buffer, buffer_size, &done) == MPG123_OK) {
        ao_play(dev, (char*)buffer, done);
    }

    free(buffer);
    ao_close(dev);
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
    ao_shutdown();
}


int main() {
    int clientSocket;
    struct sockaddr_in serverAddress;
    char buffer[BUFFER_SIZE] = {0};
    
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }
    
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }
    
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return -1;
    }
    
    std::string filename;
    std::cout << "Enter the name of the MP3 file you want to download: ";
    std::cin >> filename;
    
    send(clientSocket, filename.c_str(), filename.size(), 0);
    
    std::ofstream outputFile(filename, std::ios::binary);
    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to create file " << filename << std::endl;
        return -1;
    }
    
    int i = 0;

    int bytesReceived;
    while ((bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0)) > 0) {
        outputFile.write(buffer, bytesReceived);
    }
    
    if (bytesReceived < 0) {
        std::cerr << "Receive failed" << std::endl;
    }
    
    std::cout << "File received successfully" << std::endl;
    
    play(filename);
    
    return 0;
}
