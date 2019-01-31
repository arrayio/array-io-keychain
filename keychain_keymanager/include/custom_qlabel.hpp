#ifndef  CUSTOMQLABEL_H
#define  CUSTOMQLABEL_H

#include <QObject>
#include <QWidget>
#include <QLabel>

class custom_qlabel : public QLabel
{
    Q_OBJECT
public:
    custom_qlabel(QWidget* parent = Q_NULLPTR);

//protected:
//    void custom_qlabel::enterEvent(QEvent *ev) override; 
//    void custom_qlabel::leaveEvent(QEvent *ev) override;
};


#endif
