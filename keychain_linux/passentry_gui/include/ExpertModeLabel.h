#ifndef EXPERTMODELABEL_H
#define EXPERTMODELABEL_H

#include <QWidget>
#include <QLabel>
#include <QString>

class ExpertModeLabel : public QLabel
{
	Q_OBJECT
public:
	ExpertModeLabel(QWidget * parent = Q_NULLPTR);
	~ExpertModeLabel() {}

signals:
	void clicked();

protected:
	void mousePressEvent(QMouseEvent * event);

};
#endif
