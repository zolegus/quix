#ifndef __GUARD__TRANSPORT_H__
#define __GUARD__TRANSPORT_H__


#include "config.h"
#include <stdint.h> // uint64_t


typedef union
{
  struct
  {
    uint64_t id;
    uint64_t magic;
    uint64_t get;
    uint64_t fed;
    uint64_t fad;
    uint64_t put;
    uint64_t bed;
    uint64_t bad;
  }
  data;
  char raw[64];
}
cacheline_counter_t;


typedef union
{
  struct
  {
    uint64_t value;
  }
  data;
  char raw[64];
}
cacheline_uint64_t;


void* init_transport();
void* init_transport_ex( void* s, uint64_t n, uint64_t *f );
uint64_t* aquire_transport();
void publish_transport();
uint64_t* consume_transport();
void republish_transport();
void release_transport();


#endif // __GUARD__TRANSPORT_H__
