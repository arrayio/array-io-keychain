#include "PopupWindow.h"



PopupWindow::PopupWindow(QWidget *parent)
	: QFrame(parent)
{
	this->setFixedWidth(171);
	this->setFixedHeight(113);
	header = new QLabel(this);
	body = new QLabel(this);
	more = new QLabel(this);
	this->setStyleSheet("background-image:url(:/keychain_gui_win/notify_back.png)");
	header->setText("Transaction is secure");
	header->setStyleSheet("background:transparent;width:150px;height:12px;text-align:center;color:rgb(81,172,74)");
	header->move(0, 10);
	body->move(0, 16);
	body->setStyleSheet("background:transparent;width:150px;height:64px;text-align:justify;color:rgb(147,148,151)");
	body->setText("This transaction is verified.<br>Your private keys are kept in<br>an isolated environment to<br>ensure a strong layer of<br>protection.");
}


PopupWindow::~PopupWindow()
{
}
