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
  virtual void operator()(QApplication* app, const sm_cmd::secmod_parser_f& parser) const = 0;
};

template <sm_cmd::events_te etype>
struct EventHandler : EventHandlerBase
{
  EventHandler() :EventHandlerBase(static_cast<sm_cmd::events_te>(etype)) {}
  virtual ~EventHandler() {}
  virtual void operator()(QApplication* app, const sm_cmd::secmod_parser_f& parser) const override
  {
    FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Secmod command is not found, etype = %{ETYPE}", ("ETYPE", e_type));
  }
};

template <>
struct EventHandler< sm_cmd::events_te::create_key> : EventHandlerBase
{
  EventHandler() :EventHandlerBase(static_cast<sm_cmd::events_te>(sm_cmd::events_te::create_key)) {}
  virtual ~EventHandler() {}
  virtual void operator()(QApplication* app, const sm_cmd::secmod_parser_f& parser) const override
  {
    try
    {
      auto cmd = parser.params< sm_cmd::events_te::create_key >();
      auto gui_dlg = new keychain_gui_win(app);
      gui_dlg->mWarningMessage.SetWarning(KeychainWarningMessage::WarningType::CreateWarning);
      //TODO: need to use bold font for keyname string
      gui_dlg->descriptionLabel->setText("Enter the password for the new key \"" + QString::fromStdString(cmd.keyname) + "\"");
      gui_dlg->password = new PasswordEnterElement(true, gui_dlg);//TODO: it is more preferable to use template constant parameters
     
      //TODO: need to wrap this code
      gui_dlg->password->SetLabel("Passphrase");
      gui_dlg->password->SetPosition(0, gui_dlg->mEndControlPosition, keychain_gui_win::FIELD_WIDTH);
      gui_dlg->password->move(0, gui_dlg->mEndControlPosition);
      gui_dlg->mEndControlPosition += gui_dlg->password->GetElementHeigth();
      gui_dlg->mEndControlPosition += 10;
      
      gui_dlg->OKButton = new QPushButton("CREATE", this);

      //TODO: need to wrap this code
      gui_dlg->OKButton->setFixedSize(89, 25);
      gui_dlg->OKButton->setFlat(true);
      gui_dlg->OKButton->setCursor(Qt::PointingHandCursor);
      gui_dlg->OKButton->setStyleSheet("color:white;background-color:rgb(70,134,255);border-style:outset;border-width:0px;border-radius:5px;font:16px \"Segoe UI\"");
      gui_dlg->OKButton->setWindowFlags(Qt::FramelessWindowHint);
      gui_dlg->setFixedHeight(gui_dlg->mEndControlPosition + gui_dlg->OKButton->height() + 15);
      gui_dlg->OKButton->move(gui_dlg->width() - 109, gui_dlg->mEndControlPosition);

      QObject::connect(gui_dlg->password, &PasswordEnterElement::changePassword, gui_dlg, &keychain_gui_win::_disableSignButton);
      gui_dlg->show();
    }
    FC_LIGHT_CAPTURE_AND_RETHROW()
  }
};

template <>
struct EventHandler < sm_cmd::events_te::sign_hex > : EventHandlerBase
{
  EventHandler() :EventHandlerBase(static_cast<sm_cmd::events_te>(sm_cmd::events_te::sign_hex)) {}
  virtual ~EventHandler() {}
  virtual void operator()(QApplication* app, const sm_cmd::secmod_parser_f& parser) const override
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
      gui_dlg->password = new PasswordEnterElement(false, gui_dlg);//TODO: it is more preferable to use template constant parameters
      //TODO: need to wrap this code
      gui_dlg->password->SetLabel("Passphrase");
      gui_dlg->password->SetPosition(0, gui_dlg->mEndControlPosition, keychain_gui_win::FIELD_WIDTH);
      gui_dlg->password->move(0, gui_dlg->mEndControlPosition);
      gui_dlg->mEndControlPosition += gui_dlg->password->GetElementHeigth();
      gui_dlg->mEndControlPosition += 10;

      gui_dlg->OKButton = new QPushButton("SIGN", this);

      //TODO: need to wrap this code
      gui_dlg->OKButton->setFixedSize(89, 25);
      gui_dlg->OKButton->setFlat(true);
      gui_dlg->OKButton->setCursor(Qt::PointingHandCursor);
      gui_dlg->OKButton->setStyleSheet("color:white;background-color:rgb(70,134,255);border-style:outset;border-width:0px;border-radius:5px;font:16px \"Segoe UI\"");
      gui_dlg->OKButton->setWindowFlags(Qt::FramelessWindowHint);
      gui_dlg->setFixedHeight(gui_dlg->mEndControlPosition + gui_dlg->OKButton->height() + 15);
      gui_dlg->OKButton->move(gui_dlg->width() - 109, gui_dlg->mEndControlPosition);

      gui_dlg->element->move(0, gui_dlg->mEndControlPosition);
      gui_dlg->element->SetPosition(0, gui_dlg->mEndControlPosition, keychain_gui_win::FIELD_WIDTH);
      gui_dlg->mEndControlPosition += 10;
      gui_dlg->mEndControlPosition = gui_dlg->mEndControlPosition + gui_dlg->element->GetCurrentHeight();
      gui_dlg->show();
    }
    FC_LIGHT_CAPTURE_AND_RETHROW()
  }
};

