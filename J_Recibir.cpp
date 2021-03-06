

#include "stdafx.h"
#include "J_Recibir.h"

#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#if defined (WIN32) && !defined(__CYGWIN__)
#include <winsock.h>
#else
#include <unistd.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/time.h>
#endif
#include <string.h>

#include <iostream>

using namespace sockets;

J_Recibir::J_Recibir( void )
{
    _port = 0;
    _initialized = false;
    _buffer = 0L;
}

J_Recibir::~J_Recibir( void )
{
#if defined (WIN32) && !defined(__CYGWIN__)
    closesocket( _so);
#else
    close( _so );
#endif
}

bool J_Recibir::init( void )
{
#if defined(WIN32) && !defined(__CYGWIN__)
    WORD version = MAKEWORD(1,1);
    WSADATA wsaData;
    // First, we start up Winsock
    WSAStartup(version, &wsaData);
#endif

    if( _port == 0 )
    {
    fprintf( stderr, "Receiver::init() - port not defined\n" );
    return false;
    }

    if( (_so = socket( AF_INET, SOCK_DGRAM, 0 )) < 0 )
    {
        perror( "Socket" );
    return false;
    }
#if defined (WIN32) && !defined(__CYGWIN__)
//    const BOOL on = TRUE;
//    setsockopt( _so, SOL_SOCKET, SO_REUSEADDR, (const char*) &on, sizeof(int));
#else
    int on = 1;
    setsockopt( _so, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
#endif

//    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port   = htons( _port );
#if defined (WIN32) && !defined(__CYGWIN__)
    saddr.sin_addr.s_addr =  htonl(INADDR_ANY);
#else
    saddr.sin_addr.s_addr =  0;
#endif

    if( bind( _so, (struct sockaddr *)&saddr, sizeof( saddr )) < 0 )
    {
        perror( "bind" );
        return false;
    }

	u_long iMode = 1;		// 1 para No bloqueante, 0 para bloqueante
    ioctlsocket(_so, FIONBIO, &iMode);

    _initialized = true;
    return _initialized;
}


void J_Recibir::setPort( const short port )
{
    _port = port;
}

void J_Recibir::setBuffer( void *buffer, const unsigned int size )
{
    _buffer = buffer;
    _buffer_size = size;
}

int J_Recibir::sync( void )
{
    if(!_initialized) init();

    if( _buffer == 0L )
    {
        fprintf( stderr, "Receiver::sync() - No buffer\n" );
        return -1;
    }

#if defined(__linux) || defined(__FreeBSD__) || defined( __APPLE__ )
    socklen_t 
#else
    int
#endif
        size = sizeof( struct sockaddr_in );

    fd_set fdset;
    FD_ZERO( &fdset );
    FD_SET( _so, &fdset );

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;

#if defined (WIN32) && !defined(__CYGWIN__)
//    saddr.sin_port   = htons( _port );
    recvfrom( _so, (char *)_buffer, _buffer_size, 0, (sockaddr*)&saddr, &size );

//    recvfrom(sock_Receive, szMessage, 256, 0, (sockaddr*)&addr_Cli, &clilen)
    int err = WSAGetLastError ();
	if (err!=0){
		fprintf( stderr, "Receiver::sync() - error %d\n",err );
		perror("Error: ");
	}

    while( select( _so+1, &fdset, 0L, 0L, &tv ) )
    {
        if( FD_ISSET( _so, &fdset ) )
        {
            recvfrom( _so, (char *)_buffer, _buffer_size, 0, (sockaddr*)&saddr, &size );
        }
    }
#else
    recvfrom( _so, (caddr_t)_buffer, _buffer_size, 0, 0, &size );
    while( select( _so+1, &fdset, 0L, 0L, &tv ) )
    {
        if( FD_ISSET( _so, &fdset ) )
        {
            recvfrom( _so, (caddr_t)_buffer, _buffer_size, 0, 0, &size );
        }
    }
#endif

	if (err!=0) return -1;
	else		return 0;
}

