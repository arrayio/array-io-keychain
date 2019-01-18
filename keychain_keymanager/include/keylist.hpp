#ifndef KEYLIST_H
#define KEYLIST_H

#include <QObject>
#include <QDialog>
#include <QWidget>
#include "key_table_row.hpp"

class keylist : public QWidget
{
	Q_OBJECT

public:
	keylist(QWidget *parent=Q_NULLPTR);

public slots:
	void row_was_selected(int row_index);

private:
	key_table_row **rows;
	int expanded_row_index = -1;
	int p_rows_count = 0;
};

#endif
