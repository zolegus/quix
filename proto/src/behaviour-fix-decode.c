#define DEBUG(X) X
#define DEBUGV(X) X
#define DEBUGVV(X) //X

#include "behaviour.h"
#include <stdio.h> // printf


typedef struct
{
  event_t ring[ RING_ENTRY_COUNT ];
}
event_pool_t;


buffer_t *buffer_ptr;
event_pool_t *event_pool_ptr;
event_t *event_ptr;
unsigned int idx;


void *init_behaviour( void *raw )
{
  printf( "Init behaviour....." );

  event_pool_ptr = raw;
  idx = 0;

  printf( "done\n" );
  return raw;
}


uint64_t *exec_behaviour( uint64_t *message_ptr )
{
  buffer_ptr = ( buffer_t* )*message_ptr;
  DEBUGV( printf( "Consuming message: %s\n", buffer_to_string( buffer_ptr ) ) );

  event_ptr = &event_pool_ptr->ring[ ++idx & RING_ENTRY_MASK ];
  event_ptr->id = idx;  

  // Decode fix message  
  char *fixmsg = buffer_ptr->data;
  while(*fixmsg)
  {
    DEBUGVV( printf( "Processing fixmsg: %s\n", fixmsg ) );

    int t, i = *fixmsg - '0'; 
    while( *++fixmsg != '=' )  i = ( i * 10 ) + ( *fixmsg - '0' );
    switch( i )
    {
		case 35:
      switch( *++fixmsg )
			{
      case '8':
        event_ptr->type = event_ptr->type | 0x80;
			  break;
      case '9':
        event_ptr->type = event_ptr->type | 0x90;
			  break;
      case 'D':
        event_ptr->type = event_ptr->type | 0xD0;
			  break;
      case 'G':
        event_ptr->type = event_ptr->type | 0xE0;
			  break;
      case 'F':
        event_ptr->type = event_ptr->type | 0xF0;
			  break;
      default:
        event_ptr->type = 0;
        break;
      }
 			DEBUGVV( printf( "Order type (%c) is: %X\n", *fixmsg, event_ptr->type ) );
		  if( *++fixmsg && ( *fixmsg != ';' ) ) printf( "Bad Fix Message Type\n");
			break;

		case 150:
      switch( *++fixmsg )
			{
      case '0':
        event_ptr->type = event_ptr->type & 0x0;
			  break;
      case '1':
        event_ptr->type = event_ptr->type | 0x1;
			  break;
      case '2':
        event_ptr->type = event_ptr->type | 0x2;
			  break;
      case '3':
        event_ptr->type = event_ptr->type | 0x3;
			  break;
      case '4':
        event_ptr->type = event_ptr->type | 0x4;
			  break;
      case '5':
        event_ptr->type = event_ptr->type | 0x5;
			  break;
      case '6':
        event_ptr->type = event_ptr->type | 0x6;
			  break;
      case '7':
        event_ptr->type = event_ptr->type | 0x7;
			  break;
      case '8':
        event_ptr->type = event_ptr->type | 0x8;
			  break;
      case '9':
        event_ptr->type = event_ptr->type | 0x9;
			  break;
      case 'A':
        event_ptr->type = event_ptr->type | 0xA;
			  break;
      case 'B':
        event_ptr->type = event_ptr->type | 0xB;
			  break;
      case 'C':
        event_ptr->type = event_ptr->type | 0xC;
			  break;
      case 'D':
        event_ptr->type = event_ptr->type | 0xD;
			  break;
      case 'E':
        event_ptr->type = event_ptr->type | 0xE;
			  break;
      default:
        event_ptr->type = 0;
        break;
      }
      DEBUGVV( printf( "Order type (%c) is: %X\n", *fixmsg, event_ptr->type ) );
		  if( *++fixmsg && ( *fixmsg != ';' ) ) printf( "Bad Fix Message Type\n");
			break;

    default :
      while( *++fixmsg && *fixmsg != ';' ) t += *fixmsg;
      break;
    }

    if(*fixmsg) fixmsg++;
  }

  DEBUGV( printf( "Republishing message: %s\n", event_to_string( event_ptr ) ) );
  *message_ptr = ( uint64_t )event_ptr;
  return message_ptr;
}


void post_behaviour()
{
  return;
}
