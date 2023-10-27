CC = g++ -std=c++11 -Wall -Wextra -g

client: client.o helpers.o buffer.o requests.o
	$(CC) $^ -o $@

client.o: client.cpp
	$(CC) $(CFLAGS) $< -c

helpers.o: helpers.cpp
	$(CC) $(CFLAGS) $< -c

buffer.o: buffer.cpp
	$(CC) $(CFLAGS) $< -c

requests.o: requests.cpp
	$(CC) $(CFLAGS) $< -c

run: client
	./client

clean:
	rm -f *.o client