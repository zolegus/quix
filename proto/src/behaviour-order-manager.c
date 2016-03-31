#define DEBUG(X) //X
#define DEBUGV(X) //X
#define DEBUGVV(X) //X


#include "behaviour.h"
#include <stdio.h> // printf


typedef struct
{
  order_t ring[ RING_ENTRY_COUNT ];
}
order_pool_t;


event_t *event_ptr;
order_pool_t *order_pool_ptr;
order_t *order_ptr;
unsigned int idx;


void *init_behaviour( void *raw )
{
  printf( "Init behaviour....." );

  order_pool_ptr = raw;

  printf( "done\n" );
  return raw;
}


int process_order();
uint64_t *exec_behaviour( uint64_t *message_ptr )
{
  event_ptr = ( event_t* )*message_ptr;
  DEBUGV( printf( "Consume message: %s\n", event_to_string( event_ptr ) ) );

  if( order_ptr = event_ptr->order_ptr )
  {
    // Existing event
    DEBUGV( printf( "Update existing event order\n" ) );
    order_ptr->value = process_order();
	}
  else
	{
    // New event
    DEBUGV( printf( "Create new event order\n" ) );
    order_ptr = event_ptr->order_ptr = &order_pool_ptr->ring[ ++idx & RING_ENTRY_MASK ];
    order_ptr->value = 0;
	}

  DEBUGV( printf( "Republishing message: %s\n", event_to_string( event_ptr ) ) );
  *message_ptr = ( uint64_t )event_ptr;
  return message_ptr;
}


void post_behaviour()
{
  return;
}


