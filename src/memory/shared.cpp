//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <memory/shared.hpp>
#include <stdexcept>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
struct memory::shared::impl
{
  void *data_mem{ nullptr };
	std::size_t size_mem{ 0 };
  int shmid_mem{ 0 };

  impl( const std::size_t, const std::size_t );
  ~impl();
};
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
memory::shared::impl::impl(
  const std::size_t base_arg,
  const std::size_t size_arg
  )
  : size_mem( size_arg )
  
{
  shmid_mem = shmget( IPC_PRIVATE, size_arg, SHM_R | SHM_W );
  if( shmid_mem < 0 )
    throw std::runtime_error( "Failed to create shared memory" );

  data_mem = shmat( shmid_mem, 0 , 0 );
  if( data_mem < 0 )
    throw std::runtime_error( "Failed to attach shared memory" );
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
memory::shared::impl::~impl()
{
  shmctl( shmid_mem, IPC_RMID, 0 );
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
memory::shared::shared(
  const std::size_t base_arg,
  const std::size_t size_arg
  )
  : pimpl( 
      new impl( 
        base_arg, 
        size_arg 
        )
      )
{
  return;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
memory::shared::~shared()
{
  delete pimpl;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
void*
memory::shared::data( 
  const std::size_t offset_arg 
  )
{
  return reinterpret_cast< uint8_t* >( pimpl->data_mem ) + offset_arg;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
std::size_t
memory::shared::size( 
  const std::size_t offset_arg 
  )
{
  return pimpl->size_mem - offset_arg;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
std::string
toString(
  const memory::shared& shared_arg
  )
{ 
  throw std::runtime_error( "Unimplemented" );
  return ""; 
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
