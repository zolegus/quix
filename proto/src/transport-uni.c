#define DEBUG(X) //X
#define DEBUGV(X) //X
#define DEBUGVV(X) //X


#include "transport.h"
#include <stdio.h> // printf
#include <stdlib.h> //exit

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
  char pool[ MAX_NODE_COUNT ][ NODE_MEMORY_POOL_SIZE ];
}
data_t;

// Ipc alias's
volatile uint64_t *node_value_ptr;
volatile uint64_t *follow_value_ptr;

// Metric alias's
uint64_t *get_counter_ptr;
uint64_t *fed_value_ptr;
uint64_t *fad_value_ptr;
uint64_t *put_counter_ptr;
uint64_t *bed_value_ptr;
uint64_t *bad_value_ptr;

// Process details
uint64_t node;
uint64_t follow;
uint64_t pending;
uint64_t message;

void* init_transport_ex( void* s, uint64_t n, uint64_t *f )
{
  // Check memory mapped file is large enough for all data
  if( sizeof(data_t) > MMFILE_SIZE )
  {
    printf( "Memory mapped file needs to be at least %d bytes in length (%d)\n", sizeof( data_t ), MMFILE_SIZE );
    exit( EXIT_FAILURE );
  }

  // Check enough space for transport data as start of mapped memory
  if( sizeof(transport_t) > NODE_MEMORY_POOL_SIZE )
  {
    printf( "Node memory pool size needs to be least %d bytes in length\n", sizeof( transport_t ) );
    exit( EXIT_FAILURE );
  }

  // Set process details
  node = n;
  follow = *f;

  // Set pointer to shared memory area
  data_t *data = ( data_t* )s;
  
  // Map ipc alias's
  node_value_ptr    =  &data->transport[node].cursor.data.value;
  follow_value_ptr  =  &data->transport[follow].cursor.data.value;
  printf( "Node sequence address: 0x%X\n", node_value_ptr );
  printf( "Follow sequence address: 0x%X\n", follow_value_ptr );


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
  DEBUG( message =  *node_value_ptr );
  DEBUGVV( printf( "%d:\tP%d:%d\n", counter, node, message ) );

  while( message = *node_value_ptr ) DEBUG( ++(*bed_value_ptr) );

  DEBUGVV( printf( "%d:\tP%d:%d\n", counter, node, message ) );
  DEBUG( *get_counter_ptr = ++counter );
  DEBUG( *fad_value_ptr = *fed_value_ptr / DEBUG_SKIP_MASK );
  DEBUGV( if( !( counter & DEBUG_SKIP_MASK ) )  \
       printf( "Ctr: %dm\tFed: %d\n", counter/DEBUG_SKIP_MASK, *fad_value_ptr ) );
  DEBUG( if( counter == 100 ) *fed_value_ptr = 0 );

  return &pending;
}

void publish_transport()
{
  DEBUG( static uint64_t counter = 0 );

  *node_value_ptr = pending;

  DEBUGVV( printf( "%d:\tP%d:%d\n", counter, node, *node_value_ptr ) );
  DEBUG( *put_counter_ptr = ++counter );
  DEBUG( *bad_value_ptr = *bed_value_ptr / counter );
  DEBUGV( if( !( counter & DEBUG_SKIP_MASK ) )  \
    printf( "Ctr: %dm\tBed: %d\n", counter/DEBUG_SKIP_MASK, *bad_value_ptr ) );
  DEBUG( if( counter == 100 ) *bed_value_ptr = 0 );
}

uint64_t* consume_transport()
{
  DEBUG( static uint64_t counter = 0 );

  DEBUG( message = *follow_value_ptr );
  DEBUGVV( printf( "%d:\tC%d:%d\n", counter, follow, message ) );

  while( !( message = *follow_value_ptr ) ) DEBUG( ++(*fed_value_ptr) );

  DEBUGVV( printf( "%d:\tC%d:%d\n", counter, follow, message ) );

  pending = *follow_value_ptr;
  *follow_value_ptr = 0;

  DEBUGVV( printf( "%d:\tC%d:%d\n", counter, follow, *follow_value_ptr ) );
  DEBUG( *get_counter_ptr = ++counter );
  DEBUG( *fad_value_ptr = *fed_value_ptr / counter );
  DEBUGV( if( !( counter & DEBUG_SKIP_MASK ) )  \
    printf( "Ctr: %dm\tFed: %d\n", counter/DEBUG_SKIP_MASK, *fad_value_ptr ) );
  DEBUG( if( counter == 100 ) *fed_value_ptr = 0 );

  return &pending;
}

void republish_transport()
{
  DEBUG( static uint64_t counter = 0 );

  DEBUG( message =  *node_value_ptr );
  DEBUGVV( printf( "%d:\tP%d:%d\n", counter, node, message ) );

  while( message = *node_value_ptr ) DEBUG( ++(*bed_value_ptr ) );

  DEBUGVV( printf( "%d:\tP%d:%d\n", counter, node, message ) );

  *node_value_ptr = pending;

  DEBUGVV( printf( "%d:\tP%d:%d\n", counter, node, *node_value_ptr ) );
  DEBUG( *put_counter_ptr = ++counter );
  DEBUG( *bad_value_ptr = *bed_value_ptr / counter );
  DEBUGV( if( !( counter & DEBUG_SKIP_MASK ) )  \
    printf( "Ctr: %dm\tBed: %d\n", counter/DEBUG_SKIP_MASK, *bad_value_ptr ) );
  DEBUG( if( counter == 100 ) *bed_value_ptr = 0 );
}

void release_transport()
{
  DEBUG( static uint64_t counter = 0 );
  DEBUG( *put_counter_ptr = ++counter );
  DEBUG( *bad_value_ptr = *bed_value_ptr / counter );
  DEBUGV( if( !( counter & DEBUG_SKIP_MASK ) )  \
    printf( "Ctr: %dm\tBed: %d\n", counter/DEBUG_SKIP_MASK, *bad_value_ptr ) );
  DEBUG( if( counter == 100 ) *bed_value_ptr = 0 );
}
