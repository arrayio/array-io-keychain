#ifndef KEYTABLEROW_H
#define KEYTABLEROW_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QString>
#include "more_details.hpp"

class key_table_row : public QWidget
{
	Q_OBJECT
public:
	key_table_row(QWidget *parent = Q_NULLPTR);
	void setShortKeyInfo(QString keyName, QString description, QString date, int row_index);
	void set_row_style_sheet(QString style_sheet);
	void show_more();
	void hide_more();
	bool is_row_expanded();
	int get_row_height();
	int get_more_height();

public:
	void mousePressEvent(QMouseEvent *event) override;
	void leaveEvent(QEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
signals:
	void row_expanded(int rowIndex);

private:
	QLabel *key_name;
	QLabel *description;
	QLabel *date;
	more_details  *more;
	QString _prev_style_sheet;
	QLabel *test;
	int mp_row_index=-1;
	bool p_row_expanded=false;
};

#endif
