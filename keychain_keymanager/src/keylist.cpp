#include "keylist.hpp"

keylist::keylist(QWidget *parent)
	:QWidget(parent)
{
	QMetaObject::connectSlotsByName(this);
	p_rows_count = 50;
	rows = new key_table_row*[p_rows_count];
	setFixedSize(1030, p_rows_count * 36);
	
	for (int i = 0; i < p_rows_count; i++) {
		*(rows + i) = new key_table_row(this);
		(*(rows + i))->setShortKeyInfo(QString("key_name%1").arg(i), QString("description%1").arg(i), QString("date%1").arg(i), i);
		if (i % 2 == 0) {
			(*(rows + i))->set_row_style_sheet("background-color:rgb(231,233,239);");
		}
		else {
			(*(rows + i))->set_row_style_sheet("background-color:rgb(255,255,255);");
		}
		////TODO: replace for height from widget
		(*(rows + i))->move(0, i * 36);
		connect((*(rows + i)), &key_table_row::row_expanded, this, &keylist::row_was_selected);
	}
}

void keylist::row_was_selected(int row_index)
{
	if (expanded_row_index == row_index)
		return;
	else {
		if (expanded_row_index == -1) {
			this->setFixedHeight(this->height() + (*(rows + row_index))->get_more_height());
			expanded_row_index = row_index;
		}
		else {
			(*(rows + expanded_row_index))->hide_more();
			(*(rows + expanded_row_index))->setFixedHeight(36);
			for (int i = expanded_row_index + 1; i < p_rows_count - expanded_row_index-1; i++) {
				QPoint _pos = (*(rows + i))->pos();
				QPoint posn(_pos.x(), _pos.y() - (*(rows + i))->get_more_height());
				(*(rows + i))->move(posn);
			}
			expanded_row_index = row_index;
		}
	}
	for (int i = expanded_row_index +1; i < p_rows_count - expanded_row_index -1; i++) {
		QPoint _pos = (*(rows + i))->pos();
		QPoint posn(_pos.x(), _pos.y()+ (*(rows + i))->get_more_height());
		(*(rows + i))->move(posn);
	}
	(*(rows + expanded_row_index))->show_more();
}