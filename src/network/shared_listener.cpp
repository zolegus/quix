//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <network/shared_listener.hpp>
#include <network/socket_listening.hpp>
#include <network/socket_connected.hpp>
#include <network/address_inet.hpp>
#include <network/address_unix.hpp>
#include <utility>
#include <stdexcept>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
struct network::shared_listener::impl
{
  socket_listening< address_inet > listening_inet;
  socket_connected< address_unix > connected_unix_receiver;
  socket_connected< address_unix > connected_unix_sender;

  impl( const std::string&, const std::string&, const std::string& );
};
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
network::shared_listener::impl::impl(
  const std::string &listening_inet_arg,
	const std::string &connected_unix_receiver_arg,
	const std::string &connected_unix_sender_arg
  )
  : listening_inet( listening_inet_arg )
  , connected_unix_receiver( connected_unix_receiver_arg )
  , connected_unix_sender( connected_unix_sender_arg )
{
  return;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
network::shared_listener::shared_listener(
	const std::string &listening_inet_arg,
	const std::string &receiving_unix_arg,
	const std::string &sending_unix_arg
  )
  : pimpl( 
			new impl(
        listening_inet_arg,
				receiving_unix_arg,
				sending_unix_arg
				) 
      )
{
  return;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
network::shared_listener::~shared_listener()
{
  delete pimpl;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
void
network::shared_listener::operator()()
{
  int fd = pimpl->listening_inet.accept();
  pimpl->connected_unix_receiver.send_fd_checked(  0xCAFEBABEDEADBEEF, fd );
  pimpl->connected_unix_sender.send_fd_checked( 0xCAFEBABEDEADBEEF, fd );
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
std::string
toString(
  const network::shared_listener& shared_listenerarg
  )
{ 
  throw std::runtime_error( "Unimplemented" );
  return ""; 
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
