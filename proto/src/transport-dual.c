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
  cacheline_uint64_t cursor[2];
}
transport_t;

typedef struct
{
  transport_t transport[MAX_NODE_COUNT];
  char pool[ MAX_NODE_COUNT ][ NODE_MEMORY_POOL_SIZE ];
}
data_t;

// Ipc alias's
uint64_t *volatile node_value_ptr[2];
uint64_t *volatile follow_value_ptr[2];

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

// tick-tock rocker
uint64_t tt;

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

  // Set process details
  node = n;
  follow = *f;

  // Set pointer to shared memory area
  data_t *data = ( data_t* )s;
  
  // Map ipc alias's
  node_value_ptr[0]    =  &data->transport[node].cursor[0].data.value;
  follow_value_ptr[0]  =  &data->transport[follow].cursor[0].data.value;
  node_value_ptr[1]    =  &data->transport[node].cursor[1].data.value;
  follow_value_ptr[1]  =  &data->transport[follow].cursor[1].data.value;
  printf( "Node sequence address: 0x%X & 0x%X\n", node_value_ptr[0], node_value_ptr[1] );
  printf( "Follow sequence address: 0x%X & 0x%X\n", follow_value_ptr[0], follow_value_ptr[1] );

  // Register process in shared memory area & map metric alias's
  data->transport[node].counter.data.id  = node;
  get_counter_ptr  =  &data->transport[node].counter.data.get;
  fed_value_ptr    =  &data->transport[node].counter.data.fed;
  fad_value_ptr    =  &data->transport[node].counter.data.fad;
  put_counter_ptr  =  &data->transport[node].counter.data.put;
  bed_value_ptr    =  &data->transport[node].counter.data.bed;
  bad_value_ptr    =  &data->transport[node].counter.data.bad;

  // Setup tick tock rocker
  tt = 0;

  printf( "Registered node with id %d into messaging sublayer\n", node );
  return data->pool[ node ];
}

uint64_t* aquire_transport()
{
  DEBUG( static uint64_t counter = 0 );

  DEBUG( message = *node_value_ptr[ tt ] );
  DEBUGVV( printf( "%d:\tP%d[%d]:%d\n", counter, node, tt, message ) );
  while( message = *node_value_ptr[ tt ] ) DEBUG( ++(*bed_value_ptr) );
  DEBUGVV( printf( "%d:\tP%d[%d]:%d\n", counter, node, tt, message ) );

  DEBUG( *get_counter_ptr = ++counter );
  DEBUG( *fad_value_ptr = *fed_value_ptr / counter );
  DEBUGV( if( !( counter & DEBUG_SKIP_MASK ) )  \
       printf( "Ctr: %dm\tFed: %d\n", counter/DEBUG_SKIP_MASK, *fad_value_ptr ) );
  DEBUG( if( counter == 100 ) *fed_value_ptr = 0 );

  return &pending;
}

void publish_transport()
{
  DEBUG( static uint64_t counter = 0 );

  *node_value_ptr[ tt ] = pending;
  DEBUGVV( printf( "%d:\tP%d[%d]:%d\n", counter, node, tt, *node_value_ptr[ tt ] ) );
  tt = !tt;

  DEBUG( *put_counter_ptr = ++counter );
  DEBUG( *bad_value_ptr = *bed_value_ptr / counter );
  DEBUGV( if( !( counter & DEBUG_SKIP_MASK ) )  \
       printf( "Ctr: %dm\tBed: %d\n", counter/DEBUG_SKIP_MASK, *bad_value_ptr ) );
  DEBUG( if( counter == 100 ) *bed_value_ptr = 0 );
}

uint64_t* consume_transport()
{
  DEBUG( static uint64_t counter = 0 );

  DEBUGVV( message =  *follow_value_ptr[ tt ] );
  DEBUGVV( printf( "%d:\tC%d[%d]:%d\n", counter, follow, tt, message ) );
  while( !( message = *follow_value_ptr[ tt ] ) ) DEBUG(++(*fed_value_ptr));
  DEBUGVV( printf( "%d:\tC%d[%d]:%d\n", counter, follow, tt, message ) );
  pending = *follow_value_ptr[tt];
  *follow_value_ptr[tt] = 0;
  DEBUGVV( printf( "%d:\tC%d[%d]:%d\n", counter, follow, tt, *follow_value_ptr[tt] ) );
  
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

  DEBUGVV( message = *node_value_ptr[ tt ] );
  DEBUGVV( printf( "%d:\tP%d[%d]:%d\n", counter, node, tt, message ) );
  while( message = *node_value_ptr[ tt ] ) DEBUG( ++(*bed_value_ptr) );
  DEBUGVV( printf( "%d:\tP%d[%d]:%d\n", counter, node, tt, message ) );

  *node_value_ptr[ tt ] = pending;
  DEBUGVV( printf( "%d:\tP%d[%d]:%d\n", counter, node, tt, *node_value_ptr[ tt ] ) );
  tt = !tt;

  DEBUG( *put_counter_ptr = ++counter );
  DEBUG( *bad_value_ptr = *bed_value_ptr / counter );
  DEBUGV( if( !( counter & DEBUG_SKIP_MASK ) )  \
       printf( "Ctr: %dm\tBed: %d\n", counter/DEBUG_SKIP_MASK, *bad_value_ptr ) );
  DEBUG( if( counter == 100 ) *bed_value_ptr = 0 );
}

void release_transport()
{
  DEBUG( static uint64_t counter = 0 );

  tt = !tt;

  DEBUG( *put_counter_ptr = ++counter );
  DEBUG( *bad_value_ptr = *bed_value_ptr / counter );
  DEBUGV( if( !( counter & DEBUG_SKIP_MASK ) )  \
       printf( "Ctr: %dm\tBed: %d\n", counter/DEBUG_SKIP_MASK, *bad_value_ptr ) );
  DEBUG( if( counter == 100 ) *bed_value_ptr = 0 );
}
