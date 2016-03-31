#define DEBUG(X) //X
#define DEBUGV(X) //X
#define DEBUGVV(X) //X

#include "behaviour.h"
#include "arguments.h"
#include <stdio.h> // printf
#include <stdlib.h> //exit
#include <fcntl.h> //open
#include <unistd.h> // read
#include <errno.h> //errno
#include <string.h> //strerror


typedef struct
{
  union
  {
    buffer_t buffer;
    uint8_t raw[ RAW_MSG_SIZE_MAX ];
  }
  ring[ RING_ENTRY_COUNT ];
}
buffer_pool_t;


buffer_pool_t *buffer_pool_ptr;
buffer_t *buffer_ptr;
unsigned int idx;
int fd;


void *init_behaviour( void *raw )
{
  printf( "Init behaviour....." );

  buffer_pool_ptr = raw;
  idx = 0;

  char *read_filename = get_argument_as_string( 'r' );

  // Open stdin for reading
  if( !strcmp( read_filename, "-" ) )
	{
    printf( "using stdin.....\n" );
    fd = 0;
  }

  // Open file for reading
  else if ( ( fd = open( read_filename, O_RDONLY, 0 ) ) < 0 )
  {
    printf( "Failed to open file to read messages %s: %d %s\n", 
      read_filename, errno, strerror( errno ) );
    exit( 1 );
  }

  printf( "done\n" );
  return raw;
}


uint64_t *exec_behaviour( uint64_t *message_ptr )
{
  buffer_ptr = &buffer_pool_ptr->ring[ ++idx & RING_ENTRY_MASK ].buffer;;

  // Read next message from file
  buffer_ptr->size = read( fd, buffer_ptr->data, RAW_MSG_SIZE_MAX );
  if( buffer_ptr->data[ buffer_ptr->size - 1 ] == '\n' ) 
    buffer_ptr->data[ buffer_ptr->size - 1 ] = 0;
  else
    buffer_ptr->data[ buffer_ptr->size ] = 0;

  DEBUGV( printf( "Publishing message: %a\n", buffer_to_string( buffer_ptr ) ) );
  *message_ptr = ( uint64_t )buffer_ptr;
  return message_ptr;
}


void post_behaviour()
{
  return;
}

