#include "ExpertModeDialog.h"

ExpertModeDialog::ExpertModeDialog(QWidget * parent)
	: QDialog(parent)
{
	QMetaObject::connectSlotsByName(this);
	setFixedSize(450, 400);
	setWindowFlags(Qt::FramelessWindowHint);
	setStyleSheet("background-color:rgb(242,243,246);border-style:solid;border-width:1px;border-radius:5px;border-color:rgb(225,224,224);");
	transactionDescription = new QPlainTextEdit(this);
	transactionDescription->setReadOnly(true);
	transactionDescription->setStyleSheet("color:rgb(70,134,255);background:transparent;border-style:outset;border-width:0px;border-radius:5px;font:15px \"Segoe UI\"");
	transactionDescription->setFixedSize(400, 340);
	transactionDescription->move(25,5);
	OKButton = new QPushButton(this);
	OKButton->setFixedSize(100,25);
	OKButton->move(175, 350);
	OKButton->setCursor(Qt::PointingHandCursor);
	OKButton->setText("OK");
	OKButton->setStyleSheet("color:white;background-color:rgb(70,134,255);border-style:outset;border-width:0px;border-radius:5px;font:16px \"Segoe UI\"");
	_roundCorners();
	connect(OKButton, &QPushButton::pressed, this, &ExpertModeDialog::closeExpertMode);
}


ExpertModeDialog::~ExpertModeDialog()
{

}

void ExpertModeDialog::keyPressEvent(QKeyEvent *event)
{
	int m = event->key();
	if(event->key() == Qt::Key_Escape || event->key() == Qt::Key_Return) {
		close();
	}
}

void ExpertModeDialog::SetExpertModeText(QString string)
{
	transactionDescription->setPlainText(string);
}

void ExpertModeDialog::closeExpertMode()
{
	close();
}

void ExpertModeDialog::_roundCorners()
{
	int radius = 10;
	QRegion region(0, 0, width(), height(), QRegion::Rectangle);

	// top left
	QRegion round(0, 0, 2 * radius, 2 * radius, QRegion::Ellipse);
	QRegion corner(0, 0, radius, radius, QRegion::Rectangle);
	region = region.subtracted(corner.subtracted(round));

	// top right
	round = QRegion(width() - 2 * radius, 0, 2 * radius, 2 * radius, QRegion::Ellipse);
	corner = QRegion(width() - radius, 0, radius, radius, QRegion::Rectangle);
	region = region.subtracted(corner.subtracted(round));

	// bottom right
	round = QRegion(width() - 2 * radius, height() - 2 * radius, 2 * radius, 2 * radius, QRegion::Ellipse);
	corner = QRegion(width() - radius, height() - radius, radius, radius, QRegion::Rectangle);
	region = region.subtracted(corner.subtracted(round));

	// bottom left
	round = QRegion(0, height() - 2 * radius, 2 * radius, 2 * radius, QRegion::Ellipse);
	corner = QRegion(0, height() - radius, radius, radius, QRegion::Rectangle);
	region = region.subtracted(corner.subtracted(round));

	setMask(region);
}
