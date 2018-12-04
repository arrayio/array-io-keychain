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
	header->setFixedWidth(150);
	header->setText("Transaction is secure");
	header->setStyleSheet("background:transparent;text-align:center;color:rgb(81,172,74);font:11pt \"Segoe UI\";");
	header->move(15, 2);
	body->move(0, 16);
	body->setStyleSheet("background:transparent;width:150px;height:64px;text-align:justify;color:rgb(147,148,151);font:9.5pt \"Segoe UI\";padding:5px;");
	body->setText("This transaction is verified.<br>Your private keys are kept<br>in an isolated environment<br>to ensure a strong layer<br>of protection.");
}


PopupWindow::~PopupWindow()
{
}
