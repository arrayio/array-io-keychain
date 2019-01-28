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
    MinimizeButtonLabel->setFixedSize(27, 23);
	
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
    CloseButtonLabel->setFixedSize(27, 23);
	
	//change buttons to flat style
    MinimizeButton->setFlat(true);
    CloseButton->setFlat(true);
	
	//add buttons to toolbar
    QToolBar *toolBarHints = new QToolBar();
    toolBarHints->addWidget(MinimizeButton);
    toolBarHints->addWidget(CloseButton);
    
    //add toolbar to main layout
    this->addWidget(toolBarHints);
	
	//connect actions to slots via qt signals
    QObject::connect(MinimizeButton, SIGNAL(clicked()), this, SLOT(Minimize()));
    QObject::connect(CloseButton, SIGNAL(clicked()), this, SLOT(CloseWindow()));
}

//call minimize dialog
void hints_toolbar::Minimize()
{
    emit MinimizeSelected("Minimize was clicked!");
}

//call exit dialog
void hints_toolbar::CloseWindow()
{
    emit CloseWindowSelected("CloseWindow was clicked!");
}
