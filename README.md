# Client-Server-Music-Player

This project implements a simple client-server music player using C++ sockets. The client sends the name of an MP3 file to the server, which then sends the corresponding file back to the client. The client then plays the received MP3 file.


## Prerequisites:

1. C++ compiler supporting C++11 or later.
2. Linux environment (for system calls).
3. Libraries: ao and mpg123 for audio playback.

## Setup:

1. Clone the repository or download the source files client.cpp and server.cpp.


2. Compile the client and server programs separately using the C++ compiler. For example:

       g++ -o client client.cpp -lao -lmpg123
       
       g++ -o server server.cpp

    
Ensure that the required libraries (ao and mpg123) are installed and accessible in your system.


## Usage:

Start the server by running the compiled server executable.

    ./server

Once the server is running, start the client by running the compiled client executable.

    ./client

The client prompts you to enter the name of the MP3 file you want to download and play.
Enter the filename (make sure it exists in the music/ directory on the server).

The client sends the filename to the server, which then sends the MP3 file back to the client.

The client receives the MP3 file and plays it using the play() function.


## Directory Structure:

client.cpp: Contains the client-side code for sending the MP3 filename to the server and playing the received file.

server.cpp: Contains the server-side code for receiving the MP3 filename from the client, 
            sending the corresponding file, and serving the client requests.

music/: Directory containing the MP3 files. Ensure that the server has the required MP3 files in this directory.

## Notes:

The server listens on port 8080 by default. Ensure that this port is not blocked by any firewall.

Make sure to have the ao and mpg123 libraries installed in your system and linked during compilation.

This project assumes a local server-client setup. For a remote setup, modify the IP address in the client code accordingly.

Ensure that the MP3 files are present in the music/ directory on the server and are accessible.

This is a basic implementation and might require further enhancements for robustness and additional features.
