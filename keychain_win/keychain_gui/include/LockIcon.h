#ifndef LOCKICON_H
#define LOCKICON_H

#include <QWidget>
#include <QFrame>
#include <QEvent>
#include <QMessageBox>
#include "PopupWindow.h"

class LockIcon : public QFrame
{
	Q_OBJECT
public:
	LockIcon(QWidget * parent=Q_NULLPTR);
	~LockIcon();
	void setSourceDialog(PopupWindow * popup);
protected:
	void mouseMoveEvent(QMouseEvent *event);
	void leaveEvent(QEvent *event);
private:
	PopupWindow * _popup;
};
#endif