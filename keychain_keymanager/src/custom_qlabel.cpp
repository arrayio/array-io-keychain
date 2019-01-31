#include "custom_qlabel.hpp"


custom_qlabel::custom_qlabel(QWidget *parent)
    : QLabel(parent)
{
    //by default set style when custum_qlabel created
    setStyleSheet(this->styleSheet() + "QLabel {color: #9faec5;}");
}

//when QLabel on hover turn text color to new
//void custom_qlabel::enterEvent(QEvent *ev)
//{
//   setStyleSheet(this->styleSheet() + "QLabel {color: #9faec5;}");
//}

//when QLabel off hover turn text color back to default
//void custom_qlabel::leaveEvent(QEvent *ev)
//{
//   setStyleSheet(this->styleSheet() + "QLabel {color: #9faec5;}");
//}
