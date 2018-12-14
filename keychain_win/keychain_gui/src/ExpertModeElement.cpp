#include "ExpertModeElement.h"

ExpertModeElement::ExpertModeElement(QWidget * parent)
	: QWidget(parent)
{
	QMetaObject::connectSlotsByName(this);
	expertLabel = new QLabel(this);
	expertLabel->setStyleSheet("font:16px \"Segoe UI\";background:transparent;");
	expertLabel->setText("Expert mode");

	//expertLabel->setFixedSize(116, 20);
	expertLabel->setFrameStyle(QFrame::NoFrame);
	expertLabel->setAlignment(Qt::AlignBottom | Qt::AlignRight);

	expertValue = new ExpertModeLabel(this);
	expertValue->setStyleSheet("background-color:rgb(227,232,248);border-style:outset;border-width:0px;border-radius:7px;font:14px \"Segoe UI\";color:rgb(70,134,255);padding:5px;");
	//expertModeLabelValue->setStyleSheet("font:10pt \"Segoe UI\";background-image:url(:/keychain_gui_win/expert.png);color:rgb(70,134,255);padding:5px;");
	expertValue->setWordWrap(true);
	//expertValue->setFixedSize(400, 50);
	expertValue->setFrameStyle(QFrame::NoFrame);
	expertValue->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	expertValue->setCursor(Qt::PointingHandCursor);
	connect(expertValue, &ExpertModeLabel::clicked, this, &ExpertModeElement::showMoreForExpert);

	/*moreButton = new QPushButton(this);
	moreButton->setFixedSize(13, 7);
	moreButton->setFlat(true);
	moreButton->setStyleSheet("background:transparent;background-image:url(:/keychain_gui_win/more_arrow.png);border-style:outset;border-width:0px;");
	moreButton->setWindowFlags(Qt::FramelessWindowHint);
	moreButton->setCursor(Qt::PointingHandCursor);*/
	//connect(moreButton, &QPushButton::clicked, this, &ExpertModeElement::showMoreForExpert);
}

ExpertModeElement::~ExpertModeElement()
{

}

void ExpertModeElement::SetPosition(int x, int y, int labelWidth, int valueWidth)
{
	expertLabel->move(0, 0);
	expertLabel->setFixedWidth(labelWidth);
	expertValue->setFixedWidth(valueWidth);
	expertLabel->setFixedHeight(25);
	expertValue->setFixedHeight(50);
	setFixedWidth(expertLabel->width() + expertValue->width() + 16);
	expertValue->move(labelWidth + 16, 0);
	//moreButton->move((expertValue->x() + expertValue->width() - 20), (expertValue->y() + 50 - 12));
	/*_valueX = x + labelWidth + 16, y;
	_labelX = x;*/
}

void ExpertModeElement::SetExpertModeText(QString expertMode)
{
	expertModeText = expertMode;
	QString transText = expertMode;
	transText = transText.insert(40, '\n');
	transText = transText.mid(0, 80);
	transText = transText.append("...");
	expertValue->setText(transText);
}

void ExpertModeElement::showMoreForExpert()
{
	ExpertModeDialog dialog;
	dialog.SetExpertModeText(expertModeText);
	dialog.exec();
}