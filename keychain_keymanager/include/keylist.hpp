#ifndef KEYLIST_H
#define KEYLIST_H

#include <QObject>
#include <QDialog>
#include <QWidget>
#include "key_table_row.hpp"
#include "table_header.hpp"

class keylist : public QWidget
{
	Q_OBJECT

public:
	keylist(QWidget *parent=Q_NULLPTR);
	void load_records();

public slots:
	void row_was_selected(int row_index);

private:
	table_header *tbl_header;
	key_table_row **rows;
	int expanded_row_index = -1;
	int p_rows_count = 0;

private:
	int START_POS = 0;
	int ROW_HEIGHT = 44;
	int ROW_WIDTH=1030;
};

#endif
