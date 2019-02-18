#include "keymanager_dialog.hpp"

keymanager_dialog::keymanager_dialog(QWidget * parent)
	: QDialog(parent) 
{
    //define common dialog properties
    QMetaObject::connectSlotsByName(this);
    setWindowTitle(QApplication::translate("keychain_gui_winClass", "keychain_gui_win", nullptr));
	setWindowFlags(Qt::FramelessWindowHint);
    resize(1100, 760);
    setStyleSheet("background-color:rgb(242,243,246);");
	//QFontDatabase

	QFontDatabase::addApplicationFont(":/fonts/Cartograph Mono CF/Cartograph Mono CF Regular.ttf");
	QFontDatabase::addApplicationFont(":/fonts/Cartograph Mono CF/Cartograph Sans CF Regular.ttf");
}

void keymanager_dialog::init()
{
	//
	//define header properties
	header = new QLabel(this);
	header->setFixedSize(this->width(), 50);
	header->setStyleSheet("background-color:rgb(255,255,255);");
	//define logo picture
	QPixmap logo(":/keymanager/keychain_logo.png");
	logo = logo.scaled(85, 31, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	logoLabel = new QLabel(this);
	logoLabel->setStyleSheet("background:transparent;");
	logoLabel->setPixmap(logo);
	logoLabel->move(36, 12);
	//end dialog initialization

    //initialize keys table
	keysTable = new keychain_table(this);
    keysTable->move(32, 90);
    keysTable->init();
    //keysTable->load_records(); //remove temporary

	//create menu toolbar 
	toolbar = new menu_toolbar(this);
	toolbar->setStyleSheet("font:\"Segoe UI\";background:transparent;");
	toolbar->move(74, 7);

	//create text-edit as temporary GUI element
	textEditform = new QTextEdit(this);
    textEditform->setFixedSize(100, 30);
    textEditform->setStyleSheet("font:10px\"Segoe UI\";background:transparent;");
    textEditform->move(250, 10);

    QObject::connect(toolbar, SIGNAL(CreateSelected(QString)), this, SLOT(ProcessCreate(QString)));
	QObject::connect(toolbar, SIGNAL(ExportSelected(QString)), this, SLOT(ProcessExport(QString)));
	QObject::connect(toolbar, SIGNAL(ImportSelected(QString)), this, SLOT(ProcessImport(QString)));
	QObject::connect(toolbar, SIGNAL(AboutSelected(QString)), this, SLOT(ProcessAbout(QString)));
	QObject::connect(toolbar, SIGNAL(StatusSelected(QString)), this, SLOT(ProcessStatus(QString)));
	QObject::connect(toolbar, SIGNAL(ExitSelected(QString)), this, SLOT(ProcessExit(QString)));

    //create window hints toolbar 
    hintsbar = new hints_toolbar(this);
    hintsbar->setStyleSheet("font:\"Segoe UI\";background:transparent;");
    //set absolute position
    //because width delta between frame geometry width and width is equal zero
    hintsbar->move(985, -5);

    //connect to hintsbar actions
    QObject::connect(hintsbar, SIGNAL(MinimizeSelected(QString)), this, SLOT(ProcessMinimize(QString)));
    QObject::connect(hintsbar, SIGNAL(CloseWindowSelected(QString)), this, SLOT(ProcessExit(QString)));
}

//process create dialog signal call
void keymanager_dialog::ProcessCreate(const QString &text)
{
    textEditform->setText("Process Create!");
}

//process export dialog signal call
void keymanager_dialog::ProcessExport(const QString &text)
{
	export_dialog dlg(this);
	dlg.exec();
    textEditform->setText("Process Export!");
}

//process import dialog signal call
void keymanager_dialog::ProcessImport(const QString &text)
{
    textEditform->setText("Process Import!");
}

//process about dialog signal call
void keymanager_dialog::ProcessAbout(const QString &text)
{
    textEditform->setText("Process About!");
}

//process status dialog signal call
void keymanager_dialog::ProcessStatus(const QString &text)
{
    textEditform->setText("Process Status!");
}

//process exit dialog signal call
void keymanager_dialog::ProcessExit(const QString &text)
{
    textEditform->setText("Process Exit!");
    QApplication::quit();
}

//process minimize signal call
void keymanager_dialog::ProcessMinimize(const QString &text)
{
    textEditform->setText("Process Minimize!");
    showMinimized();
}

//process hint events
void keymanager_dialog::changeEvent(QEvent* e)
{
    if (e->type() == QEvent::WindowStateChange)
    {
        QWindowStateChangeEvent* event = static_cast<QWindowStateChangeEvent*>(e);

        if (this->windowState() & Qt::WindowMaximized)
        {
            if (event->oldState() & Qt::WindowNoState)
            {
                showMaximized();
            }
            else if (event->oldState() == Qt::WindowMaximized)
            {
                showNormal();
            }
        }
    }
}
