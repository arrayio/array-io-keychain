#include "PopupWindow.h"



PopupWindow::PopupWindow(const KeychainWarningMessage wMessage, QWidget *parent)
	: QFrame(parent)
{
	this->setFixedWidth(350);
	header = new QLabel(this);
	body = new QLabel(this);
	setStyleSheet("background-color:white;border-color:rgb(158,158,158);border-style:outset;border-width:1px;border-radius:5px;");
	header->setFixedWidth(150);
	header->move(105, 2);
	header->setAlignment(Qt::AlignCenter);
	
	body->move(0, 10);

	body->setFixedWidth(350);
	body->setWordWrap(true);
	body->setStyleSheet("background:transparent;border-width:0;color:rgb(147,148,151);font:14px \"Segoe UI\";padding:10px;");
	
	
	QFont myFont= body->font();
	int mCount = wMessage.MessgeCount();
	QString messageStr = wMessage.GetMessage();
	myFont.setPixelSize(14);
	QFontMetrics fm(myFont);
	int k = myFont.pixelSize();

	int strWidth = fm.width(messageStr);
	int strHeight = k+k/2;
	int numLines = std::ceil(strWidth / 350.0)+((mCount>1)?((mCount-1)*2):0);

	
	body->setText(messageStr);
	body->setFixedHeight(strHeight*numLines+20);
	this->setFixedHeight(body->height()+10);
	
	body->setAlignment(Qt::AlignJustify);
	if (wMessage.isWarn()==false) {
		header->setText("Transaction is secure");
		header->setStyleSheet("background:transparent;border-width:0;color:rgb(81,172,74);font:15px \"Segoe UI\";");
	}
	else
	{
		header->setText("<b>Unsecure transaction</b>");
		header->setStyleSheet("background:transparent;border-width:0;color:rgb(215,132,133);font:15px \"Segoe UI\";");
	}
}

PopupWindow::~PopupWindow()
{
}
