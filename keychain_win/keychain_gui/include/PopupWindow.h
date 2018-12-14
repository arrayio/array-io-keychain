#ifndef POPUP_WINDOW_H
#define POPUP_WINDOW_H
#include <QWidget>
#include <QFrame>
#include <QLabel>
#include "KeychainWarningMessage.h"

class PopupWindow : public QFrame
{
	Q_OBJECT
public:
	PopupWindow(const KeychainWarningMessage wMessage, QWidget* parent=Q_NULLPTR);
	~PopupWindow();

private:
	QLabel * header;
	QLabel * body;
};
#endif

