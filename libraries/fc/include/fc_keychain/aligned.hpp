#pragma once
namespace fc_keychain {

  template<unsigned int S, typename T=double>
  struct aligned {
    union {
      T    _align;
      char _data[S];
    } _store;
    operator char*()            { return _store._data; }
    operator const char*()const { return _store._data; }
  };

}
