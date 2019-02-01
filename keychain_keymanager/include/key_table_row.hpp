#ifndef KEYTABLEROW_H
#define KEYTABLEROW_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QString>
#include "more_details.hpp"
#include "remove_dialog.hpp"

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
	void set_row_height(int row_height);
	int get_row_height();
	int get_more_height();

public:
	void mousePressEvent(QMouseEvent *event) override;
	//void leaveEvent(QEvent *event) override;
	//void mouseMoveEvent(QMouseEvent *event) override;
signals:
	void row_expanded(int rowIndex);

private:
	QFrame *key_name_frame;
	QLabel *key_name;
	QFrame *description_frame;
	QLabel *description;
	QFrame *date_frame;
	QLabel *date;
    std::unique_ptr< remove_dialog> removeDialog;
	more_details  *more;
	QString _prev_style_sheet;
	QLabel *test;
	int mp_row_index=-1;
	bool p_row_expanded=false;

	int mp_row_height;

	const int KEY_NAME_WIDTH= 314;
	const int P_KEY_WIDTH = 460;
	const int DATE_WIDTH = 259;

private:
	void set_selected();

public slots:
    //process remove key button click action
    void ProcessRemoveKey(const QString &text);
};

#endif
