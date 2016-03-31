#define DEBUG(X) //X
#define DEBUGV(X) //X
#define DEBUGVV(X) //X


#include "behaviour.h"
#include <stdio.h> // printf

void *init_behaviour( void *raw )
{
  printf( "Init behaviour....." );
  printf( "done\n" );
  return raw;
}

uint64_t *exec_behaviour( uint64_t *message_ptr )
{
  DEBUGV( static uint64_t counter = 0; printf( "%d:\tRepubishing message %llu\n", ++counter, *message_ptr ) );
  DEBUG( if( !*message_ptr ) { printf( "Null message republished\n" ); exit(1); } );

  return message_ptr;
}

void post_behaviour()
{
  return;
}

