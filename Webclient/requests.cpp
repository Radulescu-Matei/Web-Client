#include <stdlib.h> /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.hpp"
#include "requests.hpp"

// Creates delete request in the form of a char*
char *compute_delete_request(char *host, char *url,
                             char *cookies, char *token)
{
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));
    // Adds DELETE method, path  and HTTP version
    sprintf(line, "DELETE %s HTTP/1.1", url);

    compute_message(message, line);

    // Adds Host
    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // Adds cookie if there is one
    if (cookies != NULL)
    {
        compute_message(message, cookies);
    }

    // Adds token if there is one
    if (token != NULL)
    {
        compute_message(message, token);
    }

    // Adds final endline
    compute_message(message, "");
    return message;
}

// Creates get request as a char *
// exactly the same as the delete request other than the method
char *compute_get_request(char *host, char *url,
                          char *cookies, char *token)
{
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));

    sprintf(line, "GET %s HTTP/1.1", url);

    compute_message(message, line);

    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    if (cookies != NULL)
    {
        compute_message(message, cookies);
    }

    if (token != NULL)
    {
        compute_message(message, token);
    }

    compute_message(message, "");
    return message;
}

// Creates post request as a char*
char *compute_post_request(char *host, char *url, char *content_type, char *body_data,
                           char *token)
{
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));

    // Adds post method, path and HTTP version
    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);

    // Adds token if there is one
    if (token != NULL)
        compute_message(message, token);

    // Adds host
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // Adds content_type in this case it uses jsons
    sprintf(line, "Content-Type: %s", content_type);
    compute_message(message, line);

    // Adds the length of data
    sprintf(line, "Content-Length: %zu", strlen(body_data));
    compute_message(message, line);

    // Adds end line
    compute_message(message, "");

    memset(line, 0, LINELEN);

    // Adds the data
    compute_message(message, body_data);
    free(line);
    return message;
}
