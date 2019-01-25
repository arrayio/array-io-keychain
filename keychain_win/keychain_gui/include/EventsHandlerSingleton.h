#pragma once

#include <keychain_lib/secmod_parser_cmd.hpp>

namespace sm_cmd = keychain_app::secmod_commands;

struct EventHandlerBase
{
  EventHandlerBase(sm_cmd::events_te type) : e_type(type) {}
  virtual ~EventHandlerBase() {}
  sm_cmd::events_te e_type;
  virtual void operator()(const sm_cmd::secmod_parser_f& parser) const = 0;
};

sm_cmd::events_te parseSecmodEvents(const std::string& json);

struct EventHandlerSingleton
{
  using event_handler_ptr = std::shared_ptr<EventHandlerBase>;
  static const EventHandlerSingleton& instance();
  const event_handler_ptr operator[](sm_cmd::events_te etype) const;
private:
  std::vector<event_handler_ptr> m_command_list;
  EventHandlerSingleton();
};