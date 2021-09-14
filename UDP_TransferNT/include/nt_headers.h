#ifndef NT_HEADERS_H
#define NT_HEADERS_H

#include "nt_platform.h"
#include <string>

// override if you have a proper logger. E.g spdlog
#ifndef DEFAULT_NT_LOGGER
#define DEFAULT_NT_LOGGER(x) std::cout << x << std::endl
#endif

#ifndef DEFAULT_NT_PORT
#define DEFAULT_NT_PORT 5801 // official allowed frc ports are 5800 to 5810
#endif

#ifndef DEFAULT_N_TYPEBLOCK
#define DEFAULT_N_TYPEBLOCK 4
#endif

#ifndef DEFAULT_BUFFER_SIZE
#define DEFAULT_BUFFER_SIZE 256 // must devisable by typeblock size
#endif

#define DATAPACKET_TYPESIZE DEFAULT_BUFFER_SIZE/DEFAULT_N_TYPEBLOCK

// headers for windows socket
#ifdef NT_UDP_PLATFORM_WINDOWS
	#include <stdio.h>
	#include <WinSock2.h>
	#pragma comment(lib,"ws2_32.lib") //Winsock Library

// headers for linux/macos socket
#elif defined(NT_UDP_PLATFORM_UNIX)


#endif // nt platform
#endif // NT_HEADERS_H