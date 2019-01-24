#pragma once

#include <fc_light/exception/exception.hpp>
#include <keychain_lib/secmod_parser_cmd.hpp>
#include "keychain_gui_win.h"

namespace sm_cmd = keychain_app::secmod_commands;

struct EventHandlerBase
{
  EventHandlerBase(sm_cmd::events_te type): e_type(type){}
  virtual ~EventHandlerBase(){}
  sm_cmd::events_te e_type;
  virtual std::string operator()(QApplication* app, const sm_cmd::secmod_parser_f& parser) const = 0;
};

template <sm_cmd::events_te etype>
struct EventHandler : EventHandlerBase
{
  EventHandler() :EventHandlerBase(static_cast<sm_cmd::events_te>(etype)) {}
  virtual ~EventHandler() {}
  virtual std::string operator()(QApplication* app, const sm_cmd::secmod_parser_f& parser) const override
  {
    FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Secmod command is not found, etype = %{ETYPE}", ("ETYPE", e_type));
  }
};

template <>
struct EventHandler< sm_cmd::events_te::create_key> : EventHandlerBase
{
  EventHandler() :EventHandlerBase(static_cast<sm_cmd::events_te>(sm_cmd::events_te::create_key)) {}
  virtual ~EventHandler() {}
  virtual std::string operator()(QApplication* app, const sm_cmd::secmod_parser_f& parser) const override
  {
    try
    {
      auto cmd = parser.params< sm_cmd::events_te::create_key >();
      auto gui_dlg = new keychain_gui_win(app);
      gui_dlg->mWarningMessage.SetWarning(KeychainWarningMessage::WarningType::CreateWarning);
      //TODO: need to use bold font for keyname string
      gui_dlg->descriptionLabel->setText("Enter the password for the new key \"" + QString::fromStdString(cmd.keyname) + "\"");
      gui_dlg->password = new PasswordEnterElement(true, gui_dlg);//TODO: it is more preferable to use template constant parameters
      QObject::connect(gui_dlg->password, &PasswordEnterElement::changePassword, gui_dlg, &keychain_gui_win::_disableSignButton);
    }
    FC_LIGHT_CAPTURE_AND_RETHROW()
  }
};

template <>
struct EventHandler < sm_cmd::events_te::sign_hex > : EventHandlerBase
{
  EventHandler() :EventHandlerBase(static_cast<sm_cmd::events_te>(sm_cmd::events_te::sign_hex)) {}
  virtual ~EventHandler() {}
  virtual std::string operator()(QApplication* app, const sm_cmd::secmod_parser_f& parser) const override
  {
    try
    {
      auto cmd = parser.params< sm_cmd::events_te::sign_hex >();
      auto gui_dlg = new keychain_gui_win(app);
      if(cmd.unlock_time > 0)
        gui_dlg->mWarningMessage.SetWarning(KeychainWarningMessage::WarningType::UnlockWarning);

      QString descr("Are you sure you want to sign this transaction with key <b>''" + QString::fromStdString(cmd.keyname) + "''</b>?");
      gui_dlg->descriptionLabel->setText(descr);
      if (!cmd.is_parsed) 
      {
        gui_dlg->element = new UnparsedTransactionWidget(to_expert_mode_string(cmd), cmd.unlock_time, this);
        gui_dlg->mWarningMessage.SetWarning(KeychainWarningMessage::WarningType::FailedWarning);
      }
      else
      {
        switch (cmd.blockchain)
        {
        case keychain_app::secmod_commands::blockchain_secmod_te::ethereum: {
          gui_dlg->element = new EthereumWidget(cmd.get_trx_view<sm_cmd::blockchain_secmod_te::ethereum>(), this);
          gui_dlg->mWarningMessage.SetWarning(KeychainWarningMessage::WarningType::NoWarnig);
          break;
        }
        case keychain_app::secmod_commands::blockchain_secmod_te::ethereum_swap: {
          gui_dlg->element = new EthereumSwapWidget(cmd.get_trx_view<sm_cmd::blockchain_secmod_te::ethereum_swap>(), this);
          gui_dlg->mWarningMessage.SetWarning(KeychainWarningMessage::WarningType::NoWarnig);
          gui_dlg->languageLabel = new QLabel(this);
          QObject::connect(QGuiApplication::inputMethod(), &QInputMethod::localeChanged, gui_dlg, &keychain_gui_win::changeLocale);
          break;
        }
        case keychain_app::secmod_commands::blockchain_secmod_te::bitcoin:
        {
          gui_dlg->element = new BitcoinWidget(cmd.get_trx_view<sm_cmd::blockchain_secmod_te::bitcoin>(), this);
          gui_dlg->mWarningMessage.SetWarning(KeychainWarningMessage::WarningType::NoWarnig);
          break;
        }
        case keychain_app::secmod_commands::blockchain_secmod_te::unknown:
        {
          gui_dlg->mWarningMessage.SetWarning(KeychainWarningMessage::WarningType::FailedWarning);
          gui_dlg->element = new UnparsedTransactionWidget(to_expert_mode_string(cmd), cmd.unlock_time, this);
          break;
        }
        break;
        }
      }
      gui_dlg->element->move(0, gui_dlg->mEndControlPosition);
      gui_dlg->element->SetPosition(0, gui_dlg->mEndControlPosition, keychain_gui_win::FIELD_WIDTH);
      gui_dlg->mEndControlPosition += 10;
      gui_dlg->mEndControlPosition = gui_dlg->mEndControlPosition + gui_dlg->element->GetCurrentHeight();
    }
    FC_LIGHT_CAPTURE_AND_RETHROW()
  }
};