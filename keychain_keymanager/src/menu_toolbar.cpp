#include "menu_toolbar.hpp"

menu_toolbar::menu_toolbar(QWidget *parent)
	: QToolBar(parent)
{
    //create File menu actions for slots
    QAction *actionExport = new QAction("&Export", this);
    QAction *actionImport = new QAction("&Import", this);
    QAction *actionExit = new QAction("&Exit", this);

    QMenu *menuFile = new QMenu(this);
    menuFile->setStyleSheet("QMenu {background-color: #fafafc;}");
    menuFile->setStyleSheet(menuFile->styleSheet() + "QMenu::item:selected {background-color: #9faec5;}");
    menuFile->addAction(actionExport);
    menuFile->addAction(actionImport);
    menuFile->addAction(actionExit);

    //create About menu actions for slots
    QAction *actionAbout = new QAction("&About", this);
    QAction *actionStatus = new QAction("&Status", this);

    //create menu and add actions to it
    QMenu *menuAbout = new QMenu("&About", this);
    menuAbout->setStyleSheet("QMenu {background-color: #fafafc;}");
    menuAbout->setStyleSheet(menuAbout->styleSheet() + "QMenu::item:selected {background-color: #9faec5;}");
    menuAbout->addAction(actionAbout);
    menuAbout->addAction(actionStatus);

    //create file button
    QPushButton *FileButton = new QPushButton(this);
    FileButton->setStyleSheet(FileButton->styleSheet() + "QPushButton::menu-indicator{image: none;}");
    FileButton->setFixedSize(40, 20);

    //create file button text label
    custom_qlabel *FileButtonLabel = new custom_qlabel(FileButton);
    FileButtonLabel->setObjectName("filelbl");
    FileButtonLabel->setText("File");
    FileButtonLabel->setAlignment(Qt::AlignCenter);
    FileButtonLabel->setFont(QFont("Segoe UI Semibold", 10, QFont::Normal, false));
    
    //create about button text label
    QPushButton *AboutButton = new QPushButton(this);
    AboutButton->setStyleSheet(AboutButton->styleSheet() + "QPushButton::menu-indicator{image: none;}");
    AboutButton->setFixedSize(40, 20);

    //create about button text label
    custom_qlabel *Aboutbuttonlabel = new custom_qlabel(AboutButton);
    FileButtonLabel->setObjectName("aboutlabel");
    Aboutbuttonlabel->setText("About");
    Aboutbuttonlabel->setAlignment(Qt::AlignCenter);
    Aboutbuttonlabel->setFont(QFont("Segoe UI Semibold", 10, QFont::Normal, false));

    //create key button
    QPushButton *KeyButton = new QPushButton(this);
    KeyButton->setFixedSize(50, 17);

    //create key button text label
    custom_qlabel *Keybuttonlabel = new custom_qlabel(KeyButton);
    Keybuttonlabel->setAlignment(Qt::AlignCenter);

    //set flexy transformed pixmap for key button
    QPixmap keyBtnLogo(":/keymanager/tbr_key_icon.png");
    keyBtnLogo = keyBtnLogo.scaled(20, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    KeyButton->setIcon(keyBtnLogo);
    KeyButton->setIconSize(QSize(20, 16));
    
    //change buttons to flat style
    FileButton->setFlat(true);
    AboutButton->setFlat(true);
    KeyButton->setFlat(true);

    //add menu to buttons
    FileButton->setMenu(menuFile);
    AboutButton->setMenu(menuAbout);

    //add buttons to toolbar
    QToolBar *toolBarFile = new QToolBar(this);
    toolBarFile->addWidget(FileButton);
    toolBarFile->addWidget(AboutButton);
    toolBarFile->addWidget(KeyButton);
    
    //add toolbar to main layout
    this->addWidget(toolBarFile);

    //connect actions to slots via qt signals
    QObject::connect(actionExport, SIGNAL(triggered()), this, SLOT(Export()));
    QObject::connect(actionImport, SIGNAL(triggered()), this, SLOT(Import()));
    QObject::connect(actionAbout, SIGNAL(triggered()), this, SLOT(About()));
    QObject::connect(actionStatus, SIGNAL(triggered()), this, SLOT(Status()));
    QObject::connect(actionExit, SIGNAL(triggered()), this, SLOT(Exit()));
}

//call export dialog
void menu_toolbar::Export()
{
    emit ExportSelected("Export was clicked!");
}

//call import dialog
void menu_toolbar::Import()
{
    emit ImportSelected("Import was clicked!");
}

//call about dialog
void menu_toolbar::About()
{
    emit AboutSelected("About was clicked!");
}

//call status dialog
void menu_toolbar::Status()
{
    emit StatusSelected("Status was clicked!");
}

//call exit dialog
void menu_toolbar::Exit()
{
    emit ExitSelected("Exit was clicked!");
}