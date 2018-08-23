#include <fc_keychain/thread/spin_yield_lock.hpp>
#include <fc_keychain/time.hpp>
#include <boost/atomic.hpp>
#include <boost/memory_order.hpp>
#include <new>

namespace fc_keychain {
  void yield();

  #define define_self  boost::atomic<int>* self = (boost::atomic<int>*)&_lock

  spin_yield_lock::spin_yield_lock() 
  {
     define_self;
     new (self) boost::atomic<int>();
     static_assert( sizeof(boost::atomic<int>) == sizeof(_lock), "" );
     self->store(unlocked);
  }

  bool spin_yield_lock::try_lock() {
    define_self;
    return self->exchange(locked, boost::memory_order_acquire)!=locked;
  }
/*
  bool spin_yield_lock::try_lock_for( const fc_keychain::microseconds& us ) {
    return try_lock_until( fc_keychain::time_point::now() + us );
  }

  bool spin_yield_lock::try_lock_until( const fc_keychain::time_point& abs_time ) {
     while( abs_time > time_point::now() ) {
        if( try_lock() ) 
           return true;
        yield(); 
     }
     return false;
  }

  void spin_yield_lock::lock() {
    define_self;
    while( self->exchange(locked, boost::memory_order_acquire)==locked) {
      yield(); 
    }
  }
*/
  void spin_yield_lock::unlock() {
    define_self;
    self->store(unlocked, boost::memory_order_release);
  }
  #undef define_self

} // namespace fc_keychain
