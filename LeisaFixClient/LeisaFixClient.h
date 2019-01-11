#pragma once
#ifndef LEISAFIXCLIENT_H
#define LEISAFIXCLIENT_H
#include "pch.h"
#pragma comment(lib,"quickfix/quickfix.lib")
class LeisaFixClient :
	public Application,
	public MessageCracker {
public:
	void run();
private:
	void onCreate(const SessionID&);
	void onLogon(const SessionID&);
	void onLogout(const SessionID&);
	void toAdmin(Message&, const SessionID&);
	void toApp(Message&, const SessionID&) throw(DoNotSend);
	void fromAdmin(const Message&, const SessionID&)
		throw(FieldNotFound, IncorrectDataFormat, IncorrectTagValue, RejectLogon);
	void fromApp(const Message& message, const SessionID& sessionID)
		throw(FieldNotFound, IncorrectDataFormat, IncorrectTagValue, UnsupportedMessageType);
	void onMessage(const Message&, const SessionID&);
	void queryEnterOrder();
	void queryCancelOrder();
	void queryReplaceOrder();
	void queryMarketDataRequest();
	FIX44::NewOrderSingle queryNewOrderSingle();
	FIX44::OrderCancelRequest queryOrderCancelRequest();
	FIX44::OrderCancelReplaceRequest queryCancelReplaceRequest();
	FIX44::MarketDataRequest queryMarketDataRequest44();
	void queryHeader(Header& header);
	char queryAction();
	int queryVersion();
	bool queryConfirm(const std::string& query);
	SenderCompID querySenderCompID();
	TargetCompID queryTargetCompID();
	TargetSubID queryTargetSubID();
	ClOrdID queryClOrdID();
	OrigClOrdID queryOrigClOrdID();
	Symbol querySymbol();
	Side querySide();
	OrderQty queryOrderQty();
	OrdType queryOrdType();
	Price queryPrice();
	StopPx queryStopPx();
	TimeInForce queryTimeInForce();
};
#endif