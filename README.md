Radulescu Matei

Web Client implemented with rest API:

This implementation of a web_client uses auxiliary files for:
Communication with the server included in helpers.cpp, operations on the buffer included in buffer.cpp.

For json files the nlohmann library has been used(only the json file from it), for convinience
(using json = nlohmann::json;) it's name is set to json. The :: parse method is used to parse the objects
received from the server into json array's or normal jsons respectively.

The requests are implemented as follows:
DELETE : On the first line it contains the name of the method which is DELETE, the url where it shoud go
and the version of HTTP being used, on the second line it contains the host, librarythan a cookie if one has been
provided in thid case it will contain the details of the current_user of the library(that is on the server), and
a JWT token if one has been provided, after witch a final empty line.

GET: Is exactly the same as DELETE other than the method name used is in this case of course GET

POST: The first two lines are of course the same, with the exeception of the name of the method being POST, next
the type of the content that will be posted is on one line, than the size of the content, after that there is
and empty line and after that the content itself is added in this case as a json.

This requests are used to send the following commands to the server, the commands are imputed from STDIN
and the valid one's are as follows :

For both login and register a json object is used to send the credentials.

register: Expects two more inputs in the form of username and password, it sends a post request to the appropriate
URL, and no user exists already under the given username it registers the user on the server, in the other case
it returns an error message.

login: Also expects two more inputs in the form of username and password, it sends a post request to a login URL
this time, and if the credentials are correct it logs the user in and remembers it's data as a cookie to be used
later, otherwise it returns an error message.

enter_library: If the saved cookie is still empty it returns an error as it can't acces with no logged in user,
otherwise it sends a get request with the appropriate cookie saved in it, if successfull it returns a JWT token
which is saved in another variable for further used, otherwise it returns an error message.

get_books: It checks wether the token variable has been modified, if it is still emtpy it returns an error
as there either is no user or the user hasn't requested acces to the library yet. If there is a token it sends
a get request to the server with both the user's cookie and the valid token, if the response is an error
it returns an error message, otherwise it checks if an array was returned, if not that the library is emtpy
but the command did still succed, if not than it parses through the array and prints each book's id and title.

get_book: Very similar to the get_books case but if expects another parameter of id, also if nothing is
returned this time we can only know for sure that a book with the given id does not exist. 

add_book: Expects several parameters : title, author, genre, publisher, page_count, it checks if the
token field is not empty, and returns an error if the uses doesn't have access to the library or is not
logged in ( in both cases it will be empty). Than it verifies if the given parameters are all correct:
none of them is empty and page_count is a number. After that it sends a post request to the library
with the new book as a json object, the cookie set to the current user's and the token to the valid one
obtained when access was granted to the user. If an error is received than the book could not be added,
otherwise the operation was successfull, both have messages pritned signaling the outcome.

delete_book: This is exactly as get_book, but the request has the delete method instead of the get method,
also as it does not return anything if the id is invalid it simply will be an treated as an error in the
given response from the server so there is a case for that.

logout: If there is no user logged in, it returns an error case messsaged, otherwise it sends a get request
to the logout url with both the cookie and token set, the token is not needed her so even if it is empty
it won't affect the outcome. If an error was received than the logout has failed, otherwise it was successfull
and the token and cookie for the user are set to an empty string again, so the checks for other commands will not
pass as no user is now logged in.

exit: closes the program

The MAKEFILE in the archive has a run rule which will automatically compile the sources and start the program, where
input can be manually typed into STDIN to use these commands.
