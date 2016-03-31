//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__NODE__PROCESSOR__H
#define GUARD__NODE__PROCESSOR__H
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <string>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
namespace node
{
  template< typename T, typename B > class processor;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename T, typename B > std::string toString( const node::processor< T, B >& );
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif // GUARD__NODE__PROCESSOR__H
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
