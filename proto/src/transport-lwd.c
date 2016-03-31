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
#include <sched.h> // sched_yield

#define MASK ( RING_ENTRY_COUNT - 1 )

typedef struct
{
  // Metrics Counters
  cacheline_counter_t counter;

  // Store current sequence number
  cacheline_uint64_t cursor;
}
transport_t;

typedef struct
{
  transport_t transport[MAX_NODE_COUNT];
  cacheline_uint64_t ring[ RING_ENTRY_COUNT ];
  char pool[ MAX_NODE_COUNT ][ NODE_MEMORY_POOL_SIZE ];
}
data_t;

// Ipc alias's
data_t *data;
uint64_t *node_sequence_ptr;
uint64_t *follow_sequence_ptr[ MAX_FOLL_COUNT ];

// Process details
uint64_t node;
uint64_t *follow;
uint64_t message;

// Metric alias's
uint64_t *get_counter_ptr;
uint64_t *fed_value_ptr;
uint64_t *fad_value_ptr;
uint64_t *put_counter_ptr;
uint64_t *bed_value_ptr;
uint64_t *bad_value_ptr;

cacheline_uint64_t *ring;
volatile uint64_t **walker;

void* init_transport_ex( void* s, uint64_t n, uint64_t *f )
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
  ring = data->ring;

  // Set process details
  node = n;
  follow = f;

  // Map node alias's
  node_sequence_ptr = &data->transport[node].cursor.data.value;
  printf( "Node sequence address: 0x%X\n", node_sequence_ptr );

  // Map following alias's
  for( int i = 0; i < MAX_NODE_COUNT; ++i)
	{
    follow_sequence_ptr[i] = follow[i] ? &data->transport[follow[i]].cursor.data.value : 0;
    printf( "Follow sequence address: 0x%X [%d]\n", follow_sequence_ptr[i], follow[i] );
	}

  // Register process in shared memory area & map metric alias's
  data->transport[node].counter.data.id  = node;
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
  DEBUG( static uint64_t counter = 0 );

  register uint64_t node_sequence = ((*node_sequence_ptr) & MASK);
  register uint64_t next_sequence = (((*node_sequence_ptr) + 1) & MASK);

  walker = (volatile uint64_t**)follow_sequence_ptr;
  do
	{
    DEBUGVV( printf( "aquire_transport:\t\tWaiting on node %d to release %d [%d]\n", \
										 *follow, next_sequence, (**walker) & MASK ) ); 
    while( next_sequence == ( (**walker) & MASK ) ) sched_yield();
	}
  while( *(++walker) );

	DEBUGVV( printf( "aquire_transport:\t\tslot %d is now aquired\n", node_sequence ) );

  DEBUG( *get_counter_ptr = ++counter );
  DEBUG( *fad_value_ptr = *fed_value_ptr / counter );
  DEBUGV( if( !( counter & DEBUG_SKIP_MASK ) )  \
       printf( "Ctr: %dm\tFed: %d\n", counter/DEBUG_SKIP_MASK, *fad_value_ptr ) );
  DEBUG( if( counter == 100 + RING_ENTRY_COUNT  ) *fed_value_ptr = 0 );

  return &ring[ node_sequence ].data.value;
}

void publish_transport()
{
  DEBUG( static uint64_t counter = 0 );

  DEBUGVV( register uint64_t node_sequence = ((*node_sequence_ptr) & MASK) );

  DEBUGVV( printf( "publish_transport:\tmessage %d published in slot %d\n", \
		 ring[ node_sequence ].data.value, node_sequence ) );

  DEBUGVV( printf( "publish_transport:\tMoving node sequence number from %d to %d\n", 
		 ((*node_sequence_ptr) & MASK), (((*node_sequence_ptr) + 1) & MASK) ) );

  BARRIER;
  ++( *node_sequence_ptr );

  DEBUG( *put_counter_ptr = ++counter );
  DEBUG( *bad_value_ptr = *bed_value_ptr / counter );
  DEBUGV( if( !( counter & DEBUG_SKIP_MASK ) )  \
       printf( "Ctr: %dm\tBed: %d\n", counter/DEBUG_SKIP_MASK, *bad_value_ptr ) );
  DEBUG( if( counter == 100 + RING_ENTRY_COUNT  ) *bed_value_ptr = 0 );
}

uint64_t* consume_transport()
{
  DEBUG( static uint64_t counter = 0 );

  register uint64_t node_sequence = ((*node_sequence_ptr) & MASK);

  walker = (volatile uint64_t**)follow_sequence_ptr;
  do
	{
    DEBUGVV( printf( "consume_transport:\tWaiting on node %d to release %d [%d]\n",  \
      *follow, node_sequence, (**walker) & MASK ) );
    while(node_sequence == ( (**walker) & MASK ) ) sched_yield();
  }
	while( *(++walker) );

  DEBUGVV( printf( "consume_transport:\tMessage %d consumed from slot %d\n", \
		   ring[ node_sequence ].data.value, node_sequence ) );

  DEBUG( *get_counter_ptr = ++counter );
  DEBUG( *fad_value_ptr = *fed_value_ptr / counter );
  DEBUGV( if( !( counter & DEBUG_SKIP_MASK ) )  \
       printf( "Ctr: %dm\tFed: %d\n", counter/DEBUG_SKIP_MASK, *fad_value_ptr ) );
  DEBUG( if( counter == 100 + RING_ENTRY_COUNT  ) *fed_value_ptr = 0 );

  return &ring[ node_sequence ].data.value;
}

void republish_transport()
{
  DEBUG( static uint64_t counter = 0 );

  DEBUG( register uint64_t node_sequence = ((*node_sequence_ptr) & MASK) );

  DEBUGVV( printf( "republish_transport:\tmessage %d published in slot %d\n", \
		   ring[ node_sequence ].data.value, node_sequence ) );

  DEBUGVV( printf( "republish_transport:\tMoving node sequence number from %d to %d\n", \
		   ((*node_sequence_ptr) & MASK), (((*node_sequence_ptr) + 1) & MASK) ) );

  BARRIER;
  ++(*node_sequence_ptr);

  DEBUG( *put_counter_ptr = ++counter );
  DEBUG( *bad_value_ptr = *bed_value_ptr / counter );
  DEBUGV( if( !( counter & DEBUG_SKIP_MASK ) )  \
       printf( "Ctr: %dm\tBed: %d\n", counter/DEBUG_SKIP_MASK, *bad_value_ptr ) );
  DEBUG( if( counter == 100 + RING_ENTRY_COUNT  ) *bed_value_ptr = 0 );
}

void release_transport()
{
  DEBUG( static uint64_t counter = 0 );

  DEBUGVV( register uint64_t node_sequence = ((*node_sequence_ptr) & MASK) );

  DEBUGVV( printf( "release_transport:\tmessage %d released from slot %d\n", \
		   ring[ node_sequence ].data.value, node_sequence ) );

  DEBUGVV( printf( "release_transport:\tMoving node sequence number from %d to %d\n", 
		 ((*node_sequence_ptr) & MASK), (((*node_sequence_ptr) + 1) & MASK) ) );

  BARRIER;
  ++(*node_sequence_ptr);

  DEBUG( *put_counter_ptr = ++counter );
  DEBUG( *bad_value_ptr = *bed_value_ptr / counter );
  DEBUGV( if( !( counter & DEBUG_SKIP_MASK ) )  \
       printf( "Ctr: %dm\tBed: %d\n", counter/DEBUG_SKIP_MASK, *bad_value_ptr ) );
  DEBUG( if( counter == 100 + RING_ENTRY_COUNT ) *bed_value_ptr = 0 );
}
