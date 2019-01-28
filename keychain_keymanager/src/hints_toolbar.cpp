#include "hints_toolbar.hpp"

hints_toolbar::hints_toolbar(QWidget *parent)
	: QToolBar(parent)
{
	//create minimize button
    QPushButton *MinimizeButton = new QPushButton(this);
	
	//create minimize button text label
    custom_qlabel *MinimizeButtonLabel = new custom_qlabel(MinimizeButton);
    MinimizeButtonLabel->setObjectName("minimizelabel");
    MinimizeButtonLabel->setText(QStringLiteral("–")); //U + 2013
    MinimizeButtonLabel->setAlignment(Qt::AlignCenter);
    MinimizeButtonLabel->setStyleSheet("QWidget#minimizelabel {background-color: #ffffff; \
                                        padding: 1px; \
                                        border-style: solid; \
                                        border-width: 1px; \
                                        border-radius: 0px 0px 3px 3px; \
                                        border-color: #8d8d8d;}");
    MinimizeButtonLabel->setFont(QFont("Segoe UI", 15, QFont::Light, false));
    MinimizeButtonLabel->resize(27, 23);
	
	//create maximize button
    QPushButton *MaximizeButton = new QPushButton(this);
	
	//create maximize button text label
    custom_qlabel *MaximizeButtonLabel = new custom_qlabel(MaximizeButton);
    MaximizeButtonLabel->setObjectName("maximizelabel");
    MaximizeButtonLabel->setText("[]");
    MaximizeButtonLabel->setAlignment(Qt::AlignCenter);
    MaximizeButtonLabel->setStyleSheet("QWidget#maximizelabel {background-color: #ffffff; \
                                        padding: 1px; \
                                        border-style: solid; \
                                        border-width: 1px; \
                                        border-radius: 0px 0px 3px 3px; \
                                        border-color: #8d8d8d;}");
    MaximizeButtonLabel->setFont(QFont("Segoe UI", 9, QFont::Light, false));
    MaximizeButtonLabel->resize(27, 23);
	
	//create close button
    QPushButton *CloseButton = new QPushButton(this);
	
	//create close button text label
    custom_qlabel *CloseButtonLabel = new custom_qlabel(CloseButton);
    CloseButtonLabel->setObjectName("closelabel");
    CloseButtonLabel->setText("X");
    CloseButtonLabel->setAlignment(Qt::AlignCenter);
    CloseButtonLabel->setStyleSheet("QWidget#closelabel {background-color: #c90b0b; \
                                     padding: 1px; \
                                     border-style: solid; \
                                     border-width: 1px; \
                                     border-radius: 0px 0px 3px 3px; \
                                     border-color: #c90b0b;}");
    CloseButtonLabel->setFont(QFont("Segoe UI", 13, QFont::Light, false));
    CloseButtonLabel->resize(27, 23);
	
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
