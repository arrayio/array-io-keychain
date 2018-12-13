#include "PopupWindow.h"



PopupWindow::PopupWindow(const KeychainWarningMessage wMessage, QWidget *parent)
	: QFrame(parent)
{
	this->setFixedWidth(250);
	this->setFixedHeight(215);
	header = new QLabel(this);
	body = new QLabel(this);
	more = new QLabel(this);
	setStyleSheet("background-color:white;border-color:grey;border-style:outset;border-width:0px;border-radius:5px;");
	header->setFixedWidth(140);
	header->move(15, 2);
	header->setAlignment(Qt::AlignCenter);
	
	body->move(0, 16);

	body->setFixedWidth(171);
	body->setWordWrap(true);
	body->setStyleSheet("background:transparent;width:150px;height:64px;color:rgb(147,148,151);font:13px \"Segoe UI\";padding:5px;");
	body->setText(wMessage.GetMessage());
	
	body->setAlignment(Qt::AlignJustify);
	if (wMessage.isWarn()==false) {
		header->setText("Transaction is secure");
		header->setStyleSheet("background:transparent;color:rgb(81,172,74);font:15px \"Segoe UI\";");
	}
	else
	{
		header->setText("Unsecure transaction");
			header->setStyleSheet("background:transparent;color:rgb(215,132,133);font:15px \"Segoe UI\";");
	}
}

PopupWindow::~PopupWindow()
{
}
