//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <behaviour/behaviour.hpp>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//

#if 0
#include <behaviour.h>
#include <stdio.h>

char* buffer_to_string( buffer_t *buffer_ptr )
{
  return buffer_ptr->data;
}


char* event_to_string( event_t *event_ptr )
{
  static char string[99] = {0};

  sprintf( string, "{event:{id:%d;type:%x;price:%f;quanity:%f;orig_ord:%x;order:%x}}", 
					 event_ptr->id, event_ptr->type, event_ptr->price, event_ptr->quanity,
           event_ptr->orig_ord_ptr, event_ptr->order_ptr );

  return string;
}

char* order_to_string( order_t *order_ptr )
{
  static char string[99] = {0};
  return string;
}

#endif
