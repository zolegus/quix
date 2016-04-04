//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__NETWORK__SOCKET_CONNECTED__HPP
#define GUARD__NETWORK__SOCKET_CONNECTED__HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <network/socket_connected.hh>
#include <network/socket_listening.hh>
#include <utility>
#include <stdexcept>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <unistd.h> // write/close
#include <errno.h> //errno
#include <string.h> //strerror
#include <sys/socket.h>
#include <netinet/in.h> //sockaddr_in
#include <sys/un.h>
#include <arpa/inet.h>
#include <signal.h>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename A > 
struct network::socket_connected< A >::impl
{
  using address_type = A;

  int fd_mem{ 0 };

  impl( const int );
  impl( const std::string& );
  ~impl();

  std::size_t recv( char *const, const std::size_t );
  std::size_t send( const char *const, const std::size_t );
  std::size_t recv_fd( char *const, const std::size_t, int& );
  std::size_t send_fd( const char *const, const std::size_t, const int );
  int recv_fd_checked( const uint64_t);
  void send_fd_checked( const uint64_t,	const int );
};
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename A > 
network::socket_connected< A >::impl::impl(
  const int fd_arg
  )
	: fd_mem( fd_arg )
{
  return;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename A > 
network::socket_connected< A >::impl::impl(
  const std::string &address_arg
  )
{
	address_type address( address_arg );

  if( ( fd_mem = socket( address.addr()->sa_family, SOCK_STREAM, 0 ) ) < 0 )
  {
		std::string what( "Failed create listen socket for port: " );
    throw std::runtime_error( ( what + strerror( errno ) ).c_str() );
  }

	if( connect( fd_mem, address.addr(), address.size() ) < 0 )
  {
    close( fd_mem );
		std::string what( "Failed to make outgoing connection using: " );
    what += "[" + address_arg + "] ";
    throw std::runtime_error( ( what + strerror( errno ) ).c_str() );
  }
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename A > 
std::size_t 
network::socket_connected< A >::impl::recv( 
  char *const buff_arg,
  const std::size_t size_arg
  )
{
  return read( fd_mem, buff_arg, size_arg );
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename A > 
std::size_t
network::socket_connected< A >::impl::send( 
  const char *const buff_arg, 
  const std::size_t size_arg
  )
{
  return write( fd_mem, buff_arg, size_arg );
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename A > 
std::size_t
network::socket_connected< A >::impl::recv_fd(
  char *const buff_arg,
  const std::size_t size_arg,
  int &fd_arg
)
{
  struct msghdr msg;
  struct iovec iov[1];

  union
  {
    struct cmsghdr cm;
    char control[ CMSG_SPACE( sizeof( int ) ) ];
  }
  control_un;

  struct cmsghdr *cmptr;

  msg.msg_control = control_un.control;
  msg.msg_controllen = sizeof( control_un.control );
  
  msg.msg_name = 0;
  msg.msg_namelen = 0;

  iov[ 0 ].iov_base = buff_arg;
  iov[ 0 ].iov_len = size_arg;
  msg.msg_iov = iov;
  msg.msg_iovlen = 1;

  std::size_t size_recvd = recvmsg( fd_mem, &msg, 0 );
  if( size_recvd <= 0 )
  {
    std::string what( "Failed to receive socket descriptor: " );
    throw std::runtime_error( ( what + strerror( errno ) ).c_str() );
  }

  if( !( ( cmptr = CMSG_FIRSTHDR( &msg ) ) != 0 && cmptr->cmsg_len == CMSG_LEN( sizeof( int ) ) ) )
  {
    std::string what( "Failed to receive socket descriptor: " );
    throw std::runtime_error( ( what + strerror( errno ) ).c_str() );
  }

  if( cmptr->cmsg_level != SOL_SOCKET ) 
  {
    std::string what( "Control level not SOL_SOCKET: " );
    throw std::runtime_error( ( what + strerror( errno ) ).c_str() );
  }

  if( cmptr->cmsg_type != SCM_RIGHTS ) 
  {
    std::string what( "Control type not SCM_RIGHTS: " );
    throw std::runtime_error( ( what + strerror( errno ) ).c_str() );
  }

  fd_arg =  *( ( int* ) CMSG_DATA( cmptr ) );
  return size_recvd;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename A > 
std::size_t
network::socket_connected< A >::impl::send_fd(
  const char *const buff_arg, 
  const std::size_t size_arg,
  const int fd_arg
  )
{
  void *ptr;
  size_t nbytes;

  struct msghdr msg;
  struct iovec iov[1];

  union
  {
    struct cmsghdr cm;
    char control[ CMSG_SPACE( sizeof( int ) ) ];
  }
  control_un;

  struct cmsghdr *cmptr;

  msg.msg_control = control_un.control;
  msg.msg_controllen = sizeof( control_un.control );

  cmptr = CMSG_FIRSTHDR( &msg );
  cmptr->cmsg_len = CMSG_LEN( sizeof( int ) );
  cmptr->cmsg_level = SOL_SOCKET;
  cmptr->cmsg_type = SCM_RIGHTS;
  *( ( int* ) CMSG_DATA( cmptr ) ) = fd_arg;

  msg.msg_name = 0;
  msg.msg_namelen = 0;

  iov[ 0 ].iov_base = (void*)buff_arg;
  iov[ 0 ].iov_len = size_arg;
  msg.msg_iov = iov;
  msg.msg_iovlen = 1;

  return sendmsg( fd_mem, &msg, 0 );
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename A > 
int 
network::socket_connected< A >::impl::recv_fd_checked( 
  const uint64_t id_arg
  )
{
  uint64_t id;
  int fd;
  recv_fd( reinterpret_cast< char* >( &id ), sizeof( uint64_t ), fd );
  if( id == id_arg ) return fd;
  throw std::runtime_error( "Invalid fd received" );
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename A > 
void 
network::socket_connected< A >::impl::send_fd_checked( 
  const uint64_t id_arg,
	const int fd_arg
  )
{
  send_fd( reinterpret_cast< const char* >( &id_arg ), sizeof( id_arg ), fd_arg );
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename A > 
network::socket_connected< A >::impl::~impl(
  )
{
  close( fd_mem );
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename A > 
network::socket_connected< A >::socket_connected(
  const int fd_arg
  )
	: pimpl( 
			new impl(
			  fd_arg
        )
      )
{
  return;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename A > 
network::socket_connected< A >::socket_connected(
  const std::string &address_arg
  )
  : pimpl( 
			new impl(
					address_arg
  			)
      )
{
  return;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename A > 
network::socket_connected< A >::~socket_connected()
{
  delete pimpl;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename A > 
std::size_t 
network::socket_connected< A >::recv( 
  char *const buff_arg,
  const std::size_t size_arg
  )
{
  return pimpl->recv( buff_arg, size_arg );
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename A > 
std::size_t
network::socket_connected< A >::send( 
  const char *const buff_arg, 
  const std::size_t size_arg
  )
{
  return pimpl->send( buff_arg, size_arg );
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename A > 
std::size_t 
network::socket_connected< A >::recv_fd(
  char *const buff_arg,
  const std::size_t size_arg,
  int &fd_arg
)
{
  return pimpl->recv_fd( buff_arg, size_arg, fd_arg );
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename A > 
std::size_t 
network::socket_connected< A >::send_fd( 
  const char *const buff_arg, 
  const std::size_t size_arg,
  const int fd_arg
  )
{
  return pimpl->send_fd( buff_arg, size_arg, fd_arg );
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename A > 
int 
network::socket_connected< A >::recv_fd_checked( 
  const uint64_t id_arg
  )
{
  return pimpl->recv_fd_checked( id_arg );
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename A > 
void 
network::socket_connected< A >::send_fd_checked( 
  const uint64_t id_arg,
	const int fd_arg
  )
{
  return pimpl->send_fd_checked( id_arg, fd_arg );
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename A > 
std::string
toString(
const network::socket_connected< A >& socket_connected_arg
  )
{
  throw std::runtime_error( "Unimplemented" );
  return ""; 
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif // GUARD__NETWORK__SOCKET_CONNECTED__HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
