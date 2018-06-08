#include "widget.h"
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(tr("Promt for password"));
    CreateFormInterior();
}

Widget::~Widget()
{}

void Widget::CreateFormInterior()
{
    QGridLayout * grid = new QGridLayout(this);
    {
        QLabel *plb = new QLabel(tr("input password"), this);
        grid->addWidget(plb, 0, 0);
    }
    {
        QLineEdit *ple = new QLineEdit(this);
        ple->setEchoMode(QLineEdit::Password);
        grid->addWidget(ple, 0, 1, 1, 3);
    }
    {
        QLabel *plb = new QLabel(tr("raw transaction"), this);
        grid->addWidget(plb, 1, 0);
    }
    {
        QTextEdit * pte = new QTextEdit(this);
        grid->addWidget(pte, 1, 1, 2, 3);
        pte->setText("");
        pte->setReadOnly(true);
    }
    {
        QPushButton *plb = new QPushButton("Ok", this);
        grid->addWidget(plb, 3, 1);
        connect(plb, SIGNAL(clicked()), this, SLOT(close()));
    }
    {
        QPushButton * ppb = new QPushButton("Cancel", this);
        grid->addWidget(ppb, 3, 2);
        connect(ppb, SIGNAL(clicked()), this, SLOT(close()));
    }
}
