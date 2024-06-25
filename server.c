#include <sys/socket.h> 
#include <string.h>       
#include <fcntl.h>        
#include <sys/sendfile.h> 
#include <unistd.h>     
#include <netinet/in.h>   

void main() {
    int s = socket(AF_INET, SOCK_STREAM, 0);  // Create a socket
    struct sockaddr_in addr = {               // Define the socket address
        AF_INET,                              // Address family (IPv4)
        0x901f,                               // Port number (0x901f in hex is8080 in decimal)
        0                                     // IP address (0 means any IP address)
    };

    bind(s, &addr, sizeof(addr));             // Bind the socket to the address

    listen(s, 10);                            // Listen for incoming connections, with a backlog of 10

    int client_fd = accept(s, 0, 0);          // Accept an incoming connection

    char buffer[256] = {0};                   // Create a buffer to hold the incoming request
    recv(client_fd, buffer, 256, 0);          // Receive data from the client

    // Parse the GET request (e.g., "GET /file.html .....")

    char* f = buffer + 5;                     // Skip "GET /" to get the file name
    *strchr(f, ' ') = 0;                      // Replace the space after the file name with a null terminator
    int opened_fd = open(f, O_RDONLY);        // Open the requested file for reading
    sendfile(client_fd, opened_fd, 0, 256);   // Send the file to the client
    close(opened_fd);                         // Close the file
    close(client_fd);                         // Close the client socket
    close(s);                                 // Close the server socket
}

