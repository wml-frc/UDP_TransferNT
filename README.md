# UDP Transfer Network

### Built for fast paced data streams between two or more devices

#### Build Status
[![Build Status](https://dev.azure.com/ConnorBuchel0890/WML/_apis/build/status/wml-frc.UDP_TransferNT?branchName=refs%2Fpull%2F5%2Fmerge)](https://dev.azure.com/ConnorBuchel0890/WML/_build/latest?definitionId=16&branchName=refs%2Fpull%2F5%2Fmerge)

#### Adding the lib as vendordep
- For FRC teams this library is stored in a maven repository, which can be added to vendordeps. Check https://docs.wpilib.org/en/stable/docs/software/vscode-overview/3rd-party-libraries.html on how to add the vendordep. 

- Use this link for the latest version -> https://panel.repsy.io/mvn/wml-frc/udp_transfernt/gradle/cpp/UDP_TransferNT-Deps/latest/UDP_TransferNT-Deps-latest.json

- If you want a specific verision you can use this link -> https://panel.repsy.io/mvn/wml-frc/udp_transfernt/gradle/cpp/UDP_TransferNT-Deps/VERSION/UDP_TransferNT-Deps-VERSION.json replacing `VERSION` with desired version e.g `2021.1.1`

#### Adding the lib as submodule dependency
- If you're wanting to use a specific branch, or for developers who want to contribute. You can add the project as a submodule, and include it as a dependency. See https://docs.gradle.org/current/userguide/declaring_dependencies.html for details on the library dependency.

1. use `git submodule add https://github.com/wml-frc/UDP_TransferNT.git ./SUB_LOCATION`
2. Include in root project

settings.gradle
```grale
include ':UDP_TransferNT'
```

3. Add dependency to project

e.g
```gradle
dependencies {
	implementation project(':UDP_TransferNT')
}
```

- or as a proper static/shared library in the binaries
```gradle
model {
	components {
		frcUserProgram(NativeExecutableSpec) {
			...

			binaries.all {
				// Or option one (more controlled for use case)
				lib project: ':UDP_TransferNT', library: 'UDP_TransferNT', linkage: 'shared' // Can also be static

				// Option two (May not work in certain applications)
				lib library: 'UDP_TransferNT'
			}
		}
	}
}
```


#### Manual addition.
However, if none of the above options work you can link the cpp and public folders using another means if you wish to use it without gradle. (All cpp is written using linux std libraries, in a linux environment) So there are no special requirements.

### Usage

- UDP transfer does not require a handshake between two devices, it will send the data to a port without regard, and anything listening on that port will receive data. This transfer system, is best used when the client tries to send the data to the servers IP. especially for programs like FRC where the data can be affected by other traffic.

- In cpp, include the UDP_TransferNT.h file and create either a server or a client. The process for sending data should be as simple as initializing either a server or client, creating a datapacket and then sending/receiving that datapacket.


#### Basic single threaded send/recv using server & client

##### Config
- Server is our sender
- Client is out receiver
- Port: `5801` <- Default
- IP: `192.168.178.125` <- default is `127.0.0.1`
- Buffer size/array sizes: `512` <- default 

Server
```cpp
#include "UDP_TransferNT.h"

UDP_TransferNT::Server server; // Create server
UDP_TransferNT::DataPacket dpSend; // Our datapacket which we will be sending

server.init(); // Create the socket and bind to it (Default port is 5801)

/**
 * Set values for datapacket (Each element is has an arraysize (buffersize) of 512, but can be changed)
 */
dpSend.setBooleans(1, true); // Set boolean index 1 as true
dpSend.setCharacter(10, 't'); // Set character index 10 as 't'
dpSend.setDecimals(0, 0.111); // set decimal value (float) index 0 to 0.111
dpSend.setIntegers(2, 12); // Set integer index 2 to 12
dpSend.setIntegers(1, 13); // Set integer index 1 to 13

server.send(dpSend); // Send the data packet

```

Client
```cpp
#include "UDP_TransferNT.h"

UDP_TransferNT::Client client; // Create client
UDP_TransferNT::DataPacket dpRecv; // Create the receiving datapacket

/**
 * Change socket values to point to server IP address
 */
client.getSocket().setIP("192.168.178.125"); // .setPort() is also available to switch port from 5801 to something else (Socket setters, must be accessed before the client is initialized, or the socket will be bound to the wrong place!)

client.init(); // Create the socket and connect to ip address

client.recv(dpRecv); // Listen for datapacket (Will stop program and wait)

/**
 * Output the data to console, remember the indexes you set them to, to get the correct data
 */
std::cout << dpRecv.getBooleans(1) << std::endl; // Get boolean on index 1 from recv
std::cout << dpRecv.getCharacter(10) << std::endl;
std::cout << dpRecv.getDecimals(0) << std::endl;
std::cout << dpRecv.getIntegers(2) << std::endl;
std::cout << dpRecv.getIntegers(3) << std::endl;
```

- The above example, shows how to use the most basic setup. However, it has the issue of waiting until data has been sent before the program can continue. And in programs with a constant data stream between the devices looping these functions can slow the program down. To solve this we can register a datapacket to be sent or received, and it will be put the send/recv functions onto a seperate thread which will loop continously sending or receiving data in the background.

- It should also be mentioned that the client/server init() functions can also hault the program if they're taking too long to connect/bind to a socket. When constructing them, you can set the first parameter to `true`, which will also temporarily put the init function on a seperate thread before joining back into the main thread when it's finished binding/connecting.

- Not only can you register datapackets, but you can control the registered send/recv threads later using the built in thread functions, to pause, continue, or kill the connection outright.

#### Multi-threaded registering/receiving of data

Server
```cpp
#include "UDP_TransferNT.h"

UDP_TransferNT::Server server(true); // Create server, (And set threading to true, so it will continue the rest of the program while it's still binding to the socket)
UDP_TransferNT::DataPacket dpSend; // Our datapacket which we will be sending

server.init(); // Initialize with values
float value = 0.001; // Our value that we're sending
dpSend.setDecimals(0, value); // Set index 0 to value

server.registerSend(dpSend); // Start a seperate thread which continuosly sends

while (true) {
	value += 0.001; // Contantly increase the value every cycle
	dpSend.setDecimals(0, value); // Set the decimal index
}
```

Client
```cpp
#include "UDP_TransferNT.h"

UDP_TransferNT::Client client(true); // Create client
UDP_TransferNT::DataPacket dpRecv; // Create the receiving datapacket

/**
 * Change socket values to point to server IP address
 */
client.getSocket().setIP("192.168.178.125"); // .setPort() is also available to switch port from 5801 to something else (Socket setters, must be accessed before the client is initialized, or the socket will be bound to the wrong place!)

client.init();

client.registerRecv(dpRecv); // Start a thread which constantly receives data and sends it to dpRecv

while (true) {
	std::cout << dpRecv.getDecimals(0) << std::endl; // Output what is being received
}
```

- After registered threads have been launched, you can modify them using the following (This controls all threads associated with the objects sending/receiving threads, not just the registered) WILL HAULT GENERIC SEND/RECV FUNCTIONS

```cpp
// Can be server or client
server.stop() // Stop all threads, (Still looping in the background, but not sending or receiving data)

server.start() // Start threads (Only works if they were previously stopped)

server.kill() // Kill the process. Stops the threads, sets the sate of the program and threads to DEAD, purges all values and closes the socket discriptors. (Cannot be started again after this unless you call server.init() again)
```

- The above examples showcase simple ways to send and receive data. You can have multiple servers and clients running on multiple ports, and you can change the buffersizes to increase speeds (Not by much). Below lists the rest of the functionality you can apply.

```cpp
/**
 * Generic network
 */
Server(bool thread = false); // Server constructor, optional thread init
Client(bool thread = false, ConnectionType ct = ConnectionType::IP_SPECIFIC); // Client constructor, optional thread init, select different connectiontype (ANY/IP_SPECIFIC)
void init(); // Used for server or client instances, initializes the network either on a seperate thread or same thread depending on constructed network
Network(Type t, ConnectionType ct); // Generic network connection, takes type (SERVER or CLIENT) and connection type (ANY/IP_SPECIFIC)
Type getType(); // Returns the type of network (SERVER/CLIENT)
void initNetwork(); // Used for generic network(), initializes network depending on type and connection type
void send(DataPacket &dp); // Used for Client, Server and generic Network. Sends datapacket to connected socket
void recv(DataPacket &dp); // Used for Client, Server and generic Network. Receives datapacket from connected socket
void registerSend(DataPacket &dp); // Starts a seperate thread, loops and sends data.
void registerReceive(DataPacket &dp); // Starts a seperate thread, loops and receives data.
void kill(); // Used for Client, Server, Network. Kills connection and purges all values.
void stop(); // Stop threads and set the network to IDLE. (Loops in the background but does not serialize data nor send/recv)
void start(); // sets the thread states back to running and puts the program in CONNECTED state
State getState(); // Returns current program state
ThreadState getState_t(); // Returns current thread state

/**
 * Socket (Getters)
 */
Socket &getSocket(); // Returns reference to socket values. Allows user to modify socket values (accecced via Server, Client and Network)
uint16_t getSocket().getHandshakePort(); // Returns port number for handshake
uint16_t getSocket().getPort(); // returns port for user send/recv data
const char *getSocket().getIP(); // Returns IP address, (Only used for Client/Network)
int &getSocket().getSocket(); // Return reference to socket discriptor
int &getSocket().getValread(); // Returns the read value from default std recv
socklen_t &getRecvLen(); // return reference to length of the receive
struct sockaddr_in &getLocalAddress(); // Return IPv4 local address values
struct sockaddr_in &getExternalAddress(); // Return IPv4 external/connecting address values
socklen_t &getLocalAddressLen(); // Return length of local address
socklen_t &getExternalAddressLen(); // Return length of external address
int16_t &getBufferSize(); // returns reference to buffersize
int16_t &getPacketSize(); // return packetsize

/**
 * Socket setters
 */
void getSocket().setPort(uint16_t port); // set port number for network
void getSocket().setIP(const char ip[IP_LEN]); // sets the ip of the server

/**
 * DEFINES (If you want to change buffer size/override any of these
 * defines, you need to define them before you include "UDP_TransferNT.h")
 */
#define TEAM // Does absolutely nothing :)
#define SERVER_IP "127.0.0.1" // Defines the default server ip (used for local connection/debugging) override using setIP()
#define IP_LEN 20 // Max size of an IP address is 20 characters
#define DEFAULT_BUFFSIZE 512 // Defines the size of the buffer (size of each array intergers, decimals, etc...)
#define HANDSHAKE_PORT 5800 // Handshake not implemented and this does nothing for now
#define PORT 5801 // Default port number server/client (override using setPort())

/**
 * Network macros
 */
#define KILL(s) perror(s); exit(1) // Kills the program with a message
#define ERROR(s) fprintf(stderr, s); exit(1) // Print the current error and kill the program
#define ERROR_PRINT(s) perror(s); printf("\n") // Prints the current error but does not exit the program
```