template <>
struct EventHandler < sm_cmd::events_te::sign_hash> : EventHandlerBase
{
  EventHandler() :EventHandlerBase(static_cast<sm_cmd::events_te>(sm_cmd::events_te::sign_hash)) {}
  virtual ~EventHandler() {}
  virtual void operator()(QApplication* app, const sm_cmd::secmod_parser_f& parser) const override
  {
    try
    {
      auto cmd = parser.params< sm_cmd::events_te::sign_hash >();
      auto gui_dlg = new keychain_gui_win(app);

      //TODO: code from sign_hex
      QString descr("Are you sure you want to sign this transaction with key <b>''" + QString::fromStdString(cmd_parse.keyname()) + "''</b>?");
      gui_dlg->descriptionLabel->setText(descr);

      
      gui_dlg->mWarningMessage.SetWarning(KeychainWarningMessage::WarningType::HashWarnig);
      gui_dlg->element = new RawHashWidget(cmd, this);

      //TODO: code from sign_hex
      gui_dlg->password = new PasswordEnterElement(false, gui_dlg);//TODO: it is more preferable to use template constant parameters
      //TODO: need to wrap this code
      gui_dlg->password->SetLabel("Passphrase");
      gui_dlg->password->SetPosition(0, gui_dlg->mEndControlPosition, keychain_gui_win::FIELD_WIDTH);
      gui_dlg->password->move(0, gui_dlg->mEndControlPosition);
      gui_dlg->mEndControlPosition += gui_dlg->password->GetElementHeigth();
      gui_dlg->mEndControlPosition += 10;

      gui_dlg->OKButton = new QPushButton("SIGN", this);
      //TODO: need to wrap this code
      gui_dlg->OKButton->setFixedSize(89, 25);
      gui_dlg->OKButton->setFlat(true);
      gui_dlg->OKButton->setCursor(Qt::PointingHandCursor);
      gui_dlg->OKButton->setStyleSheet("color:white;background-color:rgb(70,134,255);border-style:outset;border-width:0px;border-radius:5px;font:16px \"Segoe UI\"");
      gui_dlg->OKButton->setWindowFlags(Qt::FramelessWindowHint);
      gui_dlg->setFixedHeight(gui_dlg->mEndControlPosition + gui_dlg->OKButton->height() + 15);
      gui_dlg->OKButton->move(gui_dlg->width() - 109, gui_dlg->mEndControlPosition);

      gui_dlg->element->move(0, gui_dlg->mEndControlPosition);
      gui_dlg->element->SetPosition(0, gui_dlg->mEndControlPosition, keychain_gui_win::FIELD_WIDTH);
      gui_dlg->mEndControlPosition += 10;
      gui_dlg->mEndControlPosition = gui_dlg->mEndControlPosition + gui_dlg->element->GetCurrentHeight();
      gui_dlg->show();
    }
    FC_LIGHT_CAPTURE_AND_RETHROW()
  }
};

template <>
struct EventHandler < sm_cmd::events_te::remove_key> : EventHandlerBase
{
  EventHandler() :EventHandlerBase(static_cast<sm_cmd::events_te>(sm_cmd::events_te::remove_key)) {}
  virtual ~EventHandler() {}
  virtual void operator()(QApplication* app, const sm_cmd::secmod_parser_f& parser) const override
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
  virtual void operator()(QApplication* app, const sm_cmd::secmod_parser_f& parser) const override
  {
    auto cmd = parser.params< sm_cmd::events_te::unlock >();
    auto gui_dlg = new keychain_gui_win(app);
    gui_dlg->password = new PasswordEnterElement(false, gui_dlg);//TODO: it is more preferable to use template constant parameters
    
    //TODO: need to wrap this code
    gui_dlg->password->SetLabel("Passphrase");
    gui_dlg->password->SetPosition(0, gui_dlg->mEndControlPosition, keychain_gui_win::FIELD_WIDTH);
    gui_dlg->password->move(0, gui_dlg->mEndControlPosition);
    gui_dlg->mEndControlPosition += gui_dlg->password->GetElementHeigth();
    gui_dlg->mEndControlPosition += 10;

    gui_dlg->mWarningMessage.SetWarning(KeychainWarningMessage::WarningType::UnlockWarning);
    gui_dlg->descriptionLabel->setText("You are trying to unlock the key \"" + transaction.unlockKeyName() + "\" for \"" + QString::number(transaction.isUnlockKey()) + "\" seconds");
    gui_dlg->OKButton = new QPushButton("UNLOCK", this);
    
    //TODO: need to wrap this code
    gui_dlg->OKButton->setFixedSize(89, 25);
    gui_dlg->OKButton->setFlat(true);
    gui_dlg->OKButton->setCursor(Qt::PointingHandCursor);
    gui_dlg->OKButton->setStyleSheet("color:white;background-color:rgb(70,134,255);border-style:outset;border-width:0px;border-radius:5px;font:16px \"Segoe UI\"");
    gui_dlg->OKButton->setWindowFlags(Qt::FramelessWindowHint);
    gui_dlg->setFixedHeight(gui_dlg->mEndControlPosition + gui_dlg->OKButton->height() + 15);
    gui_dlg->OKButton->move(gui_dlg->width() - 109, gui_dlg->mEndControlPosition);

    gui_dlg->show();
  }
};

template <>
struct EventHandler < sm_cmd::events_te::print_mnemonic> : EventHandlerBase
{
  EventHandler() :EventHandlerBase(static_cast<sm_cmd::events_te>(sm_cmd::events_te::remove_key)) {}
  virtual ~EventHandler() {}
  virtual void operator()(QApplication* app, const sm_cmd::secmod_parser_f& parser) const override
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
  virtual void operator()(QApplication* app, const sm_cmd::secmod_parser_f& parser) const override
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
  virtual void operator()(QApplication* app, const sm_cmd::secmod_parser_f& parser) const override
  {
    FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Secmod command is not implemented, etype = %{ETYPE}", ("ETYPE", e_type));
    //TODO: need to implement
  }
};