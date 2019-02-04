#include "keylist.hpp"

keylist::keylist(QWidget *parent)
	:QWidget(parent)
{
	QMetaObject::connectSlotsByName(this);
}

void keylist::load_records()
{
	p_rows_count = 50;
	rows = new key_table_row*[p_rows_count];
	setFixedSize(ROW_WIDTH, p_rows_count * ROW_HEIGHT);
	for (int i = 0; i < p_rows_count; i++) {
		*(rows + i) = new key_table_row(this);
		(*(rows + i))->set_row_height(ROW_HEIGHT);
		(*(rows + i))->setShortKeyInfo(QString("key_name%1").arg(i), QString("96f12a84ffcfb7gg98yh7pjhy6hbb531<...>54d566g5"), QString("date%1").arg(i), i);
		QString test("96f12a84ffcfb7gg98yh7pjhy6hbb531<...>54d566g5");
		int test_len = test.length();
		if (i % 2 == 0) {
			(*(rows + i))->set_row_style_sheet("background-color:rgb(231,233,239);");
		}
		else {
			(*(rows + i))->set_row_style_sheet("background-color:rgb(255,255,255);");
		}
		(*(rows + i))->move(0, (i * ROW_HEIGHT) +START_POS);
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
			(*(rows + expanded_row_index))->setFixedHeight(ROW_HEIGHT);
			for (int i = expanded_row_index + 1; i < p_rows_count - expanded_row_index-1; i++) {
				QPoint _pos = (*(rows + i))->pos();
				QPoint posn(_pos.x(), _pos.y() - (*(rows + i))->get_more_height());
				(*(rows + i))->move(posn);
			}
			expanded_row_index = row_index;
		}
	}
	for (int i = expanded_row_index +1; i < p_rows_count - 1; i++) {
		QPoint _pos = (*(rows + i))->pos();
		QPoint posn(_pos.x(), _pos.y()+ (*(rows + i))->get_more_height());
		(*(rows + i))->move(posn);
	}
	(*(rows + expanded_row_index))->show_more();
}
