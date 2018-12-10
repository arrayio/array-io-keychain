#include "EthereumSwapWidget.h"

EthereumSwapWidget::EthereumSwapWidget(Transaction &transaction, QWidget * parent)
	:KeychainWidget(parent)
{
	QMetaObject::connectSlotsByName(this);

	swap = new SecureWindowElement(this);
	QString valueStyle("font:16px \"Segoe UI\";background:transparent;color:rgb(123,141,167);");
	QString labelStyle("font:16px \"Segoe UI\";background:transparent;");
	swap->SetLabelStyle("background-image:url(:/keychain_gui_win/swap_logo.png) no-repeat;");
	swap->SetValueStyle("font:14px \"Segoe UI\";background:transparent;color:rgb(123,141,167);");

	//QList<QString> fieldList({ "From","To","Amount" });

	secmod_parser_f cmd_parse;
	auto cmd_type = cmd_parse(transaction.getTransactionText().toStdString());
	auto swap_trx = cmd_parse.to_ethereum_swap();
	auto swap_info = swap_trx.swap_info;

	action = new SecureWindowElement(this);
	action->SetLabelStyle(labelStyle);
	action->SetValueStyle(valueStyle);
	switch (swap_info.action)
	{
		
		//TODO: need impleentation
	}
	action->SetLabelAndValue("Action", "(action)");

	/*create_swap = 0,
		refund,
		withdraw*/

	hash = new SecureWindowElement(this);
	hash->SetLabelStyle(labelStyle);
	hash->SetValueStyle(valueStyle);
	hash->SetLabelAndValue("Hash", QString::fromStdString(swap_info.hash));

	address = new SecureWindowElement(this);
	address->SetLabelStyle(labelStyle);
	address->SetValueStyle(valueStyle);
	address->SetLabelAndValue("Address", QString::fromStdString(swap_info.address));

	cryptoType = new SecureWindowElement(this);
	cryptoType->SetLabelStyle("background-image:url(:/keychain_gui_win/bg_ephir.png) no-repeat;");
	cryptoType->SetValueStyle(valueStyle);
	cryptoType->SetLabelAndValue("empty=ethereum99");

	//QList<QString> fieldList({ "From","To","Amount" });

	from = new SecureWindowElement(this);
	from->SetLabelStyle(labelStyle);
	from->SetValueStyle(valueStyle);
	from->SetLabelAndValue("From", QString::fromStdString(swap_trx.from));
	
	auto eth_data = swap_trx.trx_info;
	
	to = new SecureWindowElement(this);
	to->SetLabelStyle(labelStyle);
	to->SetValueStyle(valueStyle);
	to->SetLabelAndValue("To", QString::fromStdString(eth_data.to));

	amount = new SecureWindowElement(this);
	amount->SetLabelStyle(labelStyle);
	amount->SetValueStyle(valueStyle);
	amount->SetLabelAndValue("Amount", QString::fromStdString(eth_data.value));

	expertModeElement = new ExpertModeElement(this);
	expertModeElement->SetExpertModeText(QString::fromStdString(cmd_parse.to_expert_mode_string()));

}

void EthereumSwapWidget::SetPosition(int x, int y, int width)
{
	swap->SetPosition(0, 0, 116, width);
	swap->SetLabelOffset(72);
	currentHeight = 26;
	action->SetPosition(0, currentHeight, 116, width);
	action->move(0, currentHeight);
	currentHeight += 26;
	hash->SetPosition(0, currentHeight, 116, width);
	hash->move(0, currentHeight);
	currentHeight += 26;
	address->SetPosition(0, currentHeight, 116, width);
	address->move(0, currentHeight);
	currentHeight += 26;
	cryptoType->SetPosition(0, currentHeight, 116, width);
	cryptoType->move(0, currentHeight);
	cryptoType->SetLabelOffset(100);
	currentHeight += 26;
	from->SetPosition(0, currentHeight, 116, width);
	from->move(0, currentHeight);
	currentHeight += 26;
	to->SetPosition(0, currentHeight, 116, width);
	to->move(0, currentHeight);
	currentHeight += 26;
	amount->SetPosition(0, currentHeight, 116, width);
	amount->move(0, currentHeight);
	currentHeight += 26;
	expertModeElement->SetPosition(0, currentHeight, 116, width);
	expertModeElement->move(0, currentHeight);
	currentHeight += 60;
	currentWidth = from->width();
	setFixedWidth(currentWidth);
	setFixedHeight(currentHeight);
}

int EthereumSwapWidget::GetCurrentHeight() {
	return currentHeight;
}

int EthereumSwapWidget::GetCurrentWidth() {
	return currentWidth;
}

EthereumSwapWidget::~EthereumSwapWidget()
{

}