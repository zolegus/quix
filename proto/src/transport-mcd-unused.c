#define DEBUG(X) //X
#define DEBUGV(X) //X
#define DEBUGVV(X) //X


//#define BARRIER asm volatile("": : :"memory"); __sync_synchronize()
//#define BARRIER __sync_synchronize()
//#define BARRIER asm volatile("": : :"memory")
#define BARRIER


#include "transport.h"
#include <stdio.h> // printf
#include <stdlib.h> //exit
#include <memory.h>
#include <sched.h> // sched_yield

#define MASK ( RING_ENTRY_COUNT - 1 )

typedef union
{
  // Metrics Counters
  cacheline_counter_t counter;

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
uint64_t *node_sequence_ptr[ MAX_CHAN_COUNT ];
uint64_t *follow_sequence_ptr[ MAX_CHAN_COUNT ][ MAX_FOLL_COUNT ];

// Process details
int node;
int chan;
int cchan;

int walked;
uint64_t **walker_start_pptr[ MAX_CHAN_COUNT ];
volatile uint64_t **walker[ MAX_CHAN_COUNT ];
uint64_t sequence[ MAX_CHAN_COUNT ];


// Metric alias's
uint64_t *get_counter_ptr;
uint64_t *fed_value_ptr;
uint64_t *fad_value_ptr;
uint64_t *put_counter_ptr;
uint64_t *bed_value_ptr;
uint64_t *bad_value_ptr;

data_t *data;

//#define USE_NEW 1

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
  cchan = 0;
  if( node == 0 || node > MAX_NODE_COUNT || chan == 0 || chan > MAX_CHAN_COUNT ) 
  {
    printf( "Invalid node argument given\n" );
    exit( EXIT_FAILURE );
  }
#ifndef USE_NEW
  // Map node alias's
  memset( node_sequence_ptr, 0, MAX_CHAN_COUNT * sizeof( *node_sequence_ptr ) );
  for( int chan = 1 ; chan < MAX_CHAN_COUNT ; ++chan )
	{
    node_sequence_ptr[ chan ] = &data->transport[ node ].cursor[ chan ].data.value;
    printf( "Node sequence address: 0x%X [%d:%d]\n", node_sequence_ptr[ chan ], node, chan );
	}
#endif
  // Map following alias's
  memset( follow_sequence_ptr, 0, MAX_CHAN_COUNT * MAX_FOLL_COUNT * sizeof( uint64_t* ) );
  int idx[MAX_CHAN_COUNT];  memset ( idx, 0, MAX_CHAN_COUNT * sizeof( int ) );
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

#if USE_NEW

  // Map node and walker alias's
  memset( node_sequence_ptr, 0, MAX_CHAN_COUNT * sizeof( *node_sequence_ptr ) );
  memset( walker_start_pptr, 0, MAX_CHAN_COUNT * sizeof( walker_start_pptr[ 0 ] ) );
  walked = 0;
  for( int c = 0; c < MAX_CHAN_COUNT ; ++c )
	{
    if( follow_sequence_ptr[ c ][ 0 ] )
		{
      if( chan == c ) chan = walked;
      node_sequence_ptr[ walked ] = &data->transport[ node ].cursor[ c ].data.value;
      walker_start_pptr[ walked ] = follow_sequence_ptr[ c ];
      printf( "Node sequence address: *(0x%X) == 0x%X for [%d:%d]\n", 
              &node_sequence_ptr[ walked ], node_sequence_ptr[ walked ], node, c );
      printf( "Walker sequence address: *(0x%X) == 0x%X for [%d:%d]\n", 
              &walker_start_pptr[ walked ], walker_start_pptr[ walked ], node, c );
			++walked;
		}
	}
  memset( walker, 0, MAX_CHAN_COUNT * sizeof( walker[ 0 ] ) );

#endif

  // Register process in shared memory area & map metric alias's

  data->transport[node].counter.data.id  = node;
  data->transport[node].counter.data.magic  = node * 1111111111111111111;
  get_counter_ptr  =  &data->transport[node].counter.data.get;
  fed_value_ptr    =  &data->transport[node].counter.data.fed;
  fad_value_ptr    =  &data->transport[node].counter.data.fad;
  put_counter_ptr  =  &data->transport[node].counter.data.put;
  bed_value_ptr    =  &data->transport[node].counter.data.bed;
  bad_value_ptr    =  &data->transport[node].counter.data.bad;

  printf( "Registered node with id %d into messaging sublayer\n", node );
  return data->pool[ node ];
}


