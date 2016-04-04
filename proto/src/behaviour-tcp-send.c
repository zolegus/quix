#define DEBUG(X) //X
#define DEBUGV(X) //X
#define DEBUGVV(X) //X


#include "behaviour.h"
#include "arguments.h"
#include <stdio.h> // printf
#include <stdlib.h> //exit
#include <unistd.h> // write/close
#include <errno.h> //errno
#include <string.h> //strerror
#include <sys/socket.h>
#include <netinet/in.h> //sockaddr_in
#include <arpa/inet.h>


buffer_t *buffer_ptr;
int fd;


void *init_behaviour( void *raw )
{
  printf( "Init behaviour....." );

  int socket_port = get_argument_as_int( 'p' );

  struct sockaddr_in servaddr;
  bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(socket_port);

  int listen_fd;
  if( ( listen_fd = socket(AF_INET, SOCK_STREAM, 0 ) ) < 0 )
  {
    printf( "Failed create listen socket for port %p: %d %s\n", 
      socket_port, errno, strerror( errno ) );
    exit( 1 );
  }

  if( bind( listen_fd, ( struct sockaddr* )&servaddr, sizeof( servaddr ) ) < 0 )
  {
    printf( "Failed create listen socket for port %d: %d %s\n", 
      socket_port, errno, strerror( errno ) );
    exit( 1 );
  }

  if( listen( listen_fd, TCP_LISTENQ ) < 0 )
  {
    printf( "Failed to bind listen socket for  %s: %d %s\n", 
      socket_port, errno, strerror( errno ) );
    exit( 1 );
  }

  if( ( fd = accept( listen_fd, (struct sockaddr*)0, 0 ) ) < 0 )
  {
    printf( "Failed to accept incoming connection on port %d: %d %s\n", 
      socket_port, errno, strerror( errno ) );
    exit( 1 );
  }

  close( listen_fd );

  printf( "done\n" );
	return raw;
}


uint64_t *exec_behaviour( uint64_t *message_ptr )
{
  buffer_ptr = ( buffer_t* )*message_ptr;
  DEBUGV( printf( "Consuming message: %s\n", buffer_to_string( buffer_ptr ) ) );

  // Send message on socket
  write( fd, buffer_ptr->data, buffer_ptr->size );

  DEBUGV( printf( "Republish message: %s\n", buffer_to_string( buffer_ptr ) ) );
  return message_ptr;
}


void post_behaviour()
{
  return;
}

