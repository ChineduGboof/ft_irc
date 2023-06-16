# ft_irc
 Internet Relay Chat

## IRC Server Port Number
The maximum port number that can be used for an IRC server is `65,535`. Port numbers range from `0 to 65,535`, and ports below `1024` are reserved for well-known services and protocols.

In general, IRC servers commonly use port number `6667` for unencrypted connections and port number `6697` for encrypted connections using SSL/TLS. These ports are widely supported by IRC clients and servers.

## Understanding IP Addresses for IRC Servers: 0.0.0.0 and 127.0.0.1

The IP address 0.0.0.0 is used to indicate that a server should bind to all available network interfaces, allowing it to listen for incoming connections from both local and external networks. On the other hand, the IP address 127.0.0.1 (localhost) refers to the local machine itself and is used for communication between network programs within the same machine. If an IRC server is set to bind to 0.0.0.0, it can accept connections from both the local machine and external networks. However, if it is set to bind to 127.0.0.1, it will only accept connections from the local machine and will not be accessible from external network connections.

## Step for Implementing the Server

- Validate the port and password arguments.
- Handle any exceptions that may occur during the validation process. 
- If the arguments are valid, proceed with the server implementation.
- Use the validated port and password to configure the IRC server.  =>
- Start the server and listen for incoming connections.
- Handle any errors or exceptions that may occur during the server's execution.


## Configuring the Server

- `socket()`: This function creates a new socket for communication. In this case, it creates a TCP socket (SOCK_STREAM) using the IPv4 protocol (AF_INET). The importance of this step is to establish a communication endpoint that the server can use for sending and receiving data.

- `setsockopt()`: This function sets socket options. In this case, it sets the SO_REUSEADDR option to allow reusing the address. This is useful when the server needs to restart or bind to the same address that was previously used. The importance of this step is to enable address reuse and prevent errors related to "address already in use."

- `fcntl()`: This function sets the file status flags for the socket. In this case, it sets the O_NONBLOCK flag, which makes the socket non-blocking. Non-blocking sockets allow the server to perform I/O operations without waiting, which is crucial for handling multiple clients simultaneously. The importance of this step is to ensure non-blocking behavior and prevent the server from hanging.

- `bind()`: This function binds the socket to a specific address and port number. In this case, it binds to the address specified in the address structure. The importance of this step is to associate the socket with a specific network address so that incoming client connections can be received on that address.

- `listen()`: This function puts the socket into a listening state to accept incoming connections. It specifies the maximum number of pending connections that can be queued. In this case, std::numeric_limits<int>::max() is used to set the maximum value allowed by the system. The importance of this step is to make the server ready to accept client connections and specify the backlog size for incoming connection requests.

After successfully completing these steps, the server is considered online and can start accepting incoming connections.

## Running the Server

## Server Class Functions

This is a summary of the functions in the `Server` class, their purpose, and how they contribute to the overall functionality of the server.

### Server::initPollFD(int fd)
- Purpose: Initialize a `pollfd` structure for a given file descriptor (socket).
- Parameters:
  - `fd`: The file descriptor to initialize the `pollfd` structure for.
- Actions:
  - Creates a `pollfd` structure with the provided file descriptor (`fd`).
  - Sets the events to be monitored to `POLLIN` (incoming data).
  - Appends the initialized `pollfd` structure to the `_pollFD` vector.
- Notes:
  - Throws a `std::runtime_error` if the initialization fails.

### Server::run()
- Purpose: Main loop for the server to handle incoming connections and data.
- Actions:
  - Checks if the server is online and throws an exception if not.
  - Sets up a signal handler for interrupt signals.
  - Initializes the `pollfd` structure for the server socket.
  - Enters a loop that waits for events using `poll()`.
  - Handles new connections and incoming data from clients.
  - Closes client sockets and removes them from the `_pollFD` vector when necessary.
  - Checks if the server should stop and exits the loop if needed.
  - Performs necessary cleanup before exiting.
- Notes:
  - Throws a `std::runtime_error` if an error occurs during `poll()`.
  - Calls the `handleNewConnection()`, `handleClientData()`, and `closeClientSocket()` functions.

### Server::handleNewConnection()
- Purpose: Handle a new client connection.
- Actions:
  - Accepts the new client connection and creates a new socket for communication.
  - Initializes the `pollfd` structure for the new socket.
  - Prints information about the new connection.
- Notes:
  - Throws a `std::runtime_error` if the client connection fails.

### Server::handleClientData(size_t index)
- Purpose: Handle data received from an existing client.
- Parameters:
  - `index`: The index of the client's `pollfd` structure in the `_pollFD` vector.
- Actions:
  - Reads data from the client socket into a buffer.
  - Processes the received message.
  - Prints the received message.
- Notes:
  - Closes the client socket and removes it from the `_pollFD` vector if there is an error or the connection is closed.

### Server::closeClientSocket(size_t index)
- Purpose: Close the socket and remove a client from the `_pollFD` vector.
- Parameters:
  - `index`: The index of the client's `pollfd` structure in the `_pollFD` vector.
- Actions:
  - Closes the client socket.
  - Removes the client's `pollfd` structure from the `_pollFD` vector.

### Server::bye()
- Purpose: Clean up resources and close the server socket.
- Actions:
  - Closes the server socket and sets `_sockfd` to -1.
  - Performs any additional cleanup tasks.

### Server::handleSignal(int signal)
- Purpose: Handle signals received by the server.
- Parameters:
  - `signal`: The signal number received.
- Actions:
  - Prints the received signal.
  - Calls the `bye()` function to clean up resources and close the server socket.
  - Sets `_running` to false to stop the server gracefully.

### Server::signalHandler(int signal)
- Purpose: Static signal handling function.
- Parameters:
  - `signal`: The signal number received.
