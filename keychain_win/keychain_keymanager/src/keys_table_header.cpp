#include "keys_table_header.hpp"

key_table_header::key_table_header(QTreeWidget *parent)
	: QTreeWidgetItem(parent)
{
	QColor bgColor(140,155,181);
	QColor txtColor(255, 255, 255);
	QFont txtFont("Arial");
	txtFont.setPixelSize(14);

	setText(0, "Keyname");
	setText(1, "Description");
	setText(2, "Date");

	setBackgroundColor(0, bgColor);
	setForeground(0, txtColor);
	setFont(0, txtFont);
	setBackgroundColor(1, bgColor);
	setForeground(1, txtColor);
	setFont(1, txtFont);
	setBackgroundColor(2, bgColor);
	setForeground(2, txtColor);
	setFont(2, txtFont);

}