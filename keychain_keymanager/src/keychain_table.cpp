#include <keychain_table.hpp>



keychain_table::keychain_table(QWidget *parent)
	:QWidget(parent)
{
	QMetaObject::connectSlotsByName(this);
	scrooll_area = new key_scrooll_area(this);
	header = new table_header(this);
	key_list = new keylist(this);
}

void keychain_table::init()
{
	//configuring header
	header->move(0, 0);
	header->set_search_enabled();

	//configuring scroll area
	scrooll_area->setFixedSize(1040, 600);
	scrooll_area->move(0, 40);
	scrooll_area->setWidget(key_list);
}

void keychain_table::load_records()
{
	
	key_list->load_records();
}