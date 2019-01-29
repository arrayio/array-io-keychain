#pragma once

#include <boost/hana.hpp>

#include <fc_light/exception/exception.hpp>
#include "keychain_gui_win.h"
#include "EventsHandlerSingleton.h"

template <sm_cmd::events_te etype>
struct EventHandler : EventHandlerBase
{
  EventHandler() :EventHandlerBase(static_cast<sm_cmd::events_te>(etype)) {}
  virtual ~EventHandler() {}
  virtual void operator()(const sm_cmd::secmod_parser_f& parser) const override
  {
    FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Secmod command is not found, etype = %{ETYPE}", ("ETYPE", e_type));
  }
};

template <>
struct EventHandler< sm_cmd::events_te::create_key> : EventHandlerBase
{
  EventHandler() :EventHandlerBase(static_cast<sm_cmd::events_te>(sm_cmd::events_te::create_key)) {}
  virtual ~EventHandler() {}
  virtual void operator()(const sm_cmd::secmod_parser_f& parser) const override
  {
    try
    {
      auto cmd = parser.params< sm_cmd::events_te::create_key >();
      FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Secmod command is not implemented, etype = %{ETYPE}", ("ETYPE", e_type));
      //TODO: need to implement
    }
    FC_LIGHT_CAPTURE_AND_RETHROW()
  }
};

template <>
struct EventHandler < sm_cmd::events_te::sign_hex > : EventHandlerBase
{
  EventHandler() :EventHandlerBase(static_cast<sm_cmd::events_te>(sm_cmd::events_te::sign_hex)) {}
  virtual ~EventHandler() {}
  virtual void operator()(const sm_cmd::secmod_parser_f& parser) const override
  {
    try
    {
      auto cmd = parser.params< sm_cmd::events_te::sign_hex >();
      FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Secmod command is not implemented, etype = %{ETYPE}", ("ETYPE", e_type));
      //TODO: need to implement
    }
    FC_LIGHT_CAPTURE_AND_RETHROW()
  }
};

template <>
struct EventHandler < sm_cmd::events_te::sign_hash> : EventHandlerBase
{
  EventHandler() :EventHandlerBase(static_cast<sm_cmd::events_te>(sm_cmd::events_te::sign_hash)) {}
  virtual ~EventHandler() {}
  virtual void operator()(const sm_cmd::secmod_parser_f& parser) const override
  {
    try
    {
      auto cmd = parser.params< sm_cmd::events_te::sign_hash >();
      FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Secmod command is not implemented, etype = %{ETYPE}", ("ETYPE", e_type));
      //TODO: need to implement
    }
    FC_LIGHT_CAPTURE_AND_RETHROW()
  }
};

template <>
struct EventHandler < sm_cmd::events_te::remove_key> : EventHandlerBase
{
  EventHandler() :EventHandlerBase(static_cast<sm_cmd::events_te>(sm_cmd::events_te::remove_key)) {}
  virtual ~EventHandler() {}
  virtual void operator()(const sm_cmd::secmod_parser_f& parser) const override
  {
    FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Secmod command is not implemented, etype = %{ETYPE}", ("ETYPE", e_type));
    //TODO: need to implement
  }
};

template <>
struct EventHandler < sm_cmd::events_te::unlock> : EventHandlerBase
{
  EventHandler() :EventHandlerBase(static_cast<sm_cmd::events_te>(sm_cmd::events_te::unlock)) {}
  virtual ~EventHandler() {}
  virtual void operator()(const sm_cmd::secmod_parser_f& parser) const override
  {
    auto cmd = parser.params< sm_cmd::events_te::unlock >();
    FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Secmod command is not implemented, etype = %{ETYPE}", ("ETYPE", e_type));
    //TODO: need to implement
  }
};

template <>
struct EventHandler < sm_cmd::events_te::edit_key> : EventHandlerBase
{
  EventHandler() :EventHandlerBase(static_cast<sm_cmd::events_te>(sm_cmd::events_te::unlock)) {}
  virtual ~EventHandler() {}
  virtual void operator()(const sm_cmd::secmod_parser_f& parser) const override
  {
    auto cmd = parser.params< sm_cmd::events_te::unlock >();
    FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Secmod command is not implemented, etype = %{ETYPE}", ("ETYPE", e_type));
    //TODO: need to implement
  }
};

template <>
struct EventHandler < sm_cmd::events_te::print_mnemonic> : EventHandlerBase
{
  EventHandler() :EventHandlerBase(static_cast<sm_cmd::events_te>(sm_cmd::events_te::remove_key)) {}
  virtual ~EventHandler() {}
  virtual void operator()(const sm_cmd::secmod_parser_f& parser) const override
  {
    FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Secmod command is not implemented, etype = %{ETYPE}", ("ETYPE", e_type));
    //TODO: need to implement in future
  }
};

template <>
struct EventHandler < sm_cmd::events_te::export_keys> : EventHandlerBase
{
  EventHandler() :EventHandlerBase(static_cast<sm_cmd::events_te>(sm_cmd::events_te::remove_key)) {}
  virtual ~EventHandler() {}
  virtual void operator()(const sm_cmd::secmod_parser_f& parser) const override
  {
    FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Secmod command is not implemented, etype = %{ETYPE}", ("ETYPE", e_type));
    //TODO: need to implement
  }
};

template <>
struct EventHandler < sm_cmd::events_te::import_keys> : EventHandlerBase
{
  EventHandler() :EventHandlerBase(static_cast<sm_cmd::events_te>(sm_cmd::events_te::remove_key)) {}
  virtual ~EventHandler() {}
  virtual void operator()(const sm_cmd::secmod_parser_f& parser) const override
  {
    FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Secmod command is not implemented, etype = %{ETYPE}", ("ETYPE", e_type));
    //TODO: need to implement
  }
};

constexpr auto event_handlers_static_list = boost::hana::make_range(
  boost::hana::int_c<static_cast<int>(sm_cmd::events_te::unknown)>,
  boost::hana::int_c<static_cast<int>(sm_cmd::events_te::last)>);
