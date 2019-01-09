#ifndef PRIVATEKEYINMEMORY_H
#define PRIVATEKEYINMEMORY_H

#include <QWidget>
#include <QLabel>

class PrivateKeyInMemory : public QWidget
{
	Q_OBJECT
public:
	PrivateKeyInMemory(QWidget * parent = Q_NULLPTR);
	void SetPosition(int x, int y, int labelWidth, int valueWidth);
	void SetTime(QString timeValueStr);
	~PrivateKeyInMemory();

private:
	QLabel * timeLabel;
	QLabel * timeValue;
	int currentHeight = 0;
	int currentWidth = 0;
};

#endif


