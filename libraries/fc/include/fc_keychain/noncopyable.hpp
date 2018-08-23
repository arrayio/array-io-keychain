#pragma once

namespace fc_keychain
{
   class noncopyable
   {
      public:
         noncopyable(){}
      private:
         noncopyable( const noncopyable& ) = delete;
         noncopyable& operator=(  const noncopyable& ) = delete;
   };
}

