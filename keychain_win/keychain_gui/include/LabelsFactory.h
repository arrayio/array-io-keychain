#pragma once
#include <QWidget>
#include <QFrame>
#include <QLabel>

#include <fc_light/exception/exception.hpp>

enum struct Labels_te
{
  UNSPECIFIED = 0,
  HEADER,
  LOGO,
  DESCRIPTION,
  LAST
};

template <Labels_te label>
struct CreateLabel
{
  QLabel* operator()(QWidget* parent)
  {
    FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Label is undefined, label = ${LABEL}", ("LABEL", label));
  }
};

template <>
struct CreateLabel<Labels_te::HEADER>
{
  QLabel* operator()(QWidget* parent)
  {
    auto* headerBlock = new  QLabel(parent);
    headerBlock->setFixedHeight(68);
    headerBlock->setStyleSheet("background-color:rgb(255,255,255);");
    return headerBlock;
  }
};

template <>
struct CreateLabel<Labels_te::DESCRIPTION>
{
  QLabel* operator()(QWidget* parent)
  {
    auto* descriptionLabel = new QLabel(this);
    descriptionLabel->setStyleSheet("font:16px \"Segoe UI\";background:transparent;");
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    descriptionLabel->setFixedSize(343, 68);
    return descriptionLabel;
  }
};

template <>
struct CreateLabel<Labels_te::LOGO>
{
  QLabel* operator()(QWidget* parent)
  {
    auto* logoLabel = new QLabel(this);
    QPixmap logo(":/keychain_gui_win/kch_logo.png");
    logo = logo.scaled(100, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    logoLabel->setStyleSheet("background:transparent;");
    logoLabel->setPixmap(logo);
    logoLabel->move(25, 8);
    return logoLabel;
  }
};

