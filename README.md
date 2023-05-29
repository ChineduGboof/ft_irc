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

- Validate the port and password arguments.
- Handle any exceptions that may occur during the validation process.
- If the arguments are valid, proceed with the server implementation.
- Use the validated port and password to configure the IRC server.
- Start the server and listen for incoming connections.
- Handle any errors or exceptions that may occur during the server's execution.