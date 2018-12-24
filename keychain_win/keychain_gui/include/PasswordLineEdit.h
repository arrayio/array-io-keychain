#ifndef PASSWORDLINEEDIT_H
#define PASSWORDLINEEDIT_H

#include <QWidget>
#include <QKeyEvent>
#include <QLineEdit>

class PasswordLineEdit : public QLineEdit
{
	Q_OBJECT

public:
	PasswordLineEdit(QWidget * parent = Q_NULLPTR);
	~PasswordLineEdit();

protected:
	void keyPressEvent(QKeyEvent *event) override;

signals:
	void finishEnter();

};

#endif