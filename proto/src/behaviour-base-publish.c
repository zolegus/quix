#define DEBUG(X) //X
#define DEBUGV(X) //X
#define DEBUGVV(X) //X


#include "behaviour.h"
#include "arguments.h"
#include <stdio.h> // printf, getch

uint64_t node;
uint64_t loops;

void *init_behaviour( void *raw )
{
  printf( "Init behaviour....." );
  node = get_argument_as_int( 'n' );
  node *= 1000000000000000;
  loops = get_argument_as_int( 'l' );
  printf( "done\n" );
  return raw;
}

uint64_t *exec_behaviour( uint64_t *message_ptr )
{
  *message_ptr = loops--;
  
  DEBUGV( static uint64_t counter = 0; printf( "%d:\tPublishing message: %llu\n", ++counter, *message_ptr ) );
  DEBUG( if( !*message_ptr ) { printf( "Null message published\n" ); exit(1); } );

  return message_ptr;
}

void post_behaviour()
{
  return;
}
