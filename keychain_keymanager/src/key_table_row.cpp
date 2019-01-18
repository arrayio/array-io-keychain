#include "key_table_row.hpp"

key_table_row::key_table_row(QWidget *parent)
	: QWidget(parent)
{
	QMetaObject::connectSlotsByName(parent);
	key_name = new QLabel(this);
	description = new QLabel(this);
	date = new QLabel(this);
	setFixedSize(1030, 36);
	setCursor(Qt::PointingHandCursor);
	//set sizes of table fields
	key_name->setFixedSize(355, 36);
	description->setFixedSize(418, 36);
	date->setFixedSize(267, 36);
	//move fields on the place of columns
	description->move(355, 0);
	date->move(773, 0);
	//more_area initialization
	more = new more_details(this);
	more->move(0, 36);
	//set fonts
	QFont font("Cartograph Mono CF");
	font.setPixelSize(12);
	key_name->setFont(font);
	description->setFont(font);
	date->setFont(font);
	//set padding
	key_name->setAlignment(Qt::AlignVCenter);
	key_name->setStyleSheet("margin:10px 0px 0px 0px;");
	description->setAlignment(Qt::AlignVCenter);
	description->setStyleSheet("margin:10px 0px 0px 0px;");
	date->setAlignment(Qt::AlignVCenter);
	date->setStyleSheet("margin:10px 0px 0px 0px;");
}

void key_table_row::setShortKeyInfo(QString key_name_str, QString description_str, QString date_str, int row_index)
{
	key_name->setText(key_name_str);
	description->setText(description_str);
	date->setText(date_str);
	mp_row_index = row_index;
}

void key_table_row::set_row_style_sheet(QString style_sheet)
{
	key_name->setStyleSheet(style_sheet);
	description->setStyleSheet(style_sheet);
	date->setStyleSheet(style_sheet);
	_prev_style_sheet = style_sheet;
}

void key_table_row::mousePressEvent(QMouseEvent * event)
{
	emit row_expanded(mp_row_index);
}

void key_table_row::leaveEvent(QEvent * event)
{
	key_name->setStyleSheet(_prev_style_sheet);
	description->setStyleSheet(_prev_style_sheet);
	date->setStyleSheet(_prev_style_sheet);
}

void key_table_row::mouseMoveEvent(QMouseEvent * event)
{
	key_name->setStyleSheet("background-color:rgb(165,178,200);");
	description->setStyleSheet("background-color:rgb(165,178,200);");
	date->setStyleSheet("background-color:rgb(165,178,200);");
}

void key_table_row::show_more()
{
	if (p_row_expanded)
		return;
	p_row_expanded = !p_row_expanded;
	setFixedHeight(get_row_height());
	more->set_details_value();
}

void key_table_row::hide_more()
{
	key_name->setStyleSheet(_prev_style_sheet);
	description->setStyleSheet(_prev_style_sheet);
	date->setStyleSheet(_prev_style_sheet);
}

bool key_table_row::is_row_expanded()
{
	return p_row_expanded;
}

int key_table_row::get_row_height()
{
	return 36+more->get_total_height();
}

int key_table_row::get_more_height()
{
	return more->get_total_height();
}


