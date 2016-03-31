//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__BEHAVIOUR__BEHAVIOUR__HH
#define GUARD__BEHAVIOUR__BEHAVIOUR__HH
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <behaviour/behaviour.h>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E > 
class behaviour::behaviour
{
  struct impl;
  impl *pimpl;

public:

  using event_type = E;

  behaviour( void* );
  behaviour( const behaviour& ) = delete;
  behaviour( behaviour&& ) = delete;
  behaviour& operator=( behaviour ) = delete;
  behaviour& operator=( behaviour&& ) = delete;
  ~behaviour();

  void operator()( event_type& );
  void post();
};
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif // GUARD__BEHAVIOUR__BEHAVIOUR__HH
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//

#if 0
#ifndef __GUARD__BEHAVIOUR_H__
#define __GUARD__BEHAVIOUR_H__


#include <config.h>
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
uint64_t *exec_behaviour( uint64_t *event_ptr );
void post_behaviour();


#endif // __GUARD__BEHAVIOUR_H__
#endif
