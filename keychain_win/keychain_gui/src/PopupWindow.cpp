#include "PopupWindow.h"



PopupWindow::PopupWindow(QWidget *parent)
	: QFrame(parent)
{
	this->setFixedWidth(172);
	this->setFixedHeight(115);
	header = new QLabel(this);
	body = new QLabel(this);
	more = new QLabel(this);
	this->setStyleSheet("background-image:url(:/keychain_gui_win/notify_back.png)");
	header->setFixedWidth(140);
	header->setText("Transaction is secure");
	header->setStyleSheet("background:transparent;color:rgb(81,172,74);font:15px \"Segoe UI\";");
	header->move(15, 2);
	header->setAlignment(Qt::AlignCenter);
	body->move(0, 16);
	body->setFixedWidth(171);
	body->setWordWrap(true);
	body->setStyleSheet("background:transparent;width:150px;height:64px;color:rgb(147,148,151);font:13px \"Segoe UI\";padding:5px;");
	body->setText("This transaction is verified. Your private keys are kept in an isolated environment to ensure a strong layer of protection.");
	body->setAlignment(Qt::AlignJustify);
}


void PopupWindow::setUnsecureText()
{
	header->setText("Unsecure transaction");
	header->setStyleSheet("background:transparent;color:rgb(215,132,133);font:15px \"Segoe UI\";");
	body->setFixedWidth(171);
	body->setStyleSheet("background:transparent;width:150px;height:64px;color:rgb(147,148,151);font:13px \"Segoe UI\";padding:5px;");
	body->setWordWrap(true);
	body->setText("We do not recommend you to sign this transaction because it might come from a fraudulent source.");
	body->setAlignment(Qt::AlignJustify);
}


PopupWindow::~PopupWindow()
{
}
