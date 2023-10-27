#include "helpers.hpp"
#include "requests.hpp"

#define HOST "34.254.242.81"

#define PORT 8080

int main()
{
    char *message;
    char *response;
    int sockfd;

    // Execute is used read the next command from STDIN
    char *execute = (char *)malloc(100 * sizeof(char));

    // Logged_in used to check if a user is logged_in and
    // save it's cookie
    string logged_in = "";
    // JWT used to save JWT token and check if we have acces
    // to the library
    string JWT = "";

    cin >> execute;

// Executing commands until exit is read
    while (strcmp(execute, "exit") != 0)
    {
        // Opens connection to the host trhough the required port thhroughh the
        // sockfd socket
        sockfd = open_connection((char *)HOST, PORT, PF_INET, SOCK_STREAM, 0);

        if (strcmp(execute, "register") == 0)
        {
            // Struct user used for stroing credentials for a user
            user new_user;

            // As username and password's cannot contain spaces they
            // can simply be read from STDIN
            cout << "username=";
            cin >> new_user.username;
            cout << "password=";
            cin >> new_user.password;

            // json created with nlohman and required data
            json reg = {
                {"username", new_user.username},
                {"password", new_user.password}};

            // Post created with the contents of the json and
            // json as data, sent to the given url for register
            message = compute_post_request((char *)HOST,
                                           (char *)"/api/v1/tema/auth/register",
                                           (char *)"application/json",
                                           (char *)reg.dump().c_str(), NULL);

            // Generateed post request is sent to the server and a response
            // is received in return
            send_to_server(sockfd, message);

            response = receive_from_server(sockfd);

            if (strstr(response, "error") == NULL)
            {
                // If no error was received, the user has been registered
                // successfully
                cout << "200 - OK - User registered successfully\n";
            }
            else
            {
                // If an error has been received that the username is already taken
                cout << "The username " << new_user.username << " is already taken!\n";
            }
        }
        else if (strcmp(execute, "login") == 0)
        {
            // Struct used for reading same as register
            user curr_user;

            cout << "username=";
            cin >> curr_user.username;
            cout << "password=";
            cin >> curr_user.password;

            // Json also created the same way
            json log = {
                {"username", curr_user.username},
                {"password", curr_user.password}};

            // A post request is sent identical to the register one
            // but to the login url this time
            message = compute_post_request((char *)HOST,
                                           (char *)"/api/v1/tema/auth/login",
                                           (char *)"application/json",
                                           (char *)log.dump().c_str(), NULL);

            // The request is sent to the server and a reseponse is received
            send_to_server(sockfd, message);

            response = receive_from_server(sockfd);

            if (strstr(response, "error") == NULL)
            {
                // If no error was received that the login was successfull
                // and the cookie of the user is saved in  the previously
                // mentioned variable
                string aux(response);
                // As Path is the next parameter received in the response
                // we read untill we find it and removed the last 2
                // charachters "; "
                logged_in = aux.substr(aux.find("Cookie: "),
                                       aux.find("Path") - aux.find("Cookie: ") - 2);

                cout << "200 - OK - Welcome!\n";
            }
            else
            {
                // If an error was received that the login attempt
                // was incorrect
                cout << "Invalid username or password!\n";
            }
        }
        else if (strcmp(execute, "enter_library") == 0)
        {
            if (logged_in.compare("") == 0)
            {
                // If logged_in remained empty the library can't
                // be accessed as no user is logged in
                cout << "No user is logged in!\n";
            }
            else
            {

                // Otherwise a get request is generated with the
                // given acces url, and logged_in cookie sent as
                // the cookie
                message = compute_get_request((char *)HOST,
                                              (char *)"/api/v1/tema/library/access",
                                              (char *)logged_in.c_str(), NULL);

                // The request is sent to the server and a response is received
                send_to_server(sockfd, message);

                response = receive_from_server(sockfd);

                if (strstr(response, "error") == NULL)
                {
                    // If no error is received than the token
                    // is saved in JWT
                    // As the toekn is received in {}, we find the }
                    // charachter and remove 8 chars representing ("token":")
                    // the number of chars to be saved also has 1 more removed
                    // representing the final "
                    // Also as per the format Authorization: Bearer is added
                    // before the token
                    string aux(response);
                    JWT = "Authorization: Bearer " +
                          aux.substr(aux.find("token") + 8,
                                     aux.find("}") - aux.find("token") - 9);

                    cout << "200 - OK - Access granted!\n";
                }
                else
                {
                    // If an error has been received thant acces has been denied
                    cout << "Failed to access library!\n";
                }
            }
        }
        else if (strcmp(execute, "get_books") == 0)
        {
            if (JWT.compare("") == 0)
            {
                // If the token is still empty thant the current user
                // does not have access to the library
                cout << "Access to the library has not been granted yet!\n";
            }
            else
            {
                // Generates get request with the current user's cookie
                // and the token that was received, and the library/books
                // path
                message = compute_get_request((char *)HOST,
                                              (char *)"/api/v1/tema/library/books",
                                              (char *)logged_in.c_str(), (char *)JWT.c_str());

                // The request is sent to the server and a message is received
                send_to_server(sockfd, message);

                response = receive_from_server(sockfd);

                if (strstr(response, "error") == NULL)
                {
                    // If no error was received than we check if a list
                    // was received if it was not that the request was
                    // successfull but there are no books in the library
                    string aux(response);
                    if (aux.find("[{") == string::npos)
                    {
                        cout << "200 - OK - There are no books in the library!\n";
                    }
                    else
                    {
                        // If there are books we use ntohmann parse to iterate it into a json
                        // and print each books id and title
                        json books = json::parse(aux.substr(aux.find("[{"), aux.find("}]") - aux.find("[{") + 2));

                        cout << "200 - OK - Showing books :\n";

                        for (int i = 0; i < (int)books.size(); i++)
                        {
                            cout << "id= " << books[i]["id"] << "," << endl;
                            cout << "title= " << books[i]["title"] << endl;
                            cout << endl;
                        }
                    }
                }
                else
                {
                    // If an error has occured than the books could not be retrieved
                    cout << "Could not get books!\n";
                }
            }
        }
        else if (strcmp(execute, "get_book") == 0)
        {
            string id;

            // Similar to get_book but expects id as an input
            cout << "id=";
            cin >> id;
            if (JWT.compare("") == 0)
            {
                // Token verification
                cout << "Access to the library has not been granted yet!\n";
            }
            else
            {

                // id is added after the usual url so only one booked is received
                string merge = "/api/v1/tema/library/books/" + id;
                message = compute_get_request((char *)HOST,
                                              (char *)merge.c_str(),
                                              (char *)logged_in.c_str(), (char *)JWT.c_str());

                // Exact same logic as get_books but only one book is printed
                // in the non error case
                send_to_server(sockfd, message);

                response = receive_from_server(sockfd);

                if (strstr(response, "error") == NULL)
                {
                    // If nothign was received than the given id is invalid
                    string aux(response);
                    if (aux.find("{") == string::npos)
                    {
                        cout << "Book not found!\n";
                    }
                    else
                    {
                        json book = json::parse(aux.substr(aux.find("{"), aux.find("}") - aux.find("{") + 1));
                        cout << "200 - OK - Showing book :\n";
                        cout << "id= " << book["id"] << endl;
                        cout << "title= " << book["title"] << endl;
                        cout << "author= " << book["author"] << "," << endl;
                        cout << "genre= " << book["genre"] << "," << endl;
                        cout << "publisher= " << book["publisher"] << "," << endl;
                        cout << "page_count= " << book["page_count"] << endl;
                    }
                }
                else
                {
                    cout << "Could not get book!\n";
                }
            }
        }
        else if (strcmp(execute, "add_book") == 0)
        {
            // Stuct used to read book
            book new_book;

            // They are read as getline as most can have spaces
            // skip is used to read the endline after add_books
            string skip;
            getline(cin, skip);

            cout << "title:";
            getline(cin, new_book.title);

            cout << "author:";
            getline(cin, new_book.author);

            cout << "genre:";
            getline(cin, new_book.genre);

            cout << "publisher:";
            getline(cin, new_book.publisher);

            cout << "page_count:";
            getline(cin, new_book.page_count);

            if (JWT.compare("") == 0)
            {
                // Token validation
                cout << "Access to the library has not been granted yet!\n";
            }
            else
            {
                // If the page_count doesn't contatin only decimals or any of the other
                // parameters are empty the given input is wrong.
                if ((new_book.page_count.find_first_not_of("0123456789") != string::npos) ||
                    new_book.title.empty() || new_book.author.empty() ||
                    new_book.genre.empty() || new_book.publisher.empty())
                {
                    cout << "Wrong input\n";
                }
                else
                {
                    // json created to send the book to the server
                    json add = {
                        {"title", new_book.title},
                        {"author", new_book.author},
                        {"genre", new_book.genre},
                        {"publisher", new_book.publisher},
                        {"page_count", stoi(new_book.page_count)}

                    };

                    // Post request generated with books url and the valid token
                    message = compute_post_request((char *)HOST,
                                                   (char *)"/api/v1/tema/library/books",
                                                   (char *)"application/json",
                                                   (char *)add.dump().c_str(), (char *)JWT.c_str());

                    send_to_server(sockfd, message);

                    response = receive_from_server(sockfd);

                    if (strstr(response, "error") == NULL)
                    {
                        // If no error was received than the book was added
                        cout << "200 - OK - Added the book!\n";
                    }
                    else
                    {
                        // Error case
                        cout << "Could not add book!\n";
                    }
                }
            }
        }
        else if (strcmp(execute, "delete_book") == 0)
        {
            string id;

            // Expects id input
            cout << "id=";
            cin >> id;
            if (JWT.compare("") == 0)
            {
                //Token authorization
                cout << "Access to the library has not been granted yet!\n";
            }
            else
            {
                // Exactly same as get_book but sends delete instead of get request
                string merge = "/api/v1/tema/library/books/" + id;
                message = compute_delete_request((char *)HOST,
                                                 (char *)merge.c_str(),
                                                 (char *)logged_in.c_str(), (char *)JWT.c_str());

                send_to_server(sockfd, message);

                response = receive_from_server(sockfd);
                cout << response;

                if (strstr(response, "error") == NULL)
                {
                    // If no error is receved the book was deleted
                    cout << "200 - OK - Book with id " << id << " has been deleted!\n";
                }
                else
                {
                    // Most likely invalid id if a book could not be deleted
                    cout << "Could not delete book!\n";
                }
            }
        }
        else if (strcmp(execute, "logout") == 0)
        {
            if (logged_in.compare("") == 0)
            {
                // Error case if no user is logged in
                cout << "No user is logged in!\n";
            }
            else
            {
                // Get request sent to logout url with the correct cookie and token
                // token is not required here but it is in the get request formats
                // so  even if it is not input it willl not affect the request
                message = compute_get_request((char *)HOST,
                                              (char *)"/api/v1/tema/auth/logout",
                                              (char *)logged_in.c_str(), (char *)JWT.c_str());

                send_to_server(sockfd, message);
                response = receive_from_server(sockfd);

                if (strstr(response, "error") == NULL)
                {
                    // If  no error was received than the token and cookie
                    // are emptied, so the two checks won't pass for other
                    // commands
                    JWT = "";
                    logged_in = "";
                    cout << "200 - OK - User logged out!\n";
                }
                else
                {
                    // Error case
                    cout << "Failed to logout!\n";
                }
            }
        }

        else
        {
            // Any other command is invalid
            cout << "Invalid command please try agian!\n";
        }

        // Connection closed after each command to not slow down server
        close_connection(sockfd);
        // Reading next command from stdin
        cin >> execute;
    }

// Freeing char vectors and emptying cookies and token, not necessary as the
// program closes after, but no tokens should remain on the client in any way
    JWT = "";
    logged_in = "";
    free(message);
    free(response);

    return 0;
}