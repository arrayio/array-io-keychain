#ifndef LOCKICON_H
#define LOCKICON_H

#include <QWidget>
#include <QFrame>
#include <QEvent>
#include <QMessageBox>
#include "PopupWindow.h"
#include "KeychainWarningMessage.h"

class LockIcon : public QFrame
{
	Q_OBJECT
public:
	LockIcon(const KeychainWarningMessage wMessage, QWidget * parent=Q_NULLPTR);
	~LockIcon();
	void setSourceDialog(PopupWindow * popup);
	void setUnSecureMode();
protected:
	void mouseMoveEvent(QMouseEvent *event);
	void leaveEvent(QEvent *event);
private:
	PopupWindow * _popup;
};
#endif