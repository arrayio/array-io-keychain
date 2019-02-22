//
// Created by user on 2/13/19.
//

#ifndef KEYCHAINAPP_WIDGET_SINGLETON_H
#define KEYCHAINAPP_WIDGET_SINGLETON_H

#include <memory>
#include "secmod_protocol.hpp"

namespace sm_cmd = keychain_app::secmod_commands;

struct widget_singleton
{
    static auto& instance()
    {
        static widget_singleton instance;
        return instance;
    };
private:

    widget_singleton(){};
    ~widget_singleton(){};
};


struct shared_event
{
    template<sm_cmd::events_te  T>
    static std::shared_ptr<typename sm_cmd::secmod_event<T>::params_t>& ptr()
    {
        static std::shared_ptr<typename sm_cmd::secmod_event<T>::params_t> shared;
        return shared;
    }

    static sm_cmd::events_te& event_num()
    {
        static  sm_cmd::events_te event;
        return event;
    }
};


#endif //KEYCHAINAPP_WIDGET_SINGLETON_H
