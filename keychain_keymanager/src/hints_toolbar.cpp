#include "hints_toolbar.hpp"

hints_toolbar::hints_toolbar(QWidget *parent)
	: QToolBar(parent)
{
	//create minimize button
    QPushButton *MinimizeButton = new QPushButton(this);
	
	//create minimize button text label
    custom_qlabel *MinimizeButtonLabel = new custom_qlabel(MinimizeButton);
    MinimizeButtonLabel->setText(QStringLiteral("–")); //U + 2013
    MinimizeButtonLabel->setAlignment(Qt::AlignCenter);

    //set fixed size to make "border" frame equal to parent pushbutton frame
    MinimizeButtonLabel->setFixedSize(30, 20);

    //set flexy transformed pixmap for minimize button
    QPixmap minimize_btn_logo(":/keymanager/minimize_btn_icon.png");
    minimize_btn_logo = minimize_btn_logo.scaled(30, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    MinimizeButton->setIcon(minimize_btn_logo);
    MinimizeButton->setIconSize(QSize(30, 20));
    MinimizeButton->setFixedSize(30, 20);

    //create maximize button
    QPushButton *MaximizeButton = new QPushButton(this);

    //create maximize button text label
    custom_qlabel *MaximizeButtonLabel = new custom_qlabel(MaximizeButton);
    MaximizeButtonLabel->setAlignment(Qt::AlignCenter);

    //set fixed size to make "border" frame equal to parent pushbutton frame
    MaximizeButtonLabel->setFixedSize(30, 20);

    //set flexy transformed pixmap for maximize button
    QPixmap maximize_btn_logo(":/keymanager/maximize_btn_icon.png");
    maximize_btn_logo = maximize_btn_logo.scaled(30, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    MaximizeButton->setIcon(maximize_btn_logo);
    MaximizeButton->setIconSize(QSize(30, 20));
    MaximizeButton->setFixedSize(30, 20);

	//create close button
    QPushButton *CloseButton = new QPushButton(this);
	
	//create close button text label
    custom_qlabel *CloseButtonLabel = new custom_qlabel(CloseButton);
    CloseButtonLabel->setText("x");
    CloseButtonLabel->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    CloseButtonLabel->setFont(QFont("Segoe UI", 13, QFont::Light, false));

    //set fixed size to make "border" frame equal to parent pushbutton frame
    CloseButtonLabel->setFixedSize(30, 20);

    //set flexy transformed pixmap for close button
    QPixmap close_btn_logo(":/keymanager/close_btn_icon.png");
    close_btn_logo = close_btn_logo.scaled(30, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    CloseButton->setIcon(close_btn_logo);
    CloseButton->setIconSize(QSize(30, 20));
    CloseButton->setFixedSize(30, 20);
	
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
