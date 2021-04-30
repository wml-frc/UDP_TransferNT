#ifndef UDP_TRANSFERNT_H
#define UDP_TRANSFERNT_H

#ifdef _WIN32
#define UDP_TRANSFERNT_EXPORT_FUNC __declspec(dllexport)
#else
#define UDP_TRANSFERNT_EXPORT_FUNC
#endif

#include "nt_headers.h"
#include "Client.h"
#include "Server.h"

#endif
