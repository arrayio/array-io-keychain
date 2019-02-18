#ifndef KEYCHAIN_TABLE_HPP
#define KEYCHAIN_TABLE_HPP

#include <QObject>
#include <QWidget>
#include "keylist.hpp"
#include "key_scroll_area.hpp"
#include "table_header.hpp"
#include <keychain_lib/keychain_logger.hpp>

class keychain_table : public QWidget
{
	Q_OBJECT

public:
	keychain_table(QWidget *parent = Q_NULLPTR);
	void load_records();
	void init();

private:
	key_scrooll_area *scrooll_area;
	table_header *header;
	keylist *key_list;

private:
	int START_POS = 0;
	int ROW_HEIGHT = 44;
	int ROW_WIDTH = 1030;
};

#endif