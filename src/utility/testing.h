#include <stdexcept>
#include <string>
#include <typeinfo>

#define __S1__( X ) #X
#define __S2__( X ) __S1__( X )

#define FAILURE( X )			           \
  std::logic_error(                                \
    std::string( typeid(this).name() ) +           \
    "::" +                                         \
    __func__  +                                    \
    " @ " __FILE__ "(" __S2__(__LINE__) ") "       \
    "[" __S1__( X ) "]"				   \
  )                                                \

#define PASSES( X )                   \
  do                                  \
  {                                   \
    if( !( X ) )                      \
      throw FAILURE( X );	      \
  }                                   \
  while( false )                      \

#define FAILS( X )                    \
  do                                  \
  {                                   \
    if( ( X ) )                       \
      throw FAILURE( X );	      \
  }                                   \
  while( false )                      \

#define CATCH( X, E, W )                     		\
  do                                          		\
  {                                           		\
    try                                       		\
    {                                         		\
      X;                                      		\
      FAILS( __S2__(E) "not thrown" );       		\
    }                                         		\
    catch( const E& e  )                      		\
    {					      		\
      PASSES( std::string( W ) == e.what() );  		\
    }                                         		\
    catch( const std::exception& e  )	      		\
    {                                         		\
      throw FAILURE(			      		\
	std::string(                                    \
          "Unexpected std::exception thrown  -- " )	\
          + e.what()					\
	);			              		\
    }                                         		\
    catch( ...  )                             		\
    {                                         		\
      throw FAILURE(			      		\
	std::string(					\
          "Unexpected non std::exception thrown" )      \
      );                  		      		\
    }                                         		\
  }                                           		\
  while( false )                              		\

int main(){};
