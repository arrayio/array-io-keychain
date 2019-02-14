//
// Created by user on 2/13/19.
//

#ifndef KEYCHAINAPP_WIDGET_SINGLETON_H
#define KEYCHAINAPP_WIDGET_SINGLETON_H

#include <memory>

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


template<typename T>
struct event_singleton
{
    static std::shared_ptr<T>& instance(T&& a)
    {
        shared.reset(new T(a));
        return shared;
    }
    static std::shared_ptr<T> shared;

private:
    event_singleton(){};
    ~event_singleton(){};
};

#endif //KEYCHAINAPP_WIDGET_SINGLETON_H
