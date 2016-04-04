//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__NETWORK__SOCKET_CONNECTED__HH
#define GUARD__NETWORK__SOCKET_CONNECTED__HH
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <network/socket_connected.h>
#include <network/socket_listening.h>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename A > 
class network::socket_connected
{
  struct impl;
  impl *pimpl;

public:

  socket_connected( const int );
  socket_connected( const std::string& );
  socket_connected( const socket_connected& ) = delete;
  socket_connected( socket_connected&& ) = delete;
  socket_connected& operator=( socket_connected ) = delete;
  socket_connected& operator=( socket_connected&& ) = delete;
  ~socket_connected();

  std::size_t recv( char *const, const std::size_t );
  std::size_t send( const char *const, const std::size_t );
  std::size_t recv_fd( char *const, const std::size_t, int& );
  std::size_t send_fd( const char *const, const std::size_t, const int );
  int recv_fd_checked( const uint64_t);
  void send_fd_checked(	const uint64_t, const int );
};
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif // GUARD__NETWORK__SOCKET_CONNECTED__HH
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
