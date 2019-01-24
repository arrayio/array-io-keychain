#include "hints_toolbar.hpp"

hints_toolbar::hints_toolbar(QWidget *parent)
	: QToolBar(parent)
{
	//create minimize button
    QPushButton *MinimizeButton = new QPushButton(this);
    MinimizeButton->setStyleSheet(MinimizeButton->styleSheet() + "QPushButton::menu-indicator{image: none;}");
	
	//create minimize button text label
    custom_qlabel *MinimizeButtonLabel = new custom_qlabel(MinimizeButton);
    MinimizeButtonLabel->setText("-");
    MinimizeButtonLabel->setAlignment(Qt::AlignCenter);
    MinimizeButtonLabel->setStyleSheet("QWidget > QLabel {background-color: #ffffff; \
										text-align: center;}");
    MinimizeButtonLabel->setFont(QFont("Segoe UI Semibold", 10, QFont::Normal, false));
	
	//create maximize button
    QPushButton *MaximizeButton = new QPushButton(this);
    MaximizeButton->setStyleSheet(MaximizeButton->styleSheet() + "QPushButton::menu-indicator{image: none;}");
	
	//create maximize button text label
    custom_qlabel *MaximizeButtonLabel = new custom_qlabel(MaximizeButton);
    MaximizeButtonLabel->setText("[]");
    MaximizeButtonLabel->setAlignment(Qt::AlignCenter);
    MaximizeButtonLabel->setStyleSheet("QWidget > QLabel {background-color: #ffffff; \
										text-align: center;}");
    MaximizeButtonLabel->setFont(QFont("Segoe UI Semibold", 10, QFont::Normal, false));
	
	//create close button
    QPushButton *CloseButton = new QPushButton(this);
    CloseButton->setStyleSheet(CloseButton->styleSheet() + "QPushButton::menu-indicator{image: none;}");
	
	//create close button text label
    custom_qlabel *CloseButtonLabel = new custom_qlabel(CloseButton);
    CloseButtonLabel->setText("x");
    CloseButtonLabel->setAlignment(Qt::AlignCenter);
    CloseButtonLabel->setStyleSheet("QWidget > QLabel {background-color: #ffffff; \
									 text-align: center;}");
    CloseButtonLabel->setFont(QFont("Segoe UI Semibold", 10, QFont::Normal, false));
	
	//change buttons to flat style
    MinimizeButton->setFlat(true);
    MaximizeButton->setFlat(true);
    CloseButton->setFlat(true);
	
	//add buttons to toolbar
    QToolBar *toolBarHints = new QToolBar();
    toolBarHints->addWidget(MinimizeButton);
    toolBarHints->addWidget(MaximizeButton);
    toolBarHints->addWidget(CloseButton);
    
    //add toolbar to main layout
    this->addWidget(toolBarHints);
	
	//connect actions to slots via qt signals
    QObject::connect(MinimizeButton, SIGNAL(clicked()), this, SLOT(Minimize()));
    QObject::connect(MaximizeButton, SIGNAL(clicked()), this, SLOT(Maximize()));
    QObject::connect(CloseButton, SIGNAL(clicked()), this, SLOT(CloseWindow()));
	
}

//call minimize dialog
void hints_toolbar::Minimize()
{
    emit MinimizeSelected("Minimize was clicked!");
}

//call maximize dialog
void hints_toolbar::Maximize()
{
    emit MaximizeSelected("Maximize was clicked!");
}

//call exit dialog
void hints_toolbar::CloseWindow()
{
    emit CloseWindowSelected("CloseWindow was clicked!");
}
