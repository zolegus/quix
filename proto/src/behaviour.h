#ifndef __GUARD__BEHAVIOUR_H__
#define __GUARD__BEHAVIOUR_H__


#include "config.h"
#include <stdint.h> // uint64_t
#include <stdlib.h> //size_t


// Forward declarations
struct buffer;
struct event;
struct order;


// Raw data buffer structure
typedef struct buffer
{
  size_t size;
  char data[0];
}
buffer_t;
char* buffer_to_string( buffer_t *buffer_ptr );


// Event Structure
typedef struct event
{
  int id;
  int type;
  double price;
  double quanity;
  struct event *orig_ord_ptr;
  struct order *order_ptr;
}
event_t;
char* event_to_string( event_t *event_ptr );


// Order struture
typedef struct order
{
  int value;
  struct event *event_ptr;
}
order_t;
char* order_to_string( order_t *order_ptr );


// Behavior required functions
void* init_behaviour( void* raw );
uint64_t *exec_behaviour( uint64_t *message_ptr );
void post_behaviour();


#endif // __GUARD__BEHAVIOUR_H__
