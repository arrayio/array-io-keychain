#include <keychain_table.hpp>

using namespace keychain_app;
using namespace std;
using namespace keyfile_format;
using keyfiles_map::keyfile_map_t;

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
	auto it = keyfile_singleton::instance().prim_index().begin();
	for (; it != keyfile_singleton::instance().prim_index().end(); ++it)
	{
		keyfile_t key_file = *it;
		std:string desc = key_file.description;
	}
	key_list->load_records();
}