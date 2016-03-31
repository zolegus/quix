#include "transport.h"
#include <stdio.h> // printf
#include <stdlib.h> //exit
#include <memory.h>
#include <sched.h> // sched_yield

#define MASK ( RING_ENTRY_COUNT - 1 )

typedef struct
{
  // Store current sequence number
  cacheline_uint64_t cursor[ MAX_CHAN_COUNT ];
}
transport_t;

typedef struct
{
  transport_t transport[MAX_NODE_COUNT];
  cacheline_uint64_t ring[ RING_ENTRY_COUNT ][ MAX_CHAN_COUNT ];
  char pool[ MAX_NODE_COUNT ][ NODE_MEMORY_POOL_SIZE ];
}
data_t;

// Ipc alias's
data_t *data;
uint64_t *follow_sequence_ptr[ MAX_CHAN_COUNT ][ MAX_FOLL_COUNT ];
uint64_t *node_sequence_ptr[ MAX_CHAN_COUNT ];
uint64_t **walker_start_pptr[ MAX_CHAN_COUNT ];
volatile uint64_t **walker[ MAX_CHAN_COUNT ];
uint64_t sequence[ MAX_CHAN_COUNT ];

// Process details
int node;
int chan;
int cchan;
int walked;

void* init_transport_ex( void* s, uint64_t n, uint64_t *follow )
{
  // Check memory mapped file is large enough for all data
  if( sizeof(data_t) > MMFILE_SIZE )
  {
    printf( "Memory mapped file needs to be at least %d bytes in length\n", sizeof( data_t ) );
    exit( EXIT_FAILURE );
  }

  // Check enough space for transport data as start of mapped memory
  if( sizeof(transport_t) > NODE_MEMORY_POOL_SIZE )
  {
    printf( "Node memory pool size needs to be least %d bytes in length\n", sizeof( transport_t ) );
    exit( EXIT_FAILURE );
  }

  // Set pointer to shared memory area
  data = ( data_t* )s;

  // Set process details
  node = n / 10;
  chan = n % 10; // Only used by injectors
  if( node == 0 || node > MAX_NODE_COUNT || chan == 0 || chan > MAX_CHAN_COUNT ) 
  {
    printf( "Invalid node argument given\n" );
    exit( EXIT_FAILURE );
  }

  // Map following alias's
  memset( follow_sequence_ptr, 0, MAX_CHAN_COUNT * MAX_FOLL_COUNT * sizeof( uint64_t* ) );
  int idx[ MAX_CHAN_COUNT ];  memset ( idx, 0, MAX_CHAN_COUNT * sizeof( int ) );
  for( int i = 0; i < MAX_FOLL_COUNT; ++i)
	{
    if( !follow[ i ] )  continue;

    unsigned int fnode = follow[i] / 10;
    unsigned int fchan = follow[i] % 10;
    if( fnode == 0 || fnode > MAX_NODE_COUNT || fchan == 0 || fchan > MAX_CHAN_COUNT ) 
    {
      printf( "Invalid follow argument given\n" );
      exit( EXIT_FAILURE );
    }

    follow_sequence_ptr[fchan][ idx[fchan] ] = &data->transport[ fnode ].cursor[ fchan ].data.value;
    printf( "Follow sequence address: *(0x%X) == 0x%X for [%d:%d]\n", 
            &follow_sequence_ptr[fchan][ idx[fchan] ], follow_sequence_ptr[fchan][ idx[fchan] ], fnode, fchan );
    ++idx[fchan];
	}

  // Map node and walker alias's
  cchan = walked = 0;
  memset( node_sequence_ptr, 0, MAX_CHAN_COUNT * sizeof( *node_sequence_ptr ) );
  memset( walker_start_pptr, 0, MAX_CHAN_COUNT * sizeof( walker_start_pptr[ 0 ] ) );
  for( int c = 0; c < MAX_CHAN_COUNT ; ++c )
	{
    if( follow_sequence_ptr[ c ][ 0 ] )
		{
      if( chan == c ) chan = walked;
      node_sequence_ptr[ walked ] = &data->transport[ node ].cursor[ c ].data.value;
      sequence[ walked ] = *node_sequence_ptr[ walked ];
      printf( "Node sequence address: *(0x%X) == 0x%X for [%d:%d]\n", 
              &node_sequence_ptr[ walked ], node_sequence_ptr[ walked ], node, c );
      walker_start_pptr[ walked ] = follow_sequence_ptr[ c ];
      printf( "Walker sequence address: *(0x%X) == 0x%X for [%d:%d]\n", 
              &walker_start_pptr[ walked ], walker_start_pptr[ walked ], node, c );
			++walked;
		}
	}
  memset( walker, 0, MAX_CHAN_COUNT * sizeof( walker[ 0 ] ) );

  printf( "Registered node with id %d into messaging sublayer\n", node );
  return data->pool[ node ];
}


uint64_t* aquire_transport()
{
  memcpy( walker, walker_start_pptr, sizeof( walker[0] ) * walked );
  do
  {
    do
    {
      if( ++cchan == walked ) cchan = 0;
    }
    while( ( sequence[ cchan ] + 1 & MASK ) == ( **walker[ cchan ] & MASK ) );
  }
  while( *++walker[ cchan ] );

  return &data->ring[ sequence[ chan ] & MASK ][ chan ].data.value;
}


void publish_transport()
{
  sequence[ chan ] = ( ++( *node_sequence_ptr[ chan ] ) & MASK );
}


uint64_t* consume_transport()
{
  memcpy( walker, walker_start_pptr, sizeof( walker[0] ) * walked );
  do
  {
    do
    {
      if( ++cchan == walked ) cchan = 0;
    }
    while( sequence[ cchan ] == ( **walker[ cchan ] & MASK ) );
  }
  while( *++walker[ cchan ] );

  return &data->ring[ sequence[ cchan ] ][ cchan ].data.value;
}


void republish_transport()
{
  sequence[ cchan ] = ( ++( *node_sequence_ptr[ cchan ] ) & MASK );
}


void release_transport()
{
  sequence[ cchan ] = ( ++( *node_sequence_ptr[ cchan ] ) & MASK );
}
