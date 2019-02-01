#include "remove_dialog.hpp"

remove_dialog * remove_dialog::r_instance = Q_NULLPTR;

Q_GLOBAL_STATIC(remove_dialog, r_instance)

remove_dialog::remove_dialog(QWidget *parent)
	:QDialog(parent)
{
    QMetaObject::connectSlotsByName(this);
    pWidth = parent->size().width();
    pHeight = parent->size().height();

    Q_ASSERT(!r_instance);
    r_instance = this;
}

remove_dialog* remove_dialog::instance()
{
    if (!r_instance)
        r_instance = new remove_dialog();
    return r_instance;
}

//popup delete key dialog
void remove_dialog::show_dialog(const QString& keyName)
{
    subDialog = new QDialog(this);
    subDialog->setWindowTitle("Delete key " + keyName);
    subDialog->show();
    subDialog->setFixedSize(300, 200);
    subDialog->move(pWidth - subDialog->width(), pHeight + subDialog->height());
}