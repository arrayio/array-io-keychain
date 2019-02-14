#include "menu_toolbar.hpp"

menu_toolbar::menu_toolbar(QWidget *parent)
	: QToolBar(parent)
{
    QMetaObject::connectSlotsByName(this);
    //create file button
    FileButton = new menu_button(this);
    FileButton->setObjectName("filebtn");
    FileButton->setFixedSize(45, 24);
    FileButton->setAttribute(Qt::WA_Hover);
    FileButton->setMouseTracking(true);

    //create file button text label
    custom_qlabel *FileButtonLabel = new custom_qlabel(FileButton);
    FileButtonLabel->setText("File");
    FileButtonLabel->setAlignment(Qt::AlignCenter);
    FileButtonLabel->setFont(QFont("Segoe UI Semibold", 12, QFont::Bold, false));
    FileButtonLabel->setFixedSize(42, 24);

    //create file menu
    menuFile = new custom_menu(FileButton, this);
    menuFile->setObjectName("filemenupopup");

    //create File menu actions for slots
    actionCreate = new QAction("&Create key", this);
    actionExport = new QAction("&Export file", this);
    actionImport = new QAction("&Import file", this);
    actionExit = new QAction("&Exit", this);
    menuFile->addAction(actionCreate);
    menuFile->addAction(actionImport);
    menuFile->addAction(actionExport);
    menuFile->addAction(actionExit);
    
    //create about button text label
    AboutButton = new menu_button(this);
    AboutButton->setObjectName("aboutbtn");
    AboutButton->setFixedSize(55, 24);
    AboutButton->setAttribute(Qt::WA_Hover);
    AboutButton->setMouseTracking(true);

    //create about button text label
    custom_qlabel *Aboutbuttonlabel = new custom_qlabel(AboutButton);
    Aboutbuttonlabel->setText("About");
    Aboutbuttonlabel->setAlignment(Qt::AlignCenter);
    Aboutbuttonlabel->setFont(QFont("Segoe UI Semibold", 12, QFont::Bold, false));
    Aboutbuttonlabel->setFixedSize(52, 24);

    //create about menu
    menuAbout = new custom_menu(AboutButton, this);
    menuAbout->setObjectName("aboutmenupopup");

    //create About menu actions for slots
    actionAbout = new QAction("&About", this);
    actionStatus = new QAction("&Status", this);
    menuAbout->addAction(actionAbout);
    menuAbout->addAction(actionStatus);

    //create key button
    KeyButton = new menu_button(this);
    KeyButton->setStyleSheet("");
    KeyButton->setObjectName("keybtn");
    KeyButton->setFixedSize(40, 18);

    //create key button text label
    custom_qlabel *Keybuttonlabel = new custom_qlabel(KeyButton);
    Keybuttonlabel->setAlignment(Qt::AlignCenter);

    //set flexy transformed pixmap for key button
    QPixmap keyBtnLogo(":/keymanager/tbr_key_icon.png");
    keyBtnLogo = keyBtnLogo.scaled(22, 18, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    KeyButton->setIcon(keyBtnLogo);
    KeyButton->setIconSize(QSize(22, 18));
    
    //change buttons to flat style
    FileButton->setFlat(true);
    AboutButton->setFlat(true);
    KeyButton->setFlat(true);

    //add menu to buttons
    FileButton->setMenu(menuFile);
    AboutButton->setMenu(menuAbout);

    //add horizontal elements layout
    QHBoxLayout* elementsLayout = new QHBoxLayout(this);
    elementsLayout->setSizeConstraint(QLayout::SetFixedSize);
    elementsLayout->addWidget(FileButton, 1);
    elementsLayout->addWidget(AboutButton, 1);
    elementsLayout->addWidget(KeyButton, 1);

    //add top level layout as elements layout handler
    QGridLayout* frameLayout = new QGridLayout(this);
    frameLayout->setSizeConstraint(QLayout::SetFixedSize);

    //aggregate elements via extension widget
    QWidget *extension = new QWidget(this);
    extension->setLayout(frameLayout);
    frameLayout->addLayout(elementsLayout, 0, 0);
    extension->setLayout(frameLayout);

    //add toolbar to main layout
    this->addWidget(extension);

    //emit flags for menu if it starts to be active
    QObject::connect(menuFile, &custom_menu::aboutToShow, menuFile, &custom_menu::handleShowEvent);
    QObject::connect(menuAbout, &custom_menu::aboutToShow, menuAbout, &custom_menu::handleShowEvent);

    //connect actions to slots via qt signals
    QObject::connect(actionCreate, SIGNAL(triggered()), this, SLOT(Create()));
    QObject::connect(actionExport, SIGNAL(triggered()), this, SLOT(Export()));
    QObject::connect(actionImport, SIGNAL(triggered()), this, SLOT(Import()));
    QObject::connect(actionAbout, SIGNAL(triggered()), this, SLOT(About()));
    QObject::connect(actionStatus, SIGNAL(triggered()), this, SLOT(Status()));
    QObject::connect(actionExit, SIGNAL(triggered()), this, SLOT(Exit()));
}

void menu_toolbar::enterEvent(QEvent *e)
{
    QWidget *pWin = QApplication::activeWindow();

    menu_button* pFileBtn = pWin->findChild<menu_button*>("filebtn");
    menu_button* pAboutBtn = pWin->findChild<menu_button*>("aboutbtn");
    custom_menu* pMenuFile = pWin->findChild<custom_menu*>("filemenupopup");
    custom_menu* pMenuAbout = pWin->findChild<custom_menu*>("aboutmenupopup");

    QPoint currentPos = QWidget::mapToGlobal(QCursor::pos());

    QRect pRectFile = pFileBtn->geometry();
    QPoint rectFileLeftSide = QWidget::mapToGlobal(QPoint(pRectFile.left(), pRectFile.top()));
    QPoint rectFileRightSide = QWidget::mapToGlobal(QPoint(pRectFile.right(), pRectFile.bottom()));
    QRect pRectFileGlobal(rectFileLeftSide.x(), rectFileLeftSide.y(), rectFileRightSide.x(), rectFileRightSide.y());

    QRect pRectAbout = pAboutBtn->geometry();
    QPoint rectAboutLeftSide = QWidget::mapToGlobal(QPoint(pRectAbout.left(), pRectAbout.top()));
    QPoint rectAboutRightSide = QWidget::mapToGlobal(QPoint(pRectAbout.right(), pRectAbout.bottom()));
    QRect pRectAboutGlobal(rectAboutLeftSide.x(), rectAboutLeftSide.y(), rectAboutRightSide.x(), rectAboutRightSide.y());

    QStateMachine *machine = new QStateMachine(this);

    QState *visibleFileMenu = new QState();
    visibleFileMenu->assignProperty(pMenuFile, "visible", QVariant(true));

    QState *closeFileMenu = new QState();
    closeFileMenu->assignProperty(pMenuFile, "visible", QVariant(false));

    QState *visibleAboutMenu = new QState();
    visibleAboutMenu->assignProperty(pMenuAbout, "visible", QVariant(true));

    QState *closeAboutMenu = new QState();
    closeAboutMenu->assignProperty(pMenuAbout, "visible", QVariant(false));

    QSize pFileBtnSize = pFileBtn->size();
    QSize pAboutBtnSize = pAboutBtn->size();

    bool fileActive = pMenuFile->isActive();
    bool aboutActive = pMenuAbout->isActive();

    if (pRectFileGlobal.contains(currentPos))
    {
        QEventTransition *pressTransitionAbout = new QEventTransition(pAboutBtn, QEvent::Enter);
        pressTransitionAbout->setTargetState(visibleAboutMenu);
        visibleAboutMenu->addTransition(pressTransitionAbout);

        QEventTransition *releaseTransitionAbout = new QEventTransition(pAboutBtn, QEvent::Leave);
        releaseTransitionAbout->setTargetState(closeAboutMenu);
        closeAboutMenu->addTransition(releaseTransitionAbout);

        machine->addState(visibleAboutMenu);
        machine->addState(closeAboutMenu);

        machine->setInitialState(visibleAboutMenu);
        machine->start();
    }

    if (pRectAboutGlobal.contains(currentPos))
    {
        QEventTransition *pressTransitionFile = new QEventTransition(pFileBtn, QEvent::Enter);
        pressTransitionFile->setTargetState(visibleFileMenu);
        visibleFileMenu->addTransition(pressTransitionFile);

        QEventTransition *releaseTransitionFile = new QEventTransition(pFileBtn, QEvent::Leave);
        releaseTransitionFile->setTargetState(closeFileMenu);
        closeFileMenu->addTransition(releaseTransitionFile);

        machine->addState(visibleFileMenu);
        machine->addState(closeFileMenu);

        machine->setInitialState(visibleFileMenu);
        machine->start();
    }

    machine->stop();

    QWidget::enterEvent(e);
}

//call create dialog
void menu_toolbar::Create()
{
    emit CreateSelected("Create was clicked!");
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
