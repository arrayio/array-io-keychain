#include "RawHashWidget.h"

RawHashWidget::RawHashWidget(Transaction &transaction, QWidget * parent)
	:KeychainWidget(parent)
{
	QMetaObject::connectSlotsByName(this);

	QString valueStyle("font:16px \"Segoe UI\";background:transparent;color:rgb(123,141,167);");
	QString labelStyle("font:16px \"Segoe UI\";background:transparent;");

	secmod_parser_f cmd_parse;
	auto cmd_type = cmd_parse(transaction.getTransactionText().toStdString());

	namespace sm_cmd = keychain_app::secmod_commands;
	std::string tx_hash, tx_from;
	sm_cmd::transaction_view<sm_cmd::blockchain_secmod_te::ethereum>::type eth_trx;
	switch (cmd_type)
	{
		case sm_cmd::events_te::sign_hash:
		{
			auto cmd = cmd_parse.params<sm_cmd::events_te::sign_hash>();
			tx_hash = cmd.hash;
			tx_from = cmd.from;
			break;
		}
	}

	from = new SecureWindowElement(this);
	from->SetLabelAndValue("From", QString::fromStdString(tx_from));
	from->SetLabelStyle(labelStyle);
	from->SetValueStyle(valueStyle);

	QFont fromFont = from->font();
	fromFont.setPixelSize(16);
	fromFont.setFamily("Segoe UI");
	QString fromStr = QString::fromStdString(tx_from);

	QFontMetrics fromFM(fromFont);
	int fromWidth = fromFM.width(fromStr);

	hash = new SecureWindowElement(this);
	hash->SetLabelAndValue("Hash", QString::fromStdString(tx_hash));
	hash->SetLabelStyle(labelStyle);
	hash->SetValueStyle(valueStyle);
	QFont hashFont = hash->font();
	QString hashStr = QString::fromStdString(tx_hash);
	QFontMetrics hashFM(fromFont);
	int hashWidth = hashFM.width(hashStr);
	if (hashWidth >= fromWidth)
		max_width = hashWidth;
	else
		max_width = fromWidth;
/*
	if (cmd_parse.unlock_time() > 0) {
		unlockTime = new PrivateKeyInMemory(this);
		unlockTime->SetTime(QString::number(cmd_parse.unlock_time()));
	}
*/
	//expertModeElement = new ExpertModeElement(this);
	//expertModeElement->SetExpertModeText(QString::fromStdString(cmd_parse.to_expert_mode_string()), false);

}

void RawHashWidget::SetPosition(int x, int y, int width)
{
	if (max_width > width)
		width = max_width;
	from->SetPosition(0, 0, 116, width+50);
	from->move(0, currentHeight);
	currentHeight += 26;
	hash->SetPosition(0, currentHeight, 116, width);
	hash->move(0, currentHeight);
	currentHeight += 26;
	if (unlockTime != Q_NULLPTR) {
		unlockTime->SetPosition(0, currentHeight, 116, width);
		unlockTime->move(0, currentHeight);
		currentHeight += 36;
	}
	//expertModeElement->SetPosition(0, currentHeight, 116, width);
	//expertModeElement->move(0, currentHeight);
	//currentHeight += 60;
	currentWidth = from->width();
	setFixedWidth(currentWidth);
	setFixedHeight(currentHeight);
}

int RawHashWidget::GetCurrentHeight() {
	return currentHeight;
}

int RawHashWidget::GetCurrentWidth() {
	return currentWidth;
}

RawHashWidget::~RawHashWidget()
{

}