int process_order()
{
  switch( order_ptr->value )
	{

  case 0x0: // Current State: [New]
	{
    switch( event_ptr->type )
		{
    case 0x80: // Event: [New]
      break;
    case 0x81: // Event: [PartialFill]
      break;
    case 0x82: // Event: [Fill]
      break;
    case 0x83: // Event: [DoneForDay]
      break;
    case 0x84: // Event: [Canceled]
      break;
    case 0x85: // Event: [Replaced]
      break;
    case 0x86: // Event: [PendingCancel]
      break;
    case 0x87: // Event: [Stopped]
      break;
    case 0x88: // Event: [Rejected]
      break;
    case 0x89: // Event: [Suspended]
      break;
    case 0x8A: // Event: [PendingNew]
      break;
    case 0x8B: // Event: [Calculated]
      break;
    case 0x8C: // Event: [Expired]
      break;
    case 0x8D: // Event: [Reorderd]
      break;
    case 0x8E: // Event: [PendingReplace]
      break;
    case 0x90: // Event: [CancelReject]
      break;
    case 0xD0: // Event: [OrderSingle]
      break;
    case 0xE0: // Event: [CancelReplace]
      break;
		case 0xF0: // Event: [Cancel]
      break;
		}
   break;
 	}

  case 0x1: // Current State: [PartialFill]
	{
    switch( event_ptr->type )
		{
    case 0x80: // Event: [New]
      break;
    case 0x81: // Event: [PartialFill]
      break;
    case 0x82: // Event: [Fill]
      break;
    case 0x83: // Event: [DoneForDay]
      break;
    case 0x84: // Event: [Canceled]
      break;
    case 0x85: // Event: [Replaced]
      break;
    case 0x86: // Event: [PendingCancel]
      break;
    case 0x87: // Event: [Stopped]
      break;
    case 0x88: // Event: [Rejected]
      break;
    case 0x89: // Event: [Suspended]
      break;
    case 0x8A: // Event: [PendingNew]
      break;
    case 0x8B: // Event: [Calculated]
      break;
    case 0x8C: // Event: [Expired]
      break;
    case 0x8D: // Event: [Reorderd]
      break;
    case 0x8E: // Event: [PendingReplace]
      break;
    case 0x90: // Event: [CancelReject]
      break;
    case 0xD0: // Event: [OrderSingle]
      break;
    case 0xE0: // Event: [CancelReplace]
      break;
		case 0xF0: // Event: [Cancel]
      break;
		}
    break;
 	}

  case 0x2: // Current State: [Fill]
	{
    switch( event_ptr->type )
		{
    case 0x80: // Event: [New]
      break;
    case 0x81: // Event: [PartialFill]
      break;
    case 0x82: // Event: [Fill]
      break;
    case 0x83: // Event: [DoneForDay]
      break;
    case 0x84: // Event: [Canceled]
      break;
    case 0x85: // Event: [Replaced]
      break;
    case 0x86: // Event: [PendingCancel]
      break;
    case 0x87: // Event: [Stopped]
      break;
    case 0x88: // Event: [Rejected]
      break;
    case 0x89: // Event: [Suspended]
      break;
    case 0x8A: // Event: [PendingNew]
      break;
    case 0x8B: // Event: [Calculated]
      break;
    case 0x8C: // Event: [Expired]
      break;
    case 0x8D: // Event: [Reorderd]
      break;
    case 0x8E: // Event: [PendingReplace]
      break;
    case 0x90: // Event: [CancelReject]
      break;
    case 0xD0: // Event: [OrderSingle]
      break;
    case 0xE0: // Event: [CancelReplace]
      break;
		case 0xF0: // Event: [Cancel]
      break;
		}
    break;
 	}

  case 0x3: // Current State: [DoneForDay]
	{
    switch( event_ptr->type )
		{
    case 0x80: // Event: [New]
      break;
    case 0x81: // Event: [PartialFill]
      break;
    case 0x82: // Event: [Fill]
      break;
    case 0x83: // Event: [DoneForDay]
      break;
    case 0x84: // Event: [Canceled]
      break;
    case 0x85: // Event: [Replaced]
      break;
    case 0x86: // Event: [PendingCancel]
      break;
    case 0x87: // Event: [Stopped]
      break;
    case 0x88: // Event: [Rejected]
      break;
    case 0x89: // Event: [Suspended]
      break;
    case 0x8A: // Event: [PendingNew]
      break;
    case 0x8B: // Event: [Calculated]
      break;
    case 0x8C: // Event: [Expired]
      break;
    case 0x8D: // Event: [Reorderd]
      break;
    case 0x8E: // Event: [PendingReplace]
      break;
    case 0x90: // Event: [CancelReject]
      break;
    case 0xD0: // Event: [OrderSingle]
      break;
    case 0xE0: // Event: [CancelReplace]
      break;
		case 0xF0: // Event: [Cancel]
      break;
		}
    break;
 	}

  case 0x4: // Current State: [Canceled]
	{
    switch( event_ptr->type )
		{
    case 0x80: // Event: [New]
      break;
    case 0x81: // Event: [PartialFill]
      break;
    case 0x82: // Event: [Fill]
      break;
    case 0x83: // Event: [DoneForDay]
      break;
    case 0x84: // Event: [Canceled]
      break;
    case 0x85: // Event: [Replaced]
      break;
    case 0x86: // Event: [PendingCancel]
      break;
    case 0x87: // Event: [Stopped]
      break;
    case 0x88: // Event: [Rejected]
      break;
    case 0x89: // Event: [Suspended]
      break;
    case 0x8A: // Event: [PendingNew]
      break;
    case 0x8B: // Event: [Calculated]
      break;
    case 0x8C: // Event: [Expired]
      break;
    case 0x8D: // Event: [Reorderd]
      break;
    case 0x8E: // Event: [PendingReplace]
      break;
    case 0x90: // Event: [CancelReject]
      break;
    case 0xD0: // Event: [OrderSingle]
      break;
    case 0xE0: // Event: [CancelReplace]
      break;
		case 0xF0: // Event: [Cancel]
      break;
		}
    break;
 	}

  case 0x5: // Current State: [Replaced]
	{
    switch( event_ptr->type )
		{
    case 0x80: // Event: [New]
      break;
    case 0x81: // Event: [PartialFill]
      break;
    case 0x82: // Event: [Fill]
      break;
    case 0x83: // Event: [DoneForDay]
      break;
    case 0x84: // Event: [Canceled]
      break;
    case 0x85: // Event: [Replaced]
      break;
    case 0x86: // Event: [PendingCancel]
      break;
    case 0x87: // Event: [Stopped]
      break;
    case 0x88: // Event: [Rejected]
      break;
    case 0x89: // Event: [Suspended]
      break;
    case 0x8A: // Event: [PendingNew]
      break;
    case 0x8B: // Event: [Calculated]
      break;
    case 0x8C: // Event: [Expired]
      break;
    case 0x8D: // Event: [Reorderd]
      break;
    case 0x8E: // Event: [PendingReplace]
      break;
    case 0x90: // Event: [CancelReject]
      break;
    case 0xD0: // Event: [OrderSingle]
      break;
    case 0xE0: // Event: [CancelReplace]
      break;
		case 0xF0: // Event: [Cancel]
      break;
		}
    break;
 	}

  case 0x6: // Current State: [PendingCancel]
	{
    switch( event_ptr->type )
		{
    case 0x80: // Event: [New]
      break;
    case 0x81: // Event: [PartialFill]
      break;
    case 0x82: // Event: [Fill]
      break;
    case 0x83: // Event: [DoneForDay]
      break;
    case 0x84: // Event: [Canceled]
      break;
    case 0x85: // Event: [Replaced]
      break;
    case 0x86: // Event: [PendingCancel]
      break;
    case 0x87: // Event: [Stopped]
      break;
    case 0x88: // Event: [Rejected]
      break;
    case 0x89: // Event: [Suspended]
      break;
    case 0x8A: // Event: [PendingNew]
      break;
    case 0x8B: // Event: [Calculated]
      break;
    case 0x8C: // Event: [Expired]
      break;
    case 0x8D: // Event: [Reorderd]
      break;
    case 0x8E: // Event: [PendingReplace]
      break;
    case 0x90: // Event: [CancelReject]
      break;
    case 0xD0: // Event: [OrderSingle]
      break;
    case 0xE0: // Event: [CancelReplace]
      break;
		case 0xF0: // Event: [Cancel]
      break;
		}
    break;
 	}

  case 0x7: // Current State: [Stopped]
	{
    switch( event_ptr->type )
		{
    case 0x80: // Event: [New]
      break;
    case 0x81: // Event: [PartialFill]
      break;
    case 0x82: // Event: [Fill]
      break;
    case 0x83: // Event: [DoneForDay]
      break;
    case 0x84: // Event: [Canceled]
      break;
    case 0x85: // Event: [Replaced]
      break;
    case 0x86: // Event: [PendingCancel]
      break;
    case 0x87: // Event: [Stopped]
      break;
    case 0x88: // Event: [Rejected]
      break;
    case 0x89: // Event: [Suspended]
      break;
    case 0x8A: // Event: [PendingNew]
      break;
    case 0x8B: // Event: [Calculated]
      break;
    case 0x8C: // Event: [Expired]
      break;
    case 0x8D: // Event: [Reorderd]
      break;
    case 0x8E: // Event: [PendingReplace]
      break;
    case 0x90: // Event: [CancelReject]
      break;
    case 0xD0: // Event: [OrderSingle]
      break;
    case 0xE0: // Event: [CancelReplace]
      break;
		case 0xF0: // Event: [Cancel]
      break;
		}
    break;
 	}

  case 0x8: // Current State: [Rejected]
	{
    switch( event_ptr->type )
		{
    case 0x80: // Event: [New]
      break;
    case 0x81: // Event: [PartialFill]
      break;
    case 0x82: // Event: [Fill]
      break;
    case 0x83: // Event: [DoneForDay]
      break;
    case 0x84: // Event: [Canceled]
      break;
    case 0x85: // Event: [Replaced]
      break;
    case 0x86: // Event: [PendingCancel]
      break;
    case 0x87: // Event: [Stopped]
      break;
    case 0x88: // Event: [Rejected]
      break;
    case 0x89: // Event: [Suspended]
      break;
    case 0x8A: // Event: [PendingNew]
      break;
    case 0x8B: // Event: [Calculated]
      break;
    case 0x8C: // Event: [Expired]
      break;
    case 0x8D: // Event: [Reorderd]
      break;
    case 0x8E: // Event: [PendingReplace]
      break;
    case 0x90: // Event: [CancelReject]
      break;
    case 0xD0: // Event: [OrderSingle]
      break;
    case 0xE0: // Event: [CancelReplace]
      break;
		case 0xF0: // Event: [Cancel]
      break;
		}
    break;
 	}

  case 0x9: // Current State: [Suspended]
	{
    switch( event_ptr->type )
		{
    case 0x80: // Event: [New]
      break;
    case 0x81: // Event: [PartialFill]
      break;
    case 0x82: // Event: [Fill]
      break;
    case 0x83: // Event: [DoneForDay]
      break;
    case 0x84: // Event: [Canceled]
      break;
    case 0x85: // Event: [Replaced]
      break;
    case 0x86: // Event: [PendingCancel]
      break;
    case 0x87: // Event: [Stopped]
      break;
    case 0x88: // Event: [Rejected]
      break;
    case 0x89: // Event: [Suspended]
      break;
    case 0x8A: // Event: [PendingNew]
      break;
    case 0x8B: // Event: [Calculated]
      break;
    case 0x8C: // Event: [Expired]
      break;
    case 0x8D: // Event: [Reorderd]
      break;
    case 0x8E: // Event: [PendingReplace]
      break;
    case 0x90: // Event: [CancelReject]
      break;
    case 0xD0: // Event: [OrderSingle]
      break;
    case 0xE0: // Event: [CancelReplace]
      break;
		case 0xF0: // Event: [Cancel]
      break;
		}
    break;
 	}

  case 0xA: // Current State: [PendingNew]
	{
    switch( event_ptr->type )
		{
    case 0x80: // Event: [New]
      break;
    case 0x81: // Event: [PartialFill]
      break;
    case 0x82: // Event: [Fill]
      break;
    case 0x83: // Event: [DoneForDay]
      break;
    case 0x84: // Event: [Canceled]
      break;
    case 0x85: // Event: [Replaced]
      break;
    case 0x86: // Event: [PendingCancel]
      break;
    case 0x87: // Event: [Stopped]
      break;
    case 0x88: // Event: [Rejected]
      break;
    case 0x89: // Event: [Suspended]
      break;
    case 0x8A: // Event: [PendingNew]
      break;
    case 0x8B: // Event: [Calculated]
      break;
    case 0x8C: // Event: [Expired]
      break;
    case 0x8D: // Event: [Reorderd]
      break;
    case 0x8E: // Event: [PendingReplace]
      break;
    case 0x90: // Event: [CancelReject]
      break;
    case 0xD0: // Event: [OrderSingle]
      break;
    case 0xE0: // Event: [CancelReplace]
      break;
		case 0xF0: // Event: [Cancel]
      break;
		}
    break;
 	}

  case 0xB: // Current State: [Calculated]
	{
    switch( event_ptr->type )
		{
    case 0x80: // Event: [New]
      break;
    case 0x81: // Event: [PartialFill]
      break;
    case 0x82: // Event: [Fill]
      break;
    case 0x83: // Event: [DoneForDay]
      break;
    case 0x84: // Event: [Canceled]
      break;
    case 0x85: // Event: [Replaced]
      break;
    case 0x86: // Event: [PendingCancel]
      break;
    case 0x87: // Event: [Stopped]
      break;
    case 0x88: // Event: [Rejected]
      break;
    case 0x89: // Event: [Suspended]
      break;
    case 0x8A: // Event: [PendingNew]
      break;
    case 0x8B: // Event: [Calculated]
      break;
    case 0x8C: // Event: [Expired]
      break;
    case 0x8D: // Event: [Reorderd]
      break;
    case 0x8E: // Event: [PendingReplace]
      break;
    case 0x90: // Event: [CancelReject]
      break;
    case 0xD0: // Event: [OrderSingle]
      break;
    case 0xE0: // Event: [CancelReplace]
      break;
		case 0xF0: // Event: [Cancel]
      break;
		}
    break;
 	}

  case 0xC: // Current State: [Expired]
	{
    switch( event_ptr->type )
		{
    case 0x80: // Event: [New]
      break;
    case 0x81: // Event: [PartialFill]
      break;
    case 0x82: // Event: [Fill]
      break;
    case 0x83: // Event: [DoneForDay]
      break;
    case 0x84: // Event: [Canceled]
      break;
    case 0x85: // Event: [Replaced]
      break;
    case 0x86: // Event: [PendingCancel]
      break;
    case 0x87: // Event: [Stopped]
      break;
    case 0x88: // Event: [Rejected]
      break;
    case 0x89: // Event: [Suspended]
      break;
    case 0x8A: // Event: [PendingNew]
      break;
    case 0x8B: // Event: [Calculated]
      break;
    case 0x8C: // Event: [Expired]
      break;
    case 0x8D: // Event: [Reorderd]
      break;
    case 0x8E: // Event: [PendingReplace]
      break;
    case 0x90: // Event: [CancelReject]
      break;
    case 0xD0: // Event: [OrderSingle]
      break;
    case 0xE0: // Event: [CancelReplace]
      break;
		case 0xF0: // Event: [Cancel]
      break;
		}
    break;
 	}

  case 0xD: // Current State: [Reorderd]
  {
    switch( event_ptr->type )
		{
    case 0x80: // Event: [New]
      break;
    case 0x81: // Event: [PartialFill]
      break;
    case 0x82: // Event: [Fill]
      break;
    case 0x83: // Event: [DoneForDay]
      break;
    case 0x84: // Event: [Canceled]
      break;
    case 0x85: // Event: [Replaced]
      break;
    case 0x86: // Event: [PendingCancel]
      break;
    case 0x87: // Event: [Stopped]
      break;
    case 0x88: // Event: [Rejected]
      break;
    case 0x89: // Event: [Suspended]
      break;
    case 0x8A: // Event: [PendingNew]
      break;
    case 0x8B: // Event: [Calculated]
      break;
    case 0x8C: // Event: [Expired]
      break;
    case 0x8D: // Event: [Reorderd]
      break;
    case 0x8E: // Event: [PendingReplace]
      break;
    case 0x90: // Event: [CancelReject]
      break;
    case 0xD0: // Event: [OrderSingle]
      break;
    case 0xE0: // Event: [CancelReplace]
      break;
		case 0xF0: // Event: [Cancel]
      break;
		}
    break;
 	}

  case 0xE: // Current State: [PendingReplace]
	{
    switch( event_ptr->type )
	{
    case 0x80: // Event: [New]
      break;
    case 0x81: // Event: [PartialFill]
      break;
    case 0x82: // Event: [Fill]
      break;
    case 0x83: // Event: [DoneForDay]
      break;
    case 0x84: // Event: [Canceled]
      break;
    case 0x85: // Event: [Replaced]
      break;
    case 0x86: // Event: [PendingCancel]
      break;
    case 0x87: // Event: [Stopped]
      break;
    case 0x88: // Event: [Rejected]
      break;
    case 0x89: // Event: [Suspended]
      break;
    case 0x8A: // Event: [PendingNew]
      break;
    case 0x8B: // Event: [Calculated]
      break;
    case 0x8C: // Event: [Expired]
      break;
    case 0x8D: // Event: [Reorderd]
      break;
    case 0x8E: // Event: [PendingReplace]
      break;
    case 0x90: // Event: [CancelReject]
      break;
    case 0xD0: // Event: [OrderSingle]
      break;
    case 0xE0: // Event: [CancelReplace]
      break;
    case 0xF0: // Event: [Cancel]
      break;
    }
    break;
	}
	}
  //reject();
}

