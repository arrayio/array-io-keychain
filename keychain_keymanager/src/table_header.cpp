#include "table_header.hpp"

table_header::table_header(QWidget *parent)
	:QWidget(parent)
{
	//QMetaObject::connectSlotsByName(this);
	setStyleSheet("background-image:url(':/keymanager/tbl_title_back.png');");
	setFixedSize(1033, 40);
	QString text_style("color:rgb(255,255,255);");
	QFont text_font("Segoe UI");
	text_font.setPixelSize(18);
	text_font.setWeight(80);
	//keyname frame
	key_name_frame = new QFrame(this);
	key_name_frame->setFixedSize(314, 40);
	key_name_label = new QLabel("Keyname", key_name_frame);
	key_name_label->move(15, 0);
	key_name_label->setFixedSize(305,40);
	key_name_label->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	key_name_label->setFont(text_font);
	key_name_label->setStyleSheet(text_style);

	//public key header field
	pub_key_frame = new QFrame(this);
	pub_key_frame->setFixedSize(460, 40);
	pub_key_frame->move(314, 0);
	pub_key_label = new QLabel("Public key", pub_key_frame);
	pub_key_label->setFixedSize(440, 40);
	pub_key_label->move(10, 0);
	pub_key_label->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	pub_key_label->setFont(text_font);
	pub_key_label->setStyleSheet(text_style);

	//date headeer field
	date_frame = new QFrame(this);
	date_frame->setFixedSize(259, 40);
	date_frame->move(774, 0);
	date_label = new QLabel("Date", date_frame);
	date_label->setFixedSize(258, 40);
	date_label->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	date_label->setFont(text_font);
	date_label->setStyleSheet(text_style);
}

void table_header::set_search_enabled()
{
	//search button for keyname
	search_keyname_button = new QPushButton(key_name_frame);
	search_keyname_button->setStyleSheet("background-color:transparent;background-image: url(:/keymanager/srch_icon.png);border:0px;");
	search_keyname_button->setFixedSize(15, 15);
	search_keyname_button->move(291, 13);
	search_keyname_button->setCursor(Qt::PointingHandCursor);
	//search_keyname_box = new QLineEdit(key_name_frame);

	//search button for public key
	search_pub_key_button = new QPushButton(pub_key_frame);
	search_pub_key_button->setStyleSheet("background-color:transparent;background-image: url(:/keymanager/srch_icon.png);border:0px;");
	search_pub_key_button->setFixedSize(15, 15);
	search_pub_key_button->move(420, 13);
	search_pub_key_button->setCursor(Qt::PointingHandCursor);
	//search_pub_key_box = new QLineEdit(pub_key_frame);

}