//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__NETWORK__ADDRESS_INET__HH
#define GUARD__NETWORK__ADDRESS_INET__HH
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <network/address_inet.h>
#include <netinet/in.h>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
class network::address_inet
{
  sockaddr_in sockaddr_in_mem;

public:

  address_inet( const std::string& );
  const struct sockaddr *addr();
	std::size_t size();
};
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif // GUARD__NETWORK__ADDRESS_INET__HH
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
