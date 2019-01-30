#include "more_details.hpp"

more_details::more_details(QWidget *parent)
	:QWidget(parent)
{
	QMetaObject::connectSlotsByName(this);
	QFont font("Cartograph Sans CF");
	font.setPixelSize(14);
	setStyleSheet("background-color:rgb(214,220,229);");
	//initialize gui items
	int start_position = 0;
	int height = 30;
	creation_date = new QLabel(this);
	creation_date->setFixedSize(1030, height-5);
	keychain_version = new QLabel(this);
	keychain_version->setFixedSize(1030, height);
	cipher_type = new QLabel(this);
	cipher_type->setFixedSize(1030, height);
	location = new QLabel(this);
	location->setFixedSize(1030, height);
	description = new QLabel(this);
	description->setFixedSize(1030, height);
	public_key = new QLabel(this);
	public_key->setFixedSize(1030, height);

	p_total_height = start_position + 6 * height + 10+5;
	setFixedSize(1030, p_total_height);

	//set fonts for all gui items
	creation_date->setFont(font);
	creation_date->move(10, start_position);
	
	start_position -= 5;
	keychain_version->setFont(font);
	keychain_version->move(10, start_position+1*height);
	
	cipher_type->setFont(font);
	cipher_type->move(10, start_position + 2 * height);
	
	location->setFont(font);
	location->move(10, start_position + 3 * height);
	
	description->setFont(font);
	description->move(10, start_position + 4 * height);
	
	public_key->setFont(font);
	public_key->move(10, start_position + 5 * height);

    //create edit button
    QPushButton *EditButton = new QPushButton(this);

    //create edit button text label
    custom_qlabel *EditButtonLabel = new custom_qlabel(EditButton);
    const int row_width = this->size().width() - 50;
    EditButtonLabel->move(row_width, start_position);

    //set styles for edit button and label elements
    EditButtonLabel->setFixedSize(35, 50);
    QPixmap edit_btn_logo(":/keymanager/edit_btn_icon.png");
    edit_btn_logo = edit_btn_logo.scaled(35, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    EditButton->setIcon(edit_btn_logo);
    EditButton->setFlat(true);
    EditButton->setIconSize(QSize(35, 50));
    EditButton->setFixedSize(35, 50);
    EditButton->move(row_width, start_position);

    //create delete button
    QPushButton *DeleteButton = new QPushButton(this);

    //create delete button text label
    custom_qlabel *DeleteButtonLabel = new custom_qlabel(EditButton);

    const int height_step = 40;
    DeleteButtonLabel->move(row_width, height_step);

    //set styles for delete button and label elements
    DeleteButtonLabel->setFixedSize(35, 50);
    QPixmap delete_btn_logo(":/keymanager/delete_btn_icon.png");
    delete_btn_logo = delete_btn_logo.scaled(35, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    DeleteButton->setIcon(delete_btn_logo);
    DeleteButton->setFlat(true);
    DeleteButton->setIconSize(QSize(35, 50));
    DeleteButton->setFixedSize(35, 50);
    DeleteButton->move(row_width, height_step);
}

void more_details::set_details_value() {
	creation_date->setText("Creation date : 12.01.18 16; 44");
	keychain_version->setText("Keychain version : 1.1.256");
	cipher_type->setText("Cipher type : aes 256");
	location->setText("location : keychain storage");
	description->setText("Description : My light wallet key 5");
	public_key->setText("Public key : 4f190a432dcfda0e4ac09dba217eabbe5fdab4a746f2b90c205bb3åbef734d10<br>4f190a432dcfda0e4ac09dba217eabbe5fdab4a746f2b90c205bb3åbef734d10");
}

int more_details::get_total_height()
{
	return p_total_height;
}