uint64_t* aquire_transport()
{
#ifdef USE_NEW

  memcpy( walker, walker_start_pptr, sizeof( walker[0] ) * walked );
  do
  {
    do
    {
      if( ++cchan == walked ) cchan = 0;
    }
    while( ( *node_sequence_ptr[ cchan ] + 1 & MASK ) == ( **walker[ cchan ] & MASK ) );
  }
  while( *++walker[ cchan ] );

  return &data->ring[ sequence[ chan ] & MASK ][ chan ].data.value;

#else

  DEBUG( static uint64_t counter = 0 );
  DEBUGVV( uint64_t attempts = 0 );

  register uint64_t sequence;

  volatile uint64_t **walker[MAX_CHAN_COUNT];

  for( int i = MAX_CHAN_COUNT ; --i ; )
    walker[i] = (volatile uint64_t**)follow_sequence_ptr[i];
		 
  while( 1 )
	{
    if( ++cchan == MAX_CHAN_COUNT ) cchan = 1;
    if( follow_sequence_ptr[cchan][0] == 0 ) continue; // Nothing followed for channel
    sequence = (((*node_sequence_ptr[cchan])+1) & MASK);
  	if( sequence != ( **walker[cchan] & MASK ) ) ++walker[cchan];
    if( !*walker[cchan] ) break; // Sequence followed is released
	}

  DEBUGVV( printf( "aquire_transport:\tMessage %llu at 0x%X aquired from slot [%llu:%llu(%llu)]\n",	\
									 data->ring[ (*node_sequence_ptr[ chan ]) & MASK ][ cchan ].data.value, \
									 &data->ring[ (*node_sequence_ptr[ chan ]) & MASK ][ cchan ].data.value, \
									 chan, (*node_sequence_ptr[ chan ]) & MASK, *node_sequence_ptr[ chan ] ) );

  DEBUG( *get_counter_ptr = ++counter );
  DEBUG( *fad_value_ptr = *fed_value_ptr / counter );
  DEBUGV( if( !( counter & DEBUG_SKIP_MASK ) )												\
	      printf( "Ctr: %llu\tFed: %d\n", counter, *fad_value_ptr ) );
  DEBUG( if( counter == 100 + RING_ENTRY_COUNT  ) *fed_value_ptr = 0 );

  return &data->ring[ (*node_sequence_ptr[ chan ]) & MASK ][ chan ].data.value;

#endif
}

void publish_transport()
{
  DEBUG( static uint64_t counter = 0 );
  DEBUGVV( printf( "publish_transport:\tPublished %llu at 0x%X, moving node sequence number from " \
									 "[%llu:%llu(%llu)] to [%llu:%llu(%llu)]\n",					\
									 data->ring[ (*node_sequence_ptr[ chan ]) & MASK ][ chan ].data.value, \
									 &data->ring[ (*node_sequence_ptr[ chan ]) & MASK ][ chan ].data.value, \
									 chan, (*node_sequence_ptr[ chan ]) & MASK,	*node_sequence_ptr[ chan ], 	\
                   chan, ((*node_sequence_ptr[ chan ]) + 1) & MASK, *node_sequence_ptr[ chan ] + 1 ) );

  BARRIER;
  ++( *node_sequence_ptr[ chan ] );

  DEBUG( *put_counter_ptr = ++counter );
  DEBUG( *bad_value_ptr = *bed_value_ptr / counter );
  DEBUGV( if( !( counter & DEBUG_SKIP_MASK ) )												\
	     printf( "Ctr: %llu\tBed: %d\n", counter, *bad_value_ptr ) );
  DEBUG( if( counter == 100 + RING_ENTRY_COUNT  ) *bed_value_ptr = 0 );
}


