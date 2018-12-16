#include "PopupWindow.h"



PopupWindow::PopupWindow(const KeychainWarningMessage wMessage, QWidget *parent)
	: QFrame(parent)
{
	this->setFixedWidth(350);
	header = new QLabel(this);
	body = new QLabel(this);
	setStyleSheet("background-color:white;border-color:rgb(158,158,158);border-style:outset;border-width:1px;border-radius:5px;");
	header->setFixedWidth(140);
	header->move(105, 2);
	header->setAlignment(Qt::AlignCenter);
	
	body->move(0, 16);

	body->setFixedWidth(350);
	body->setWordWrap(true);
	body->setStyleSheet("background:transparent;border-width:0;color:rgb(147,148,151);font:14px \"Segoe UI\";padding:5px;");
	
	
	QFont myFont= body->font();
	QString messageStr = wMessage.GetMessage();

	QFontMetrics fm(myFont);
	int k = myFont.pixelSize();
	int strWidth = fm.width(messageStr);
	int strHeight = fm.height() + fm.lineSpacing() + fm.lineWidth();
	int numLines = (strWidth / 350)+1;

	
	body->setText(messageStr);
	this->setFixedHeight((strHeight * numLines)+20);
	body->setFixedHeight(strHeight*numLines+5);
	
	body->setAlignment(Qt::AlignJustify);
	if (wMessage.isWarn()==false) {
		header->setText("Transaction is secure");
		header->setStyleSheet("background:transparent;border-width:0;color:rgb(81,172,74);font:15px \"Segoe UI\";");
	}
	else
	{
		header->setText("Unsecure transaction");
		header->setStyleSheet("background:transparent;border-width:0;color:rgb(215,132,133);font:15px \"Segoe UI\";");
	}
}

PopupWindow::~PopupWindow()
{
}
