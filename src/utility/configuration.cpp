//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <utility/configuration.hpp>
#include <unordered_map>
#include <utility>
#include <istream>
#include <fstream>
#include <sstream>
#include <algorithm> 
#include <functional> 
#include <cctype>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
namespace {
std::ifstream
make_ifstream( 
  int argc_arg,
  char *argv_arg[]
  )
{
	std::ifstream stream;
	std::string filename(
    argc_arg == 1 ? ( std::string( argv_arg[0] ) + ".cfg" ) :
    argc_arg == 2 ? ( std::string( argv_arg[1] ) )
                  : throw std::runtime_error( "invalid command line arguments" )
		);
  stream.open( filename );
  if( !stream.is_open() ) throw std::runtime_error( "Failed to open configuration file" );

  return stream;
}}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
namespace {
std::string&
ltrim(
  std::string &s
  ) 
{
  s.erase(
    s.begin(), 
    std::find_if(
      s.begin(), 
      s.end(), 
      std::not1(
        std::ptr_fun<int, int>( 
          std::isblank 
          ) 
        )
      )
    );
  return s;
}}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
namespace {
std::string&
rtrim(
  std::string &s
  ) 
{
  s.erase(
    std::find_if(
      s.rbegin(), 
      s.rend(), 
      std::not1(
        std::ptr_fun<int, int>(
          std::isblank
          )
        )
			).base(),
    s.end()
	  );
  return s;
}}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
struct utility::configuration::impl
{
  using dictionary_type =
    std::unordered_map<
      key_type,
      value_type
    >;

  dictionary_type dictionary_mem;
};
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
utility::configuration::configuration(
  int argc_arg,
  char *argv_arg[]
  )
	: configuration( 
 			make_ifstream( 
        argc_arg, 
        argv_arg 
        )
      )
{
  return;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
utility::configuration::configuration(
  stream&& stream_arg
  )
  : pimpl( new impl )
{
  std::string line;
	while( !getline( stream_arg, line ).eof() )
	{
    if( line.empty() ) continue;
    ltrim( rtrim( line ) );
    if( line[0] == '#' ) continue;
	  const auto equals_pos = line.find( "=" );
	  if( equals_pos == std::string::npos ) continue;
    const auto key = line.substr( 0, equals_pos );
		const auto value = line.substr( equals_pos + 1 );
		pimpl->dictionary_mem[ key ] = value;
	}
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
utility::configuration::configuration(
  const configuration& other
  ) 
  : pimpl( 
      new impl( 
        *other.pimpl ) )
{
  return;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
utility::configuration::configuration(
  configuration&& other
  ) : pimpl( other.pimpl )
{
  other.pimpl = nullptr;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
utility::configuration&
utility::configuration::operator=(
  configuration other
  )
{
  std::swap( pimpl, other.pimpl );
  return *this;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
utility::configuration&
utility::configuration::operator=(
  configuration&& other
  )
{
  std::swap( pimpl, other.pimpl );
  return *this;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
utility::configuration::~configuration()
{
  delete pimpl;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
const utility::configuration::value_type&
utility::configuration::operator[](
  const key_type& key
  )
{
  const auto it =  pimpl->dictionary_mem.find( key );
  if( it != pimpl->dictionary_mem.cend() )
  {
  	return it->second;
  }

  const auto what = key + " not found in configuration";
  throw( std::invalid_argument( what ) );
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
auto
utility::configuration::begin() const
{
  return pimpl->dictionary_mem.begin();
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
auto
utility::configuration::end() const
{
  return pimpl->dictionary_mem.end();
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
std::string
toString(
  const utility::configuration& configuration_arg
  )
{ 
  std::stringstream ss;
  ss << "{";
  for( auto pair : configuration_arg )
  {
	  const auto& key = pair.first;
  	const auto& value = pair.second;
		ss << "{\"" << key << "\":\"" << value << "\"}";
	}
	ss << "}";
	return ss.str();
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
