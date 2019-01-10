#ifndef PASSWORDLINEEDIT_H
#define PASSWORDLINEEDIT_H

#include <QWidget>
#include <QKeyEvent>
#include <QLineEdit>
enum e_line_edit {main_line = 0, confirm_line};

class PasswordLineEdit : public QLineEdit
{
	Q_OBJECT

public:
	PasswordLineEdit(e_line_edit win, QWidget * parent = Q_NULLPTR);
	~PasswordLineEdit();

protected:
	void keyPressEvent(QKeyEvent *event) override;
	void focusInEvent(QFocusEvent *event) override;

signals:
	void finishEnter();
	void focus(int);

private:
	e_line_edit window;
};

#endif