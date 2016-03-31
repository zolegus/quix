//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__UTILITY__CONFIGURATION__HH
#define GUARD__UTILITY__CONFIGURATION__HH
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <utility/configuration.h>
#include <string>
#include <istream>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
class utility::configuration
{
  struct impl;
  impl *pimpl;

public:

  using key_type = std::string;
  using value_type = std::string;
  using stream = std::istream;

  configuration( int, char *[] );
  configuration( stream&& );
  configuration( const configuration& );
  configuration( configuration&& );
  configuration& operator=( configuration );
  configuration& operator=( configuration&& );
  ~configuration();

	const value_type& operator[]( const key_type& );
  template< typename T > T get( const key_type& );
  auto begin() const;
  auto end() const;
};
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif // GUARD__UTILITY__CONFIGURATION__HH
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
