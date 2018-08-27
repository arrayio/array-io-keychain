#include <fc_light/thread/spin_yield_lock.hpp>
#include <fc_light/time.hpp>
#include <boost/atomic.hpp>
#include <boost/memory_order.hpp>
#include <new>

namespace fc_light {
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

  void spin_yield_lock::unlock() {
    define_self;
    self->store(unlocked, boost::memory_order_release);
  }
  #undef define_self

} // namespace fc_light
