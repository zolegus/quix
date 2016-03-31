//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__BEHAVIOUR__FIX_DECODE__HPP
#define GUARD__BEHAVIOUR__FIX_DECODE__HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <behaviour/fix_decode.hh>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
struct behaviour::fix_decode< E >::impl
{
  //typedef struct
  //{
  //  event_t ring[ RING_ENTRY_COUNT ];
  //}
  //event_pool_t;


  //buffer_t *buffer_ptr;
  //event_pool_t *event_pool_ptr;
  //event_t *event_ptr;
  unsigned int idx;

  impl();
  void operator()( event_type &event );
  bool post();
};
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
behaviour::fix_decode< E >::impl::impl()
{
#if 0
  event_pool_ptr = raw;
  idx = 0;
#endif
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
void
behaviour::fix_decode< E >::impl::operator()(
  event_type &event 
  )
{
#if 0
  buffer_ptr = ( buffer_t* )*event_ptr;

  event_ptr = &event_pool_ptr->ring[ ++idx & RING_ENTRY_MASK ];
  event_ptr->id = idx;  

  // Decode fix event  
  char *fixmsg = buffer_ptr->data;
  while(*fixmsg)
  {

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
		  if( *++fixmsg && ( *fixmsg != ';' ) ) printf( "Bad Fix Event Type\n");
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
		  if( *++fixmsg && ( *fixmsg != ';' ) ) printf( "Bad Fix Event Type\n");
			break;

    default :
      while( *++fixmsg && *fixmsg != ';' ) t += *fixmsg;
      break;
    }

    if(*fixmsg) fixmsg++;
  }

  *event_ptr = ( uint64_t )event_ptr;
#endif
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
bool
behaviour::fix_decode< E >::impl::post()
{
  return true;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
behaviour::fix_decode< E >::fix_decode(
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
behaviour::fix_decode< E >::~fix_decode()
{
  delete pimpl;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
void
behaviour::fix_decode< E >::operator()( 
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
behaviour::fix_decode< E >::post()
{
  return pimpl->post();
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename E >
std::string
toString(
  const behaviour::fix_decode< E >& fix_decode_arg
  )
{ 
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif // GUARD__BEHAVIOUR__FIX_DECODE__HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
