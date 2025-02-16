#include <iostream>
#include "func.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <thread>

using namespace std;

const int PORT = 8080;
const int BUFFER_SIZE = 1024;

void server() {
	cout << "Server started" << endl;
	int sockfd;
	char buffer[BUFFER_SIZE];
	struct sockaddr_in server_addr, client_addr;
	socklen_t addr_len = sizeof(client_addr);

	// Create socket
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		cout << "Socket creation failed" << endl;
		exit(EXIT_FAILURE);
	}

	memset(&server_addr, 0, sizeof(server_addr));
	memset(&client_addr, 0, sizeof(client_addr));

	// Fill server information
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	// Bind the socket with the server address
	if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		cout << "Bind failed" << endl;
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	std::cout << "Server is running, waiting for messages..." << std::endl;
	this_thread::sleep_for(chrono::seconds(1));
	cout << "Back from waiting" << endl;

	// Receive message from client
	int n = recvfrom(sockfd, buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&client_addr, &addr_len);
	buffer[n] = '\0';
	std::cout << "Client: " << buffer << std::endl;

	// Send acknowledgment to client
	const char *ack = "Message received";
	sendto(sockfd, ack, strlen(ack), MSG_CONFIRM, (const struct sockaddr *)&client_addr, addr_len);

	close(sockfd);
}

void client() {
	cout << "Client started" << endl;
	const char* SERVER_IP = "127.0.0.1";
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr;

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        cout << "Socket creation failed" << endl;
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    // Fill server information
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Send message to server
    const char *message = "Hello, server!";
    sendto(sockfd, message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&server_addr, sizeof(server_addr));

    // Receive acknowledgment from server
    int n = recvfrom(sockfd, buffer, BUFFER_SIZE, MSG_WAITALL, NULL, NULL);
    buffer[n] = '\0';
    std::cout << "Server: " << buffer << std::endl;

    close(sockfd);
}

void a() {
	cout << "Starting thread a" << endl;
	this_thread::sleep_for(chrono::seconds(2));
	cout << "Ending thread a" << endl;
}

void b() {
	cout << "Starting thread b" << endl;
	this_thread::sleep_for(chrono::seconds(1));
	cout << "Ending thread b" << endl;
}

int main() {
	cout << "Hello world!" << endl;

	array<float,3> v { 1.f, 2.f, 3.f};
	auto result = calc_vec(v);

	cout << "Distance of [" << v[0] << "," << v[1] << "," << v[2] << "] is " << result << endl;

	auto t1 = thread {server};
	cout << "Launched server" << endl;
	auto t2 = thread {client};
	cout << "Launched client" << endl;
	t2.join();
	t1.join();

	return 0;
}