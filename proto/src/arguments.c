#include "arguments.h"
#include <stdlib.h> //atoi
#include <unistd.h> //getopt
#include <memory.h> //memset


char arg_values[128][64];


void init_arguments( int argc, char *argv[] )
{
  memset( arg_values, 0, 128 * 64 );
  char opt, *args = "a:b:c:d:e:f:g:h:i:j:k:l:m:n:o:p:q:r:s:t:u:v:w:x:y:z:";
	while( ( opt = getopt( argc, argv, args ) ) != -1 )  strcpy( arg_values[ opt ], optarg );
}


char get_argument_as_char( char opt )
{
  return arg_values[ opt ][ 0 ];
}


int get_argument_as_int( char opt )
{
  return atoi( arg_values[ opt ] );
}


char *get_argument_as_string( char opt )
{
  return arg_values[ opt ];
}

