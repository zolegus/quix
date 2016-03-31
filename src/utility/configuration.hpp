//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef GUARD__UTILITY__CONFIGURATION__HPP
#define GUARD__UTILITY__CONFIGURATION__HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <utility/configuration.hh>
#include <vector>
#include <utility>
#include <cctype>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
template< typename T >
T
utility::configuration::get(
  const key_type& key_arg
	)
{
  return (*this)[ key_arg ];
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
namespace utility {
template<>
inline
const char*
configuration::get< const char* >(
  const key_type& key_arg
	)
{
  return (*this)[ key_arg ].c_str();
}}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
namespace utility {
template<>
inline
int
configuration::get< int >(
  const key_type& key_arg
	)
{
  return std::atoi( (*this)[ key_arg ].c_str() );
}}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
namespace utility {
template<>
inline
unsigned int
configuration::get< unsigned int >(
  const key_type& key_arg
	)
{
  return get< int >( key_arg );
}}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
namespace utility {
template<>
inline
unsigned short
configuration::get< unsigned short >(
  const key_type& key_arg
	)
{
  return get< int >( key_arg );
}}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
namespace utility {
template<>
inline
std::size_t
configuration::get< std::size_t >(
  const key_type& key_arg
	)
{
  return get< int >( key_arg );
}}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <iostream>
namespace utility {
template<>
inline
std::vector< unsigned int >
configuration::get< std::vector< unsigned int > >(
  const key_type& key_arg
	)
{
  std::vector< unsigned int > vec;
  unsigned int v = 0;
  bool commit = false;
  for( const char d : (*this)[ key_arg ] )
	{
    if( std::isdigit( d ) )
		{
      commit = true;
      v = ( v * 10 ) + ( d - '0' );
			continue;
		}

    if( d == ',' )
		{
			//if( commit ) std::cout << "for " << key_arg << "=" << (*this)[ key_arg ] << " push_back( " << v << " )\n";
      if( commit ) vec.push_back( v );
      v = 0;
      commit = false;
			continue;
		}

    if( std::isblank( d ) ) 
   	{
      continue;
		}

		throw std::logic_error( "Invalid configuration file entry" );
	}

	//if( commit ) std::cout << "for " << key_arg << "=" << (*this)[ key_arg ] << " push_back( " << v << " )\n";
  if( commit ) vec.push_back( v );
  return vec;
}}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
namespace utility {
template<>
inline
std::vector< std::pair< unsigned int, unsigned int > >
configuration::get< std::vector< std::pair< unsigned int, unsigned int > > >(
  const key_type& key_arg
	)
{
  std::vector< std::pair< unsigned int, unsigned int > > vec;
  unsigned int i = 0, v[2] = {0,0};
  bool commit = false;
  for( const char d : (*this)[ key_arg ] )
	{
    if( std::isdigit( d ) )
		{
      commit = true;
      v[i] = ( v[i] * 10 ) + ( d - '0' );
			continue;
		}

    if( d == '.' && !i )
		{
      i = !i;
			continue;
		}

    if( d == ',' && i )
		{
      i = !i;
			//if( commit ) std::cout << "for " << key_arg << "=" << (*this)[ key_arg ] << " push_back( { " << v[0] << "," << v[1] << " } )\n";
      if( commit ) vec.push_back( std::make_pair( v[0], v[1] ) );
 		  v[0] = v[1] = 0;
      commit = false;
			continue;
		}

    if( std::isblank( d ) ) 
   	{
      continue;
		}

		throw std::logic_error( "Invalid configuration file entry" );
	}
	//if( commit ) std::cout << "for " << key_arg << "=" << (*this)[ key_arg ] << " push_back( { " << v[0] << "," << v[1] << " } )\n";
  if( commit ) vec.push_back( std::make_pair( v[0], v[1] ) );
  return vec;
}}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#endif // GUARD__UTILITY__CONFIGURATION__HPP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
