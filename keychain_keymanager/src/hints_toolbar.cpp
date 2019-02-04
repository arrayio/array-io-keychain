#include "hints_toolbar.hpp"

hints_toolbar::hints_toolbar(QWidget *parent)
	: QToolBar(parent)
{
	//create minimize button
    QPushButton *MinimizeButton = new QPushButton(this);
    MinimizeButton->setObjectName("minbtn");
    MinimizeButton->setStyleSheet("QPushButton#minbtn:hover:!pressed{background-color:#eff2f5;border-style:inset;}");

	//create minimize button text label
    custom_qlabel *MinimizeButtonLabel = new custom_qlabel(MinimizeButton);
    MinimizeButtonLabel->setText(QStringLiteral("–")); //U + 2013
    MinimizeButtonLabel->setAlignment(Qt::AlignCenter);

    //set fixed size to make "border" frame equal to parent pushbutton frame
    MinimizeButtonLabel->setFixedSize(32, 21);
    MinimizeButton->setFixedSize(32, 21);

    //create maximize button
    QPushButton *MaximizeButton = new QPushButton(this);

    //create maximize button text label
    custom_qlabel *MaximizeButtonLabel = new custom_qlabel(MaximizeButton);
    MaximizeButtonLabel->setAlignment(Qt::AlignCenter);

    //set fixed size to make "border" frame equal to parent pushbutton frame
    MaximizeButtonLabel->setFixedSize(32, 21);

    //set flexy transformed pixmap for maximize button
    QPixmap maxBtnLogo(":/keymanager/maximize_btn_icon.png");
    maxBtnLogo = maxBtnLogo.scaled(32, 21, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    MaximizeButton->setIcon(maxBtnLogo);
    MaximizeButton->setIconSize(QSize(32, 21));
    MaximizeButton->setFixedSize(32, 21);

	//create close button
    QPushButton *CloseButton = new QPushButton(this);
    CloseButton->setObjectName("closebtn");
    CloseButton->setStyleSheet("QPushButton#closebtn:hover:!pressed{background-color:#c90b0b;border-style:inset;}");
	
	//create close button text label
    custom_qlabel *CloseButtonLabel = new custom_qlabel(CloseButton);
    CloseButtonLabel->setObjectName("closebtnlbl");
    CloseButtonLabel->setText("x");
    CloseButtonLabel->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    QFont clsButtonFont = CloseButtonLabel->font();
    clsButtonFont.setStyleStrategy(QFont::PreferAntialias);
    CloseButtonLabel->setFont(clsButtonFont);
    CloseButtonLabel->update_style("QLabel#closebtnlbl:{background-color:black;color:black;}");

    //set fixed size to make "border" frame equal to parent pushbutton frame
    CloseButtonLabel->setFixedSize(36, 15);
    CloseButton->setFixedSize(36, 21);
	
	//change buttons to flat style
    MinimizeButton->setFlat(true);
    MaximizeButton->setFlat(true);
    CloseButton->setFlat(true);
	
	//add buttons to toolbar
    QToolBar *toolBarHints = new QToolBar(this);
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
