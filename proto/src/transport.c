#include "transport.h"
#include "arguments.h"
#include <stdio.h> // printf
#include <stdlib.h> //exit
#include <stdint.h> // uint64_t
#include <fcntl.h> //open
#include <unistd.h> // read
#include <errno.h> //errno
#include <string.h> //strerror
#include <sys/mman.h> // mmap


void* shared;
uint64_t node;
uint64_t foll[MAX_NODE_COUNT];


void* init_transport()
{

  // Map file into memory address space
  const char *mapped_filename = get_argument_as_string( 'm' );
  int fd;
  if( ( fd = open( mapped_filename, O_RDWR, 0 ) ) < 0 )
  {
    printf( "Failed to open file to memory map file %s: %d %s\n", 
      mapped_filename, errno, strerror( errno ) );
    exit( EXIT_FAILURE );
  }
	shared = mmap( MMAP_ADDR, MMFILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0 );
  close( fd );
  if( shared != MMAP_ADDR )
  {
    printf( "Failed to map file at address %x: %d %s\n", MMAP_ADDR, errno, strerror( errno ) );
    exit( EXIT_FAILURE );
  }
  printf( "Mapped: %s at 0x%x( 0x%x )\n", mapped_filename, shared, MMAP_ADDR );


  // Extract node id
  node = get_argument_as_int( 'n' );
  printf( "Node: %d\n", node );


  // Extract followed nodes
  char *following = get_argument_as_string( 'f' );
  for( char i, *tok = strtok(following,","); tok && i < MAX_FOLL_COUNT ; ++i, tok = strtok(0,",") )
    foll[i] = atoi(tok);
  printf( "Following: %s\n", following );


  // Setup node cached memory areas and touch
#if 0
  uint64_t cache_size[4] = {
    0, // strawman
    16 * 1024 / sizeof( *g.begin[1] ),
    100 * 1024 / sizeof( *g.begin[2] ),
    1024 * 1024 / sizeof( *g.begin[3] )
    };
  for( int i = 3; i == 1 ; --i )
  {
    printf( "Touch: L%d\n", i );
    g.begin[i] = malloc( cache_size[i] );
    g.end[i] = g.begin[i] + cache_size[i];
    for( uint64_t t = -1, *p = g.begin[i] ; p < g.end[i]; ++p ) t &= *p;
  }
#endif

  return init_transport_ex( shared, node, foll );
}