- Actions:
  - Calls the `handleSignal()` function of the `

serverInstance` if it is not `NULL`.


### InitPollFD()

The `initPollFD()` function takes an integer parameter `fd`, which represents a file descriptor.

1. Inside the function, a new `struct pollfd` variable named `pFD` is declared and initialized.

2. The `fd` parameter is assigned to the `fd` field of the `pFD` structure. This associates the file descriptor with the `pollfd` structure.

3. The `events` field of the `pFD` structure is set to `POLLIN | POLLOUT`. This specifies that the file descriptor should be monitored for both incoming data (read) and the ability to send data (write).

4. The `revents` field of the `pFD` structure is set to 0. This field is used by the `poll()` function to indicate which events occurred on the file descriptor. By initializing it to 0, we clear any previous event flags.

5. The `pFD` structure is added to a vector called `_pollFD`. The `_pollFD` vector likely holds multiple `pollfd` structures for tracking multiple file descriptors.

6. The code checks if the last element of the `_pollFD` vector has a file descriptor value of -1. If it does, it means there was an error initializing the `pollfd` structure. In that case, it throws a `std::runtime_error` with the message "Failed to initialize pollfd struct".

In summary, the `initPollFD()` function initializes a `pollfd` structure with the given file descriptor and desired events, adds it to a vector of `pollfd` structures, and checks for initialization errors.

### Polling()

The `polling()` function is responsible for using the `poll()` system call to monitor the file descriptors stored in the `_pollFD` vector for events.

1. Inside the function, a variable named `pollResult` is declared and assigned the result of the `poll()` function call.

2. The `poll()` function is called with the following arguments:
   - The first argument is `_pollFD.data()`, which provides a pointer to the beginning of the `_pollFD` vector's underlying array of `pollfd` structures. This allows `poll()` to access the file descriptors and associated event information.
   - The second argument is `_pollFD.size()`, which specifies the number of `pollfd` structures in the `_pollFD` vector.
   - The third argument is `-1`, which means that `poll()` will wait indefinitely for an event to occur. This means the function will block until there is an event on one of the file descriptors.

3. The code checks if the result of the `poll()` function call (`pollResult`) is less than 0. If it is, it means there was an error during the `poll()` operation.

4. Inside the error handling block, the code checks if the error is due to an interrupted system call (errno equals EINTR). This can happen if a signal, such as the SIGINT signal, interrupts the `poll()` operation.

5. If the error is due to an interrupted system call, it calls a function named `handleSignal()` with the SIGINT signal as an argument. This function likely performs some cleanup or handling specific to the SIGINT signal.

6. It prints an error message to the standard error stream (std::cerr) using the `RED` color code and then resets the color to the default using the `DEFAULT` color code.

7. It throws a `std::runtime_error` with an empty message. This is done to propagate the error up the call stack, indicating that an error occurred during the `poll()` operation.

In summary, the `polling()` function uses the `poll()` system call to wait for events on the file descriptors stored in the `_pollFD` vector. It handles potential errors, including interrupted system calls, and propagates any errors encountered during the `poll()` operation.

In simple terms, the `poll()` function is used to monitor multiple file descriptors (such as sockets) for events. It allows a program to wait until there is activity on one or more file descriptors before continuing execution.

When you call `poll()`, you provide an array of structures called `pollfd` that contains the file descriptors you want to monitor and the types of events you're interested in (such as data ready to read or the ability to write). You also specify a timeout value, which determines how long `poll()` should wait for an event to occur.

The `poll()` function will block (pause) the execution of your program until one of the monitored file descriptors has an event or until the specified timeout expires. When an event occurs, `poll()` returns, and you can check which file descriptors have events by examining the `revents` field of the `pollfd` structures you provided.

In summary, `poll()` is a function that allows you to wait for events on multiple file descriptors, giving you control over when to proceed based on the availability of data or the ability to write on those file descriptors.

## searchingForConnections()

The `searchingForConnections()` function is responsible for checking the events that occurred on the monitored file descriptors and taking appropriate actions based on those events.

1. The function iterates over each `pollfd` structure in the `_pollFD` vector using a `for` loop.

2. For each `pollfd` structure, it checks if the `revents` field indicates that the file descriptor is ready for writing (`POLLOUT`) and if it's not the server's main socket (`_sockfd`).

3. If the above conditions are met, it searches for a corresponding user associated with the file descriptor. It iterates over a vector of user objects and finds the user whose file descriptor matches the one in the `pollfd` structure.

4. If a user is found, it checks if the user has any outgoing messages (`getOutgoingMsg()`). If there are outgoing messages, it takes the first message from the list and sends it to the corresponding file descriptor using the `sendMsg()` function.

5. After handling the writing event, it checks if the file descriptor has a reading event (`POLLIN`).

6. If the reading event is on the server's main socket (`_sockfd`), it indicates a new client connection and calls the `handleNewConnection()` function to handle it.

7. If the reading event is on a client's file descriptor, it calls the `handleClientData()` function to handle the data received from the client.

In summary, the `searchingForConnections()` function checks for writing events on client file descriptors and sends any pending outgoing messages. It also handles reading events, distinguishing between new client connections and data received from existing clients, and calls the appropriate functions for further processing.


## Further Studies

- [Things to know about IRC](https://ircgod.com/docs/irc/to_know/)
- [FT IRServer Documentation](https://irc.dalexhd.dev/index.html)
- [IRSSI Documentation](https://irssi.org/documentation/manual/)
- [BEEJ Guide To Network Programming](https://beej.us/guide/bgnet/html/#select)

- [Internet Relay Chat Protocol](https://www.rfc-editor.org/rfc/rfc1459)

