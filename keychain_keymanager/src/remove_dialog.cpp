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
    subDialog = new QDialog(this, Qt::CustomizeWindowHint);
    //drop system style behavior 
    subDialog->setWindowFlags(Qt::Window);
    subDialog->setWindowFlags(Qt::FramelessWindowHint);
    subDialog->setAttribute(Qt::WA_NoSystemBackground);
    subDialog->setAttribute(Qt::WA_TranslucentBackground);
    subDialog->setAttribute(Qt::WA_PaintOnScreen);

    custom_qlabel *titleBanner = new custom_qlabel(this);
    titleBanner->setObjectName("titlebnr");
    titleBanner->setText("Are you sure you want to delete the key \"" + static_cast<QString>(keyName) + "\"?");
    titleBanner->update_style("QLabel#titlebnr{background-color:#ffffff;color:black;}");
    titleBanner->setFont(QFont("Segoe UI Semibold", 10, QFont::Bold, false));

    QGridLayout* frameLayout = new QGridLayout(this);
    frameLayout->setSizeConstraint(QLayout::SetFixedSize);

    //frame to make borders round
    //QDialog works as without view styles
    QFrame *dlgFrame = new QFrame(this);
    dlgFrame->setObjectName("framedlg");
    dlgFrame->setFixedSize(400, 220);
    dlgFrame->setStyleSheet("QFrame#framedlg{background-color: #ffffff;\
                                    border-radius: 3px 3px 3px 3px; \
                                    border: 1px solid #ffffff;}");

    frameLayout->addWidget(dlgFrame);

    QVBoxLayout* elementsLayout = new QVBoxLayout(this);
    elementsLayout->setSizeConstraint(QLayout::SetFixedSize);
    elementsLayout->addWidget(titleBanner, 1, Qt::AlignCenter);

    QTextEdit *passphraseInput = new QTextEdit(this);
    passphraseInput->setText("Passphrase");
    passphraseInput->setFixedSize(200, 25);
    elementsLayout->addWidget(passphraseInput, 1, Qt::AlignCenter);
    elementsLayout->insertStretch(-1, 2);

    frameLayout->addLayout(elementsLayout, 0, 0);
    frameLayout->setSpacing(0);
    frameLayout->setContentsMargins(0, 0, 0, 0);
    subDialog->setLayout(frameLayout);
    subDialog->setFixedWidth(420);

    const QPoint newPoint = QPoint(0, 0) + QPoint(1.7*subDialog->width(), pHeight);
    const QPoint realPos = this->mapFromGlobal(newPoint);
    subDialog->move(realPos);
    subDialog->show();
}
