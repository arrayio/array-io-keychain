#include "keylist.hpp"

using namespace keychain_app;
using namespace std;
using namespace keyfile_format;
using keyfiles_map::keyfile_map_t;

keylist::keylist(QWidget *parent)
	:QWidget(parent)
{
	QMetaObject::connectSlotsByName(this);
}

void keylist::load_records()
{
	p_rows_count = 0; //50;
	rows = new key_table_row*[p_rows_count];
	auto it = keyfile_singleton::instance().prim_index().begin();
	std::vector<keylist_row_model> key_file_items;
	for (; it != keyfile_singleton::instance().prim_index().end(); ++it)
	{
		keyfile_t key_file = *it;
		keylist_row_model row_model;
		p_rows_count++;
		row_model.key_name = QString::fromStdString(key_file.keyname);
		row_model.public_key = QString::fromStdString(key_file.public_key().hex());
		row_model.last_date = QString::fromStdString(key_file.last_date());
		row_model.description = QString::fromStdString(key_file.description);
		row_model.keychain_version = QString::fromStdString(key_file.keychain_version);
		row_model.creation_date = QString::fromStdString(key_file.creation_time);
		key_file_items.push_back(row_model);
		//std:string desc3 = key_file.description;
	}
	setFixedSize(ROW_WIDTH, p_rows_count * ROW_HEIGHT);


	for (int i = 0; i < p_rows_count; i++) {
		*(rows + i) = new key_table_row(this);
		(*(rows + i))->set_row_height(ROW_HEIGHT);

		(*(rows + i))->setShortKeyInfo(key_file_items.at(i), i);
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
			for (int i = expanded_row_index + 1; i < p_rows_count - expanded_row_index; i++) {
				QPoint _pos = (*(rows + i))->pos();
				QPoint posn(_pos.x(), _pos.y() - (*(rows + i))->get_more_height());
				(*(rows + i))->move(posn);
			}
			expanded_row_index = row_index;
		}
	}
	for (int i = expanded_row_index + 1; i < p_rows_count; i++) {
		QPoint _pos = (*(rows + i))->pos();
		QPoint posn(_pos.x(), _pos.y() + (*(rows + i))->get_more_height());
		(*(rows + i))->move(posn);
	}
	(*(rows + expanded_row_index))->show_more();
}
