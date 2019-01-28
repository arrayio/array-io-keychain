#include "key_table_row.hpp"

key_table_row::key_table_row(QWidget *parent)
	: QWidget(parent)
{
	QMetaObject::connectSlotsByName(parent);
}

void key_table_row::setShortKeyInfo(QString key_name_str, QString description_str, QString date_str, int row_index)
{
	key_name_frame = new QFrame(this);
	description_frame = new QFrame(this);
	date_frame = new QFrame(this);
	key_name = new QLabel(key_name_frame);
	description = new QLabel(description_frame);
	date = new QLabel(date_frame);
	setFixedSize(KEY_NAME_WIDTH + P_KEY_WIDTH + DATE_WIDTH, mp_row_height);
	setCursor(Qt::PointingHandCursor);
	//set sizes of labels
	key_name->setFixedSize(KEY_NAME_WIDTH - 20, mp_row_height);
	description->setFixedSize(P_KEY_WIDTH - 20, mp_row_height);
	date->setFixedSize(DATE_WIDTH - 20, mp_row_height);
	key_name->move(15, 0);
	description->move(10, 0);
	date->move(10, 0);
	//set sizes of table fields
	key_name_frame->setFixedSize(KEY_NAME_WIDTH, mp_row_height);
	description_frame->setFixedSize(P_KEY_WIDTH, mp_row_height);
	date_frame->setFixedSize(DATE_WIDTH, mp_row_height);
	//move fields on the place of columns
	description_frame->move(KEY_NAME_WIDTH, 0);
	date_frame->move(KEY_NAME_WIDTH + P_KEY_WIDTH, 0);
	//more_area initialization
	more = new more_details(this);
	more->move(0, mp_row_height);
	//set fonts
	QFont font("Cartograph Mono CF");
	font.setPixelSize(14);
	key_name->setFont(font);
	description->setFont(font);
	date->setFont(font);
	//set padding
	key_name->setAlignment(Qt::AlignVCenter);
	description->setAlignment(Qt::AlignVCenter);
	date->setAlignment(Qt::AlignVCenter);
	key_name->setText(key_name_str);
	description->setText(description_str);
	date->setText(date_str);
	mp_row_index = row_index;
}

void key_table_row::set_row_style_sheet(QString style_sheet)
{
	key_name_frame->setStyleSheet(style_sheet);
	description_frame->setStyleSheet(style_sheet);
	date_frame->setStyleSheet(style_sheet);
	_prev_style_sheet = style_sheet;
}

void key_table_row::mousePressEvent(QMouseEvent * event)
{
	emit row_expanded(mp_row_index);
}

//void key_table_row::leaveEvent(QEvent * event)
//{
//	key_name_frame->setStyleSheet(_prev_style_sheet);
//	description_frame->setStyleSheet(_prev_style_sheet);
//	date_frame->setStyleSheet(_prev_style_sheet);
//}

//void key_table_row::mouseMoveEvent(QMouseEvent * event)
//{
//	//set_selected();
//}

void key_table_row::set_selected()
{
	key_name_frame->setStyleSheet("background-color:rgb(165,178,200);");
	description_frame->setStyleSheet("background-color:rgb(165,178,200);");
	date_frame->setStyleSheet("background-color:rgb(165,178,200);");
}

void key_table_row::show_more()
{
	if (p_row_expanded)
		return;
	p_row_expanded = !p_row_expanded;
	setFixedHeight(get_row_height());
	more->set_details_value();
	set_selected();
}

void key_table_row::hide_more()
{
	p_row_expanded = !p_row_expanded;
	key_name_frame->setStyleSheet(_prev_style_sheet);
	description_frame->setStyleSheet(_prev_style_sheet);
	date_frame->setStyleSheet(_prev_style_sheet);
}

bool key_table_row::is_row_expanded()
{
	return p_row_expanded;
}

void key_table_row::set_row_height(int row_height)
{
	mp_row_height = row_height;
}


int key_table_row::get_row_height()
{
	return mp_row_height+more->get_total_height();
}

int key_table_row::get_more_height()
{
	return more->get_total_height();
}


