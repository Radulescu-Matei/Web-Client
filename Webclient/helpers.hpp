#ifndef _HELPER_
#define _HELPER_

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <iostream>
#include <string.h>
#include <string>
#include "nlohmann/json.hpp"
#include "buffer.hpp"
using namespace std;



using json = nlohmann::json;

#define BUFLEN 4096
#define LINELEN 1000

typedef struct {
    string username;
    string password;
}user;


typedef struct {
    string title;
    string author;
    string genre;
    string publisher;
    string page_count;
}book;


// shows the current error
void error(const char *msg);

// adds a line to a string message
void compute_message(char *message, const char *line);

// opens a connection with server host_ip on port portno, returns a socket
int open_connection(char *host_ip, int portno, int ip_type, int socket_type, int flag);

// closes a server connection on socket sockfd
void close_connection(int sockfd);

// send a message to a server
void send_to_server(int sockfd, char *message);

// receives and returns the message from a server
char *receive_from_server(int sockfd);

// extracts and returns a JSON from a server response
char *basic_extract_json_response(char *str);



#endif
