# ft_irc
 Internet Relay Chat


## Installing IRSSI CLIENT USING DOCKER
To install the IRSSI client, follow the steps below:

### Prerequisites
- Docker should be installed on your system.

Installation Steps
- Clone the FT_IRC repository.
- Navigate to the IRSSI directory.
- Build and install IRSSI by running the following command:  `make all`

> *This command will build the Docker image and set up the necessary dependencies for IRSSI.*

### Running IRSSI
To run IRSSI, use the following command: `make run`

> *This command will start the IRSSI client in a Docker container. Now you have IRSSI installed and ready to use!*

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



## Further Studies

- [Things to know about IRC](https://ircgod.com/docs/irc/to_know/)
- [FT IRServer Documentation](https://irc.dalexhd.dev/index.html)
- [IRSSI Documentation](https://irssi.org/documentation/manual/)
- [BEEJ Guide To Network Programming](https://beej.us/guide/bgnet/html/#select)

- [Internet Relay Chat Protocol](https://www.rfc-editor.org/rfc/rfc1459)