uint64_t* consume_transport()
{
#ifdef USE_NEW

  memcpy( walker, walker_start_pptr, sizeof( walker[0] ) * walked );
  do
  {
    do
    {
      if( ++cchan == walked ) cchan = 0;
    }
    while( ( *node_sequence_ptr[ cchan ] & MASK ) == ( **walker[ cchan ] & MASK ) );
  }
  while( *++walker[ cchan ] );

  return &data->ring[ sequence[ cchan ] ][ cchan ].data.value;

#else

  DEBUG( static uint64_t counter = 0 );
  DEBUGVV( uint64_t attempts = 0 );

  register uint64_t sequence;

  volatile uint64_t **walker[MAX_CHAN_COUNT];

  for( int i = MAX_CHAN_COUNT ; --i ; )
    walker[i] = (volatile uint64_t**)follow_sequence_ptr[i];

  while( 1 )
	{
    if( ++cchan == MAX_CHAN_COUNT ) cchan = 1;
    if( follow_sequence_ptr[cchan][0] == 0 ) continue; // Nothing followed for channel
    sequence = ((*node_sequence_ptr[cchan]) & MASK);
  	if( sequence != ( **walker[cchan] & MASK ) ) ++walker[cchan];
    if( !*walker[cchan] ) break; // Sequence followed is released
	}

  DEBUGVV( printf( "consume_transport:\tMessage %llu at 0x%X consumed from slot [%llu:%llu(%llu)]\n",	\
									 data->ring[ node_sequence ][ cchan ].data.value, \
									 &data->ring[ node_sequence ][ cchan ].data.value, \
									 cchan, sequence, *node_sequence_ptr[ cchan ] ) );

  DEBUG( *get_counter_ptr = ++counter );
  DEBUG( *fad_value_ptr = *fed_value_ptr / counter );
  DEBUGV( if( !( counter & DEBUG_SKIP_MASK ) )												\
       printf( "Ctr: %llu\tFed: %d\n", counter, *fad_value_ptr ) );
  DEBUG( if( counter == 100 + RING_ENTRY_COUNT  ) *fed_value_ptr = 0 );

  return &data->ring[ sequence ][ cchan ].data.value;

#endif
}

void republish_transport()
{
  DEBUG( static uint64_t counter = 0 );
  DEBUGVV( printf( "republish_transport:\tRepublished %llu at 0x%X, moving node sequence number from " \
									 "[%llu:%llu(%llu)] to [%d:%d(%d)]\n",								\
									 data->ring[ (*node_sequence_ptr[ cchan ]) & MASK ][ cchan ].data.value, \
									 &data->ring[ (*node_sequence_ptr[ cchan ]) & MASK ][ cchan ].data.value, \
									 cchan, (*node_sequence_ptr[ cchan ] ) & MASK, *node_sequence_ptr[ cchan ], \
                   cchan, ((*node_sequence_ptr[ cchan ] ) + 1) & MASK, *node_sequence_ptr[ cchan ] + 1 ) );

  BARRIER;
  ++( *node_sequence_ptr[ cchan ] );

  DEBUG( *put_counter_ptr = ++counter );
  DEBUG( *bad_value_ptr = *bed_value_ptr / counter );
  DEBUGV( if( !( counter & DEBUG_SKIP_MASK ) )												\
	     printf( "Ctr: %llu\tBed: %d\n", counter, *bad_value_ptr ) );
  DEBUG( if( counter == 100 + RING_ENTRY_COUNT  ) *bed_value_ptr = 0 );
}

void release_transport()
{
  DEBUG( static uint64_t counter = 0 );
  DEBUGVV( printf( "release_transport:\tReleased %llu at 0x%X, moving node sequence number from " \
                   "[%llu:%llu(%llu)] to [%llu:%llu(%llu)]\n", \
									 data->ring[ (*node_sequence_ptr[ cchan ]) & MASK ][ cchan ].data.value, \
									 &data->ring[ (*node_sequence_ptr[ cchan ]) & MASK ][ cchan ].data.value, \
									 cchan, (*node_sequence_ptr[ cchan ] ) & MASK, *node_sequence_ptr[ cchan ], \
									 cchan, ((*node_sequence_ptr[ cchan ] ) + 1) & MASK, *node_sequence_ptr[ cchan ] + 1 ) );

  BARRIER;
  ++( *node_sequence_ptr[ cchan ] );

  DEBUG( *put_counter_ptr = ++counter );
  DEBUG( *bad_value_ptr = *bed_value_ptr / counter );
  DEBUGV( if( !( counter & DEBUG_SKIP_MASK ) )												\
	     printf( "Ctr: %llu\tBed: %d\n", counter, *bad_value_ptr ) );
  DEBUG( if( counter == 100 + RING_ENTRY_COUNT ) *bed_value_ptr = 0 );
}
