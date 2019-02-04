#ifndef MORE_BUTTON_HPP
#define MORE_BUTTON_HPP

#include <QPushButton>
#include <QIcon>
#include <QObject>

class more_button : public QPushButton
{
	Q_OBJECT

public:
	more_button(QWidget *parent = Q_NULLPTR);
	void set_edit_mode();
	void set_remove_mode();
protected:
	void mouseMoveEvent(QMouseEvent *e) override;
	void leaveEvent(QEvent *event) override;
};

#endif
