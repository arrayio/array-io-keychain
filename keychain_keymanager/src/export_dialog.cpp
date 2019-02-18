#include "export_dialog.hpp"


export_dialog::export_dialog(QDialog* parent)
	:QDialog(parent)
{
	QMetaObject::connectSlotsByName(this);
	//setAttribute(Qt::WA_TranslucentBackground); //enable MainWindow to be transparent
	setWindowTitle("Choose export file");
	setStyleSheet("background-color:rgb(255, 255, 255)");
	setWindowFlags(Qt::FramelessWindowHint);
	setFixedSize(319, 160);
	int wParent = parent->width();
	int hParent = parent->height();
	int _x = wParent/2 - (319 / 2);
	int _y = hParent/2 - (160 / 2);
	move(_x, _y);
	rounded_corners();

	//create window hints toolbar 
	hd_toolbar = new hints_toolbar(this);
	hd_toolbar->setStyleSheet("font:\"Segoe UI\";background:transparent;");
	//set absolute position
	//because width delta between frame geometry width and width is equal zero
	hd_toolbar->move(0,0);

	//export button init
	but_export = new QPushButton(this);
	but_export->setFixedSize(151, 31);
	but_export->move(85,109);
	but_export->setText("Create a password");
	but_export->setStyleSheet("font:16px \"Segoe UI\";color:white;background-color:rgb(165,178,200);border-style:solid;border-width:0px;border-radius:4px;");

	//enter for folder path  250 251 252
	folder_path = new QLineEdit(this);
	folder_path->move(17, 56);
	folder_path->setFixedSize(290,28);
	folder_path->setStyleSheet("font:16px \"Segoe UI\";color:rgb(183,183,184);background-color:rgb(250,251,252);border-style:solid;border-width:0px;border-radius:4px;");

	//open folder button
	but_sel_folder = new QPushButton(this);
	but_sel_folder->setFixedSize(28, 28);
	but_sel_folder->move(279, 56);
	QPixmap openFolderIcon(":/keymanager/open_folder.png");
	openFolderIcon = openFolderIcon.scaled(25, 25, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	but_sel_folder->setIcon(openFolderIcon);
	but_sel_folder->setStyleSheet("font:16px \"Segoe UI\";background-color:transparent;border-style:solid;border-width:0px;border-radius:4px;");
	
	
	//close_but.png
}

void export_dialog::rounded_corners()
{
	int radius = 10;
	QRegion region(0, 0, width(), height(), QRegion::Rectangle);

	QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
	effect->setOffset(0, 0);
	effect->setBlurRadius(5);
	this->setGraphicsEffect(effect);

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
