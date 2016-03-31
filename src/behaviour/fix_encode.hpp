//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__BEHAVIOUR__FIX_ENCODE__HPP
#define GUARD__BEHAVIOUR__FIX_ENCODE__HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <behaviour/fix_encode.hh>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
struct behaviour::fix_encode< E >::impl
{
  using event_type = E;

  //typedef struct
  //{
  //  union
  //  {
  //    buffer_t buffer;
  //    uint8_t raw[ RAW_MSG_SIZE_MAX ];
  //  }
  //  ring[ RING_ENTRY_COUNT ];
  //}
  //buffer_pool_t;


  //event_t *event_ptr;
  //buffer_pool_t *buffer_pool_ptr;
  //buffer_t *buffer_ptr;
  unsigned int idx;

  impl();
  void operator()( event_type &event );
  bool post();
};
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
behaviour::fix_encode< E >::impl::impl()
{
#if 0
  buffer_pool_ptr = raw;
  idx = 0;
#endif
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
void
behaviour::fix_encode< E >::impl::operator()(
  event_type &event 
  )
{
#if 0
  #define APPEND( D, S ) memcpy( ( D ), ( S ), ( sizeof( S ) - 1 ) ); D += ( sizeof( S ) - 1 )

  event_ptr = ( event_t* )*event_ptr;

  buffer_ptr = &buffer_pool_ptr->ring[ ++idx & RING_ENTRY_MASK ].buffer;

  char *data = buffer_ptr->data;
  int size = 0;

  // Encode fix event
  switch( event_ptr->type )
  {
  case 0x80:
    APPEND( data, "35=8;150=0;" );
    break;
	case 0x81:
    APPEND( data, "35=8;150=1;" );
		break;
	case 0x82:
    APPEND( data, "35=8;150=2;" );
		break;
	case 0x83:
    APPEND( data, "35=8;150=3;" );
		break;
	case 0x84:
    APPEND( data, "35=8;150=4;" );
		break;
	case 0x85:
    APPEND( data, "35=8;150=5;" );
	  break;
	case 0x86:
    APPEND( data, "35=8;150=6;" );
	  break;
	case 0x87:
    APPEND( data, "35=8;150=7;" );
	  break;
	case 0x88:
    APPEND( data, "35=8;150=8;" );
	  break;
	case 0x89:
    APPEND( data, "35=8;150=9;" );
	  break;
	case 0x8A:
    APPEND( data, "35=8;150=A;" );
	  break;
	case 0x8B:
    APPEND( data, "35=8;150=B;" );
	  break;
	case 0x8C:
    APPEND( data, "35=8;150=C;" );
	  break;
	case 0x8D:
    APPEND( data, "35=8;150=D;" );
	  break;
	case 0x8E:
    APPEND( data, "35=8;150=E;" );
	  break;
	case 0x90:
    APPEND( data, "35=9;" );
	  break;
	case 0xD0:
    APPEND( data, "35=D;" );
	  break;
	case 0xE0:
    APPEND( data, "35=G;" );
	  break;
	case 0xF0:
    APPEND( data, "35=F;" );
	  break;
  default:
	}

  *event_ptr = ( uint64_t )buffer_ptr;
#endif
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
bool
behaviour::fix_encode< E >::impl::post()
{
  return true;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
behaviour::fix_encode< E >::fix_encode(
  void*
  )
  : pimpl( new impl )
{
  return;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
behaviour::fix_encode< E >::~fix_encode()
{
  delete pimpl;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
void
behaviour::fix_encode< E >::operator()( 
  event_type &event 
  )
{
  return pimpl->operator()( event );
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
bool
behaviour::fix_encode< E >::post()
{
  return pimpl->post();
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
std::string
toString(
  const behaviour::fix_encode< E >& fix_encode_arg
  )
{ 
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif // GUARD__BEHAVIOUR__FIX_ENCODE__HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
