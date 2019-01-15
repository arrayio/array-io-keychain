#include "menu_toolbar.hpp"

menu_toolbar::menu_toolbar(QWidget *parent)
	: QToolBar(parent)
{
    QAction *actionExport = new QAction("&Export", this);
    QAction *actionImport = new QAction("&Import", this);
    QAction *actionExit = new QAction("&Exit", this);

    QMenu *menuFile = new QMenu("&File");
    menuFile->setStyleSheet("QMenu {background-color: #fafafc;}");
    menuFile->setStyleSheet(menuFile->styleSheet() + "QMenu::item:selected {background-color: #9faec5;}");
    menuFile->addAction(actionExport);
    menuFile->addAction(actionImport);
    menuFile->addAction(actionExit);

    QAction *actionAbout = new QAction("&About", this);
    QAction *actionStatus = new QAction("&Status", this);

    QMenu *menuAbout = new QMenu("&About");
    menuAbout->setStyleSheet("QMenu {background-color: #fafafc;}");
    menuAbout->setStyleSheet(menuAbout->styleSheet() + "QMenu::item:selected {background-color: #9faec5;}");
    menuAbout->addAction(actionAbout);
    menuAbout->addAction(actionStatus);

    QPushButton *FileButton = new QPushButton("File");
    QPushButton *AboutButton = new QPushButton("About");
    FileButton->setFlat(true);
    AboutButton->setFlat(true);
    FileButton->setStyleSheet("QPushButton {background-color: #fafafc; \
                               font: bold; \
                               min-width: 4em; \
                               padding: 2px; \
                               border-style: outset; \
                               border-radius: 1em;}");
    FileButton->setStyleSheet(FileButton->styleSheet() + "QPushButton::menu-indicator{image: none;}");

    AboutButton->setStyleSheet("QPushButton {background-color: #fafafc; \
                                font: bold; \
                                min-width: 4em; \
                                margin-left: 7px; \
                                padding: 2px; \
                                border-style: outset; \
                                border-radius: 1em;}");
    AboutButton->setStyleSheet(AboutButton->styleSheet() + "QPushButton::menu-indicator{image: none;}");

    FileButton->setMenu(menuFile);
    AboutButton->setMenu(menuAbout);

    QToolBar *toolBarFile = new QToolBar();
    toolBarFile->addWidget(FileButton);
    toolBarFile->addWidget(AboutButton);
    
    this->addWidget(toolBarFile);

    //temporary use textedit for debug output
    text_edit_form = new QTextEdit();
    text_edit_form->setFixedSize(100, 30);
    text_edit_form->setStyleSheet("font:10px \"Segoe UI\";background:transparent;");
    this->addWidget(text_edit_form);

    connect(actionExit, SIGNAL(triggered()), this, SLOT(Exit()));
    connect(actionExport, SIGNAL(triggered()), this, SLOT(Export()));
    connect(actionImport, SIGNAL(triggered()), this, SLOT(Import()));
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(About()));
    connect(actionStatus, SIGNAL(triggered()), this, SLOT(Status()));
}

//call export dialog
void menu_toolbar::Export()
{
    text_edit_form->setText("Export was clicked!");
}

//call import dialog
void menu_toolbar::Import()
{
    text_edit_form->setText("Import was clicked!");
}

//handle exit event
void menu_toolbar::Exit()
{
    text_edit_form->setText("Exit was clicked!");
    QApplication::quit();
}

//print version info and other information 
void menu_toolbar::About()
{
    text_edit_form->setText("About was clicked!");
}

//print current keychain status ok / failed
void menu_toolbar::Status()
{
    text_edit_form->setText("Status was clicked!");
}