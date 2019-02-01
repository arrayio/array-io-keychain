#ifndef MOREDETAILS_HPP
#define MOREDETAILS_HPP

#include <QWidget>
#include <QObject>
#include <QLabel>
#include <QPushButton>
#include "custom_qlabel.hpp"

class more_details : public QWidget 
{
	Q_OBJECT
public:
	more_details(QWidget *parent = Q_NULLPTR);
	void set_details_value();
	int get_total_height();

private slots:
    void RemoveKey();

signals:
    void RemoveKeySelected(QString text);

private :
	QLabel *creation_date;
	QLabel *keychain_version;
	QLabel *cipher_type;
	QLabel *location;
	QLabel *description;
	QLabel *public_key;
	int p_total_height=0;
};

#endif