#define DEBUG(X) //X
#define DEBUGV(X) //X
#define DEBUGVV(X) //X


#include "arguments.h"
#include "daemon.h"
#include "transport.h"
#include "behaviour.h"
#include <stdio.h> // printf


int main( int argc, char *argv[] )
{
  // Initial setup
  init_arguments( argc, argv );
  init_behaviour( init_transport() );

  // Get number of loops before exit
  int loops = get_argument_as_int( 'l' );

  // Turn to in daemon if asked
  init_daemon();

  // Main loop
  printf( "^C to exit\n" );
  do
  {
    publish_transport( exec_behaviour( aquire_transport() ) );
    DEBUGV( printf( "loops: %d\n" ) );
    post_behaviour();
  }
  while( --loops );
}
