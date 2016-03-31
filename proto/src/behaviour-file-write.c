#define DEBUG(X) //X
#define DEBUGV(X) //X
#define DEBUGVV(X) //X


#include "behaviour.h"
#include "arguments.h"
#include <stdio.h> // printf
#include <stdlib.h> //exit
#include <fcntl.h> //open
#include <unistd.h> // write
#include <errno.h> //errno
#include <string.h> //strerror


buffer_t *buffer_ptr;
int fd;


void *init_behaviour( void *raw )
{
  printf( "Init behaviour....." );

  char *write_filename = get_argument_as_string( 'w' );

  // Open stdin for reading
  if( !strcmp( write_filename, "-" ) )
	{
    printf( "using stdout.....\n" );
    fd = 1;
  }

  // Open file for writing
  else if ( ( fd = open( write_filename, O_WRONLY|O_CREAT, 0 ) ) < 0 )
  {
    printf( "Failed to open file to write messages %s: %d %s\n", 
      write_filename, errno, strerror( errno ) );
    exit( 1 );
  }

  printf( "done\n" );
  return raw;
}


uint64_t *exec_behaviour( uint64_t *message_ptr )
{
  buffer_ptr = ( buffer_t* )*message_ptr;
  DEBUGV( printf( "Consuming message: %d\n", buffer_to_string( buffer_ptr ) ) );

  // Write next message to file
  write( fd, buffer_ptr->data, buffer_ptr->size );

  DEBUGV( printf( "Republish message: %d\n", buffer_to_string( buffer_ptr ) ) );
  return message_ptr;
}


void post_behaviour()
{
  return;
}

