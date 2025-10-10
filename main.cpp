//
// Created by Liam on 10/10/2025.
//

#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/socket.h> // This is for the networking
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h> //
// argc == 1 + the number of arguments passed
// argv == the arguments passed in order

int setupServer(int port);
int setupClient(int port, std::string addr);

int main(int argc, char *argv[]) {

    if (argc > 1) {
	printf("Passed an argument\n");
    } else {
	fprintf(stderr, "No arguments passed\n");
	return -1;
    }

    int port;
    if (argc > 1 && !strcmp(argv[1], "client")) {
	port = std::stoi(argv[2]);	
    } else {
	try {
	    port = std::stoi(argv[1]);
	} catch (std::invalid_argument const& ex) {
	    std::cout << ex.what() << ", argument passed does not match format of port" << std::endl;
	    return -1;
	} catch (std::out_of_range const& ex) {
	    std::cout << ex.what() << ", argument passed was out of range" << std::endl;
	}
    }

    if (argc > 1 && !strcmp(argv[1], "client")) {
	printf("client\n");
	setupClient(port, "127.0.0.1");
    } else {
	setupServer(port);
    }
}

int setupServer(int port) {
	// set up the network interface using <sys/socket.h>
	int sockfd, newSockfd;
	struct sockaddr_in serverAddr, clientAddr;
	socklen_t clientAddrLen;

	// Create the socket
	// AF_INET is an address family that is used to designate the type of addresses that your socket
	// can communicate with (in this case Internet Protocol v4 Address, or IPv4)
	// There are other such address families that support UNIX sockets, IPX sockets as well as Bluetooth
	// https://stackoverflow.com/questions/1593946/what-is-af-inet-in-socket-programming
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	// This also sets up the socket for streaming TCP data with SOCK_STREAM, and the protocol value 0
	// to let the system choose the appropriate protocol


	if (sockfd == -1) {
		std::cout << "Failed to create the socket" << std::endl;
		return -1;
	}

	std::cout << "Socket created successfully" << std::endl;

	// After creating a socket, we need to bind it to a port. This is where
	// the verification that we did earlier is useful

	// This sets up the server's address
	serverAddr.sin_family = AF_INET; // Same as before, set up the server's address family to be IPv4
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Since my computer uses Little Endian values and the server will use Big Endian values, there is a need to switch them over. htonl - lit. host to network long
	serverAddr.sin_port = htons(port); // host to network short
	// TODO: WRITE ABOUT ENDIANNESS AND WHY IT IS IMPORTANT
	// use this https://www.lenovo.com/gb/en/glossary/endian

	// now we need to bind the socket to the address
	int bindResult = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

	if (bindResult == -1) {
		std::cout << "Failed to bind the socket" << std::endl;
		return -1;
	}

	std::cout << (struct sockaddr*)&serverAddr << std::endl;


	// make the socket listen for incoming connections
	int listenResult = listen(sockfd, 10);

	if (listenResult == -1) {
		std::cout << "Failed to listen for connections";
	}


	newSockfd = accept(sockfd, (struct sockaddr*)&clientAddr, &clientAddrLen);
	if (newSockfd == -1) {
		std::cout << "Failed to accept connection from client" << std::endl;
		return -1;
	}

	std::cout << "Accepted connection from client";

	// Receiving data from the client
	const int bufferSize = 1024;
	char buffer[bufferSize];
	int bytesReceived = recv(newSockfd, buffer, bufferSize, 0);
	while (bytesReceived != 0) {
		buffer[bytesReceived] = '\0';
		bytesReceived = recv(newSockfd, buffer, bufferSize, 0);
		std::cout << "Received data: " << buffer << std::endl;
		std::cout << bytesReceived << std::endl;
	}

	if (bytesReceived == -1) {
		std::cout << "Error receiving data" << std::endl;
	}

	if (bytesReceived == 0) {
		std::cout << "Connection closed" << std::endl;
	}

	std::cout << "Received data: " << buffer << std::endl;

    close(sockfd);

    return 0;
}

int setupClient(int port, std::string addr) {

    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
		std::cout << "Failed to create socket" << std::endl;
		return -1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port); // This is the port of the server
    serverAddr.sin_addr.s_addr = inet_addr(addr.c_str()); // This is the IP address of the server

    int status = connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if (status == -1) {
		std::cout << "Failed to connect to the server at address " << addr << std::endl;
		return -1;
    }

    // send the data
	std::string data;
    while (true) {
		std::cin >> data;
		status = send(sockfd, data.c_str(), data.size(), 0);
		if (status == -1) {
		    std::cout << "Failed to send data" << std::endl;
		    return -1;
		}

		std::cout << "Data send successfully!";
    }
    // Close the socket
    close(sockfd);

    return 0;
}

















