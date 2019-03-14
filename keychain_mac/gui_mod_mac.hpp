//
//  gui_mod_mac.hpp
//  keychain
//
//  Created by Mikhail Lutskiy on 28/01/2019.
//

#ifndef gui_mod_mac_h
#define gui_mod_mac_h

#include <keychain_lib/keychain.hpp>

namespace keychain_app
{
    
    class gui_mod_mac: public gui_mod_base
    {
        public:
        gui_mod_mac();
        virtual ~gui_mod_mac();
        virtual dev::Public select_key() const override;
        virtual bool entropy() const override;
    };
    
}


#endif /* gui_mod_mac_h */
