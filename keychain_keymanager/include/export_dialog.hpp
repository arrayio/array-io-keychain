#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QWidget>
#include <QObject>
#include <QDialog>
#include <QFileDialog>
#include <QRegion>
#include <QLineEdit>
#include <QPushButton>
#include <QApplication>
#include <QGraphicsDropShadowEffect>
#include "hints_toolbar.hpp"

class export_dialog : public QDialog
{
	Q_OBJECT

public:
	export_dialog(QDialog *parent = Q_NULLPTR);
	void rounded_corners();

private:
	QFileDialog * select_file_dialog;
	QLineEdit * folder_path;
	QPushButton * but_sel_folder;
	QPushButton * but_export;
	QPushButton * but_close;
	hints_toolbar * hd_toolbar;
};

#endif