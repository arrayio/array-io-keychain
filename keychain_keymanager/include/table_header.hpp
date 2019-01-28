#ifndef TABLEHEADER_HPP
#define TABLEHEADER_HPP

#include <QWidget>
#include <QObject>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QFrame>
#include <QLineEdit>

class table_header : public QWidget
{
	Q_OBJECT
public:
	table_header(QWidget *parent=Q_NULLPTR);
	void set_search_enabled();

//signals:
//	void search_key_name(QString query);
//	void search_public_key(QString query);

private:
	//keyname header field
	QFrame *key_name_frame;
	QLabel *key_name_label;
	QPushButton *search_keyname_button;
	QLineEdit *search_keyname_box;
	
	//public key header field
	QFrame *pub_key_frame;
	QLabel *pub_key_label;
	QPushButton *search_pub_key_button;
	QLineEdit *search_pub_key_box;

	//date headeer field
	QFrame *date_frame;
	QLabel *date_label;
};

#endif
