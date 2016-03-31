//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <utility/configuration.cpp>
#include <utility/testing.h>
#include <sstream>
#include <memory>
#include <iostream>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
namespace {
  auto make_setting_stringstream( 
    std::size_t strings = 0, 
    std::size_t ints = 0,
    std::size_t vecs = 0,
    std::size_t vecpairs = 0
    )
  {
    std::stringstream settings_stringstream;

    for( int i = 1 ; i < strings + 1; ++i )
    {
      settings_stringstream << "skey" << i << "=value" << i << "\n";
    }

    for( int i = 1 ; i < ints + 1; ++i )
    {
      settings_stringstream << "ikey" << i << "=" << i << "\n";
    }

    for( int i = 1 ; i < vecs + 1; ++i )
    {
      settings_stringstream << "vkey" << i << "=";
			std::string comma;
      for( int t = 1; t < i + 1; ++t )
			{
        settings_stringstream << comma << t;
        comma = ',';
			}
      settings_stringstream << "\n";
    }

    for( int i = 1 ; i < vecpairs + 1; ++i )
    {
      settings_stringstream << "pkey" << i << "=";
			std::string comma;
      for( int t = 1; t < i + 1; ++t )
			{
        settings_stringstream << comma << t << "." << ( t + 1 );
        comma = ',';
			}
      settings_stringstream << "\n";
    }

		std::cout << settings_stringstream.str() << std::endl;
    return settings_stringstream;
  }
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
struct plumbing
{
  plumbing()
  {
    structors_stack( 0, 9 );
    structors_heap( 0, 9 );
    copy_constructor();
    move_constructor();
    copy_assignment();
    move_assignment();
  }

  void structors_stack( std::size_t from, std::size_t to )
  {
    for( int i = from ; i <= to ; ++i )
    {
      auto cfg = utility::configuration( make_setting_stringstream( i ) );
    }
  }

  void structors_heap( std::size_t from, std::size_t to )
  {
    for( int i = from ; i <= to ; ++i )
    {
      auto cfg = std::make_unique<utility::configuration>( make_setting_stringstream( i ) );
    }
  }

  void copy_constructor()
  {
  }

  void move_constructor()
  {
  }

  void copy_assignment()
  {
  }

  void move_assignment()
  {
  }
};
//plumbing;
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
struct api
{
  api()
  {
    operator_indicies();
    get();
  }

  void operator_indicies()
  {
		utility::configuration cfg( make_setting_stringstream( 2, 2, 2, 2 ) );

    PASSES( cfg[ "skey1" ] == "value1" );
    PASSES( cfg[ "skey2" ] == "value2" );
    CATCH( cfg[ "skey3" ] == "value3", 
           std::invalid_argument, 
           "skey3 not found in configuration" );

    PASSES( cfg[ "ikey1" ] == "1" );
    PASSES( cfg[ "ikey2" ] == "2" );
    CATCH( cfg[ "ikey3" ] == "3", 
           std::invalid_argument, 
           "ikey3 not found in configuration" );

    PASSES( cfg[ "vkey1" ] == "1" );
    PASSES( cfg[ "vkey2" ] == "1,2" );
    CATCH( cfg[ "vkey3" ] == "1,2,3", 
           std::invalid_argument, 
           "vkey3 not found in configuration" );

    PASSES( cfg[ "pkey1" ] == "1.2" );
    PASSES( cfg[ "pkey2" ] == "1.2,2.3" );
    CATCH( cfg[ "pkey3" ] == "1.2,2.3,3.4", 
           std::invalid_argument, 
           "pkey3 not found in configuration" );
  }

  void get()
	{
		utility::configuration cfg( make_setting_stringstream( 2, 2, 2, 2 ) );

    PASSES( cfg.get< std::string >( "skey1" ) == "value1" );
    PASSES( cfg.get< std::string >( "skey2" ) == "value2" );
    CATCH( cfg.get< std::string >( "skey3" ) == "value3", 
           std::invalid_argument, 
           "skey3 not found in configuration" );

    PASSES( cfg.get< std::size_t >( "ikey1" ) == 1 );
    PASSES( cfg.get< std::size_t >( "ikey2" ) == 2 );
    CATCH( cfg.get< std::size_t >( "ikey3" ) == 3, 
           std::invalid_argument, 
           "ikey3 not found in configuration" );

    using vec = std::vector< unsigned int >;
		PASSES( cfg.get< vec >( "vkey1" ).size() == 1 );
		PASSES( cfg.get< vec >( "vkey1" )[ 0 ] == 1 );
 	  PASSES( cfg.get< vec >( "vkey2" ).size() == 2 );
 	  PASSES( cfg.get< vec >( "vkey2" )[ 0 ] == 1 );
 	  PASSES( cfg.get< vec >( "vkey2" )[ 1 ] == 2 );
    CATCH( cfg.get< vec >( "vkey3" ).size() == 3, std::invalid_argument, "vkey3 not found in configuration" );

    using pvec = std::vector< std::pair< unsigned int, unsigned int > >;
		PASSES( cfg.get< pvec>( "pkey1" ).size() == 1 );
		PASSES( cfg.get< pvec>( "pkey1" )[ 0 ].first == 1 );
		PASSES( cfg.get< pvec>( "pkey1" )[ 0 ].second == 2 );
  	PASSES( cfg.get< pvec >( "pkey2" ).size() == 2 );
  	PASSES( cfg.get< pvec >( "pkey2" )[ 0 ].first == 1 );
  	PASSES( cfg.get< pvec >( "pkey2" )[ 0 ].second == 2 );
  	PASSES( cfg.get< pvec >( "pkey2" )[ 1 ].first == 2 );
  	PASSES( cfg.get< pvec >( "pkey2" )[ 1 ].second == 3 );
    CATCH( cfg.get< pvec >( "pkey3" ).size() == 3, std::invalid_argument, "pkey3 not found in configuration" );
	}
}
api;
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
struct scenarios
{
  scenarios()
  {
  }

}
scenarios;
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
