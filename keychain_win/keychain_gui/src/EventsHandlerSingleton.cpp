#include <boost/hana.hpp>

#include "EventsHandlerSingleton.h"
#include "EventsHandlers.h"

const EventHandlerSingleton& EventHandlerSingleton::instance()
{
  static EventHandlerSingleton handler;
  return handler;
}

const EventHandlerSingleton::command_ptr EventHandlerSingleton::operator[](sm_cmd::events_te etype) const
{
  size_t ind = static_cast<size_t>(etype);
  if (ind >= m_command_list.size())
    return m_command_list[0];
  return m_command_list[ind];
}

EventHandlerSingleton::EventHandlerSingleton()
{
  m_command_list.reserve(32);//TODO: it is may be possible to get size info from boost::hana::tuple
  boost::hana::for_each(event_handlers_static_list, [&](auto val) {
    using value_type = decltype(val);
    constexpr auto const_val = static_cast<command_te>(value_type::value);
    m_command_list.push_back(command_ptr(new keychain_command<const_val>));
  });
}

sm_cmd::events_te parseSecmodEvents(const std::string& json)
{
  try
  {
    auto& eventHandler = EventHandlerSingleton::instance();
    sm_cmd::secmod_parser_f parser;
    auto etype = parser(json);
    auto pFunctor = eventHandler[etype];
    (*pFunctor)(parser);
    return etype;
  }
  FC_LIGHT_CAPTURE_AND_RETHROW((json))
}
