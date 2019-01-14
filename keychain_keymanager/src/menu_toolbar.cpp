#include "menu_toolbar.hpp"

menu_toolbar::menu_toolbar(QWidget *parent)
	: QToolBar(parent)
{
    QAction *actionExport = new QAction("&Export", this);
    QAction *actionImport = new QAction("&Import", this);
    QAction *actionExit = new QAction("&Exit", this);

    QMenu *menuFile = new QMenu("&File");
    menuFile->setStyleSheet("QMenu::item:selected {background-color: #9faec5;}");
    menuFile->addAction(actionExport);
    menuFile->addAction(actionImport);
    menuFile->addAction(actionExit);

    QAction *actionAbout = new QAction("&About", this);
    QAction *actionStatus = new QAction("&Status", this);

    QMenu *menuAbout = new QMenu("&About");
    menuAbout->setStyleSheet("QMenu::item:selected {background-color: #9faec5;}");
    menuAbout->addAction(actionAbout);
    menuAbout->addAction(actionStatus);

    QPushButton *FileButton = new QPushButton("File");
    QPushButton *AboutButton = new QPushButton("About");
    FileButton->setFlat(true);
    AboutButton->setFlat(true);
    FileButton->setStyleSheet("background-color: #fafafc; \
                               font: bold; \
                               min-width: 4em; \
                               padding: 2px; \
                               border-style: outset; \
                               border-radius: 1em;");
    AboutButton->setStyleSheet("background-color: #fafafc; \
                                font: bold; \
                                min-width: 4em; \
                                margin-left: 7px; \
                                padding: 2px; \
                                border-style: outset; \
                                border-radius: 1em;");
    AboutButton->move(32, 30);

    FileButton->setMenu(menuFile);
    AboutButton->setMenu(menuAbout);

    QToolBar *toolBarFile = new QToolBar();
    toolBarFile->addWidget(FileButton);
    toolBarFile->addWidget(AboutButton);

    this->addWidget(toolBarFile);

    connect(actionExit, SIGNAL(triggered()), this, SLOT(Exit()));
}

void menu_toolbar::Export()
{

}

void menu_toolbar::Import()
{

}

void menu_toolbar::Exit()
{

}

void menu_toolbar::About()
{

}

void menu_toolbar::Status()
{

}