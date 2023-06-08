# ft_irc
 Internet Relay Chat

## IRC Server Port Number
The maximum port number that can be used for an IRC server is `65,535`. Port numbers range from `0 to 65,535`, and ports below `1024` are reserved for well-known services and protocols.

In general, IRC servers commonly use port number `6667` for unencrypted connections and port number `6697` for encrypted connections using SSL/TLS. These ports are widely supported by IRC clients and servers.

## Understanding IP Addresses for IRC Servers: 0.0.0.0 and 127.0.0.1

The IP address 0.0.0.0 is used to indicate that a server should bind to all available network interfaces, allowing it to listen for incoming connections from both local and external networks. On the other hand, the IP address 127.0.0.1 (localhost) refers to the local machine itself and is used for communication between network programs within the same machine. If an IRC server is set to bind to 0.0.0.0, it can accept connections from both the local machine and external networks. However, if it is set to bind to 127.0.0.1, it will only accept connections from the local machine and will not be accessible from external network connections.

## Step for Implementing the Server

- Validate the port and password arguments. => (done)
- Handle any exceptions that may occur during the validation process.   => (done)
- If the arguments are valid, proceed with the server implementation.   => (done)
- Use the validated port and password to configure the IRC server.  => (partly done)
- Start the server and listen for incoming connections.     => (partly done)
- Handle any errors or exceptions that may occur during the server's execution. => (ongoing)


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


## Further Studies

- [Things to know about IRC](https://ircgod.com/docs/irc/to_know/)
- [FT IRServer Documentation](https://irc.dalexhd.dev/index.html)
- [IRSSI Documentation](https://irssi.org/documentation/manual/)
- [BEEJ Guide To Network Programming](https://beej.us/guide/bgnet/html/#select)

- [Internet Relay Chat Protocol](https://www.rfc-editor.org/rfc/rfc1459)

