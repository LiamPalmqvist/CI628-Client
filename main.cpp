//
// Created by Liam on 10/10/2025.
//

#include <iostream>


// argc == 1 + the number of arguments passed
// argv == the arguments passed in order
int main(int argc, char *argv[]) {
    
    int port;

    if (argc > 1) {
	printf("Passed an argument\n");
    } else {
	fprintf(stderr, "No arguments passed\n");
	return -1;
    }

    if (argc > 1 && !strcmp(argv[1], "client")) {
	printf("client\n");
    }

    // try catch to validate that an IP address port has been entered
    try {
	// stoi tries to convert a string to an interger
	port = std::stoi(argv[1]);
    }
    catch (std::invalid_argument const& ex) { std::cout << ex.what() << ", argument passed does not match format of port" << std::endl;
	return -1;
    } catch(std::out_of_range const& ex) {
	std::cout << ex.what() << ", argument passed was out of range";
    }
    // validate that the value passed is <= 5 digits in length
    if (port <= 65535) {
	std::cout << port << std::endl;
    }

    return 1;
}
