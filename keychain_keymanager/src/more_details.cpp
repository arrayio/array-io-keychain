#include "more_button.hpp"
#include "more_details.hpp"

more_details::more_details(QWidget *parent)
	:QWidget(parent)
{
	QMetaObject::connectSlotsByName(this);
	QFont font("Cartograph Sans CF Regular");
	font.setPixelSize(14);
	setStyleSheet("background-color:rgb(214,220,229);");
	//initialize gui items
	int start_position = 0;
	int height = 30;
	//edit_button = new more_button(this);
	//edit_button->set_edit_mode();
	//edit_button->move(980, 10);
	//rem_button = new more_button(this);
	//rem_button->set_remove_mode();
	//rem_button->move(980, 55);

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
    EditButton->setObjectName("editbtn");
    //EditButton->setStyleSheet("QPushButton#editbtn:focus{border:none;outline:none;}");

    //create edit button text label
    custom_qlabel *EditButtonLabel = new custom_qlabel(EditButton);
    const int rowWidth = this->size().width() - 50;
    
    EditButtonLabel->move(rowWidth, start_position);

    //set styles for edit button and label elements
    EditButtonLabel->setFixedSize(30, 50);
    QPixmap editBtnLogo(":/keymanager/edit_icon.png");
    editBtnLogo = editBtnLogo.scaled(18, 25, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    EditButton->setIcon(editBtnLogo);
    EditButton->setFlat(true);
    EditButton->setIconSize(QSize(18, 25));
    EditButton->setFixedSize(22, 27);
    EditButton->move(rowWidth, start_position);

    //create delete button
    QPushButton *DeleteButton = new QPushButton(this);
    DeleteButton->setObjectName("deletebtn");
    //DeleteButton->setStyleSheet("QPushButton#deletebtn:focus{border:none;outline:none;}");

    //create delete button text label
    custom_qlabel *DeleteButtonLabel = new custom_qlabel(DeleteButton);

    const int heightStep = 40;
    DeleteButtonLabel->move(rowWidth, heightStep);

    //set styles for delete button and label elements
    DeleteButtonLabel->setFixedSize(18, 25);
    QPixmap deleteBtnLogo(":/keymanager/trash_icon.png");
    deleteBtnLogo = deleteBtnLogo.scaled(35, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    DeleteButton->setIcon(deleteBtnLogo);
    DeleteButton->setFlat(true);
    DeleteButton->setIconSize(QSize(18, 25));
    DeleteButton->setFixedSize(22, 27);
    DeleteButton->move(rowWidth, heightStep);

    //connect clicked trigger for delete keys button via qt signals with remove dialog
    QObject::connect(DeleteButton, SIGNAL(clicked()), this, SLOT(RemoveKey()));
}

void more_details::set_details_value(keylist_row_model keylist_row_data) {
	creation_date->setText("Creation date : "+ keylist_row_data.creation_date);
	keychain_version->setText("Keychain version : "+keylist_row_data.keychain_version);
	cipher_type->setText("Cipher type : aes256");
	location->setText("Location : "+keylist_row_data.location);
	description->setText("Description : " + keylist_row_data.description);
	public_key->setText("Public key : " + keylist_row_data.public_key);
}

int more_details::get_total_height()
{
	return p_total_height;
}

//call remove key dialog
void more_details::RemoveKey()
{
    emit RemoveKeySelected("Remove key was clicked!");
}
