//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__NETWORK__SHARED_SOCKET__HH
#define GUARD__NETWORK__SHARED_SOCKET__HH
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <network/shared_socket.h>
#include <network/socket_connected.h>
#include <network/address_inet.h>
#include <memory>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
class network::shared_socket
{
  struct impl;
  impl *pimpl;

public:

  using socket_type = socket_connected< address_inet >;
  using socket_type_ptr = std::shared_ptr< socket_type >;

  shared_socket( const std::string& );
  shared_socket( const shared_socket& ) = delete;
  shared_socket( shared_socket&& ) = delete;
  shared_socket& operator=( shared_socket ) = delete;
  shared_socket& operator=( shared_socket&& ) = delete;
  ~shared_socket();

	socket_type_ptr operator()();
};
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif // GUARD__NETWORK__SHARED_SOCKET__HH
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
