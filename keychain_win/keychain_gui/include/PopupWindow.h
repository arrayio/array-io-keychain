#ifndef POPUP_WINDOW_H
#define POPUP_WINDOW_H
#include <QWidget>
#include <QFrame>
#include <QLabel>

class PopupWindow : public QFrame
{
	Q_OBJECT
public:
	PopupWindow(QWidget* parent=Q_NULLPTR);
	void setUnsecureText();
	~PopupWindow();

private:
	QLabel * header;
	QLabel * body;
	QLabel * more;
};
#endif

