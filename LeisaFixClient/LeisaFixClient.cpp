// LeisaFixClient.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "LeisaFixClient.h"
using namespace std;
using namespace FIX;
void LeisaFixClient::onCreate(const SessionID&) 
{
	cout << "onCreate" << endl;
}
void LeisaFixClient::onLogon(const SessionID&)
{
	cout << "onLogon" << endl;
}
void LeisaFixClient::onLogout(const SessionID&)
{
	cout << "onLogout" << endl;
}
void LeisaFixClient::onMessage(const Message&, const SessionID&) 
{
	cout << "onMessage" << endl;
}
void LeisaFixClient::run() 
{
	while (true)
    {
		try
        {
			char action = queryAction();
			switch (action)
			{
			case'1':
				queryEnterOrder();
				break;
			case '2':
				queryCancelOrder();
				break;
			case '3':
				queryReplaceOrder();
				break;
			case'4':
				queryMarketDataRequest();
				break;
			case'5':
				break;
			default:
				break;
			}
		}
		catch (exception& e) 
        {
			cout << e.what() << endl;
		}
	}
}
void LeisaFixClient::queryEnterOrder() 
{
	Message order=queryNewOrderSingle();
	if (queryConfirm("send order"))
		Session::sendToTarget(order);
}
void LeisaFixClient::queryCancelOrder() 
{
	Message cancel = queryOrderCancelRequest();
	if (queryConfirm("send cancel"))
		Session::sendToTarget(cancel);
}
void LeisaFixClient::queryReplaceOrder() 
{
	Message replace = queryCancelReplaceRequest();
	if (queryConfirm("send replace"))
		Session::sendToTarget(replace);
}
void LeisaFixClient::queryMarketDataRequest() 
{
	Message marketData = queryMarketDataRequest44();
	Session::sendToTarget(marketData);
}
FIX44::NewOrderSingle LeisaFixClient::queryNewOrderSingle() 
{
	OrdType ordType;
	FIX44::NewOrderSingle newOrderSingle(queryClOrdID(), querySide(),TransactTime(),ordType=queryOrdType() );
	newOrderSingle.set(HandlInst('1'));
	newOrderSingle.set(querySymbol());
	newOrderSingle.set(queryOrderQty());
	newOrderSingle.set(queryTimeInForce());
	if (ordType == OrdType_LIMIT || ordType == OrdType_STOP_LIMIT)
	{
		newOrderSingle.set(queryPrice());
	}
	if (ordType == OrdType_STOP || ordType == OrdType_STOP_LIMIT)
    {
		newOrderSingle.set(queryStopPx());
	}
	queryHeader(newOrderSingle.getHeader());
	return newOrderSingle;
}
FIX44::OrderCancelRequest LeisaFixClient::queryOrderCancelRequest() 
{
	FIX44::OrderCancelRequest orderCancelRequest(queryOrigClOrdID(), queryClOrdID(), querySide(), TransactTime());
	orderCancelRequest.set(querySymbol());
	orderCancelRequest.set(queryOrderQty());
	queryHeader(orderCancelRequest.getHeader());
	return orderCancelRequest;
}
FIX44::OrderCancelReplaceRequest LeisaFixClient::queryCancelReplaceRequest() 
{
	FIX44::OrderCancelReplaceRequest cancelReplaceRequest(
		queryOrigClOrdID(), queryClOrdID(), querySide(), TransactTime(), queryOrdType());
	cancelReplaceRequest.set(HandlInst('1'));
	cancelReplaceRequest.set(querySymbol());
	if (queryConfirm("new price")) 
    {
		cancelReplaceRequest.set(queryPrice());
	}
	if (queryConfirm("new quantity"))
	{
		cancelReplaceRequest.set(queryOrderQty());
	}
	queryHeader(cancelReplaceRequest.getHeader());
	return cancelReplaceRequest;
}
FIX44::MarketDataRequest LeisaFixClient::queryMarketDataRequest44() 
{
	MDReqID mdReqID("MARKETDATAID");
	SubscriptionRequestType subType(SubscriptionRequestType_SNAPSHOT);
	MarketDepth marketDepth(0);
	FIX44::MarketDataRequest::NoMDEntryTypes marketDataEntryGroup;
	MDEntryType mdEntryType(MDEntryType_BID);
	marketDataEntryGroup.set(mdEntryType);
	FIX44::MarketDataRequest::NoRelatedSym symbolGroup;
	Symbol fixSymbol("LNUX");
	symbolGroup.set(fixSymbol);
	FIX44::MarketDataRequest message(mdReqID, subType, marketDepth);
    message.addGroup(marketDataEntryGroup);
    message.addGroup(symbolGroup);
    queryHeader(message.getHeader());
    cout << message.toXML() << endl;
    cout << message.toString << endl;
    return message;
}
void LeisaFixClient::queryHeader(Header& head)
{
    head.setField(querySenderCompID());
    head.setField(queryTargetCompID());
    if (queryConfirm("use a targetSubID"))
    {
        head.setField(queryTargetSubID());
    }
}
char LeisaFixClient::queryAction() 
{
    char value;
    cout << "1)Enter Order" << endl
        << "2)Cancel Order" << endl
        << "3)Replace Order" << endl
        << "4)Market data test" << endl
        << "5)Quit" << endl
        << "Action:";
    cin >> value;
    switch (value) 
    {
    case'1':
    case'2':
    case'3':
    case'4':
    case'5':
        break;
    default:
        throw exception();
    }
    return value;
}
int LeisaFixClient::queryVersion() 
{
    return 44;
}
bool LeisaFixClient::queryConfirm(const string& query) 
{
    string value;
    cout << endl << query << "?:";
    cin >> value;
    return toupper(*value.c_str()) == 'Y';
}
SenderCompID LeisaFixClient::querySenderCompID()
{
    return SenderCompID("LeisaClient");
}
TargetCompID LeisaFixClient::queryTargetCompID()
{
    return TargetCompID("LeisaServer");
}
TargetSubID LeisaFixClient::queryTargetSubID() 
{
    return TargetSubID("");
}
ClOrdID LeisaFixClient::queryClOrdID()
{
    string value;
    cout << endl << "ClOrdID:";
    cin >> value;
    return ClOrdID(value);
}
OrigClOrdID LeisaFixClient::queryOrigClOrdID() 
{
    string value;
    cout << endl << "OrigClOrgID:";
    cin >> value;
    return OrigClOrdID(value);
}
Symbol LeisaFixClient::querySymbol() 
{
    string value;
    cout << endl << "Symbol:";
    cin >> value;
    return Symbol(value);
}
Side LeisaFixClient::querySide()
{
    char value;
    std::cout << std::endl
        << "1) Buy" << std::endl
        << "2) Sell" << std::endl
        << "3) Sell Short" << std::endl
        << "4) Sell Short Exempt" << std::endl
        << "5) Cross" << std::endl
        << "6) Cross Short" << std::endl
        << "7) Cross Short Exempt" << std::endl
        << "Side: ";
    std::cin >> value;
    switch (value)
    {
    case'1':
        return Side(Side_BUY);
    case'2':
        return Side(Side_SELL);
    case'3':
        return Side(Side_SELL_SHORT);
    case'4':
        return Side(Side_SELL_SHORT_EXEMPT);
    case'5':
        return Side(Side_CROSS);
    case'6':
        return Side(Side_CROSS_SHORT);
    case'7':
        return Side('A');
    default:
        throw exception();
    }
}
OrderQty LeisaFixClient::queryOrderQty()
{
    long value;
    cout << endl << "OrderQty:";
    cin >> value;
    return OrderQty(value);
}
OrdType LeisaFixClient::queryOrdType() 
{
    char value;
    std::cout << std::endl
        << "1) Market" << std::endl
        << "2) Limit" << std::endl
        << "3) Stop" << std::endl
        << "4) Stop Limit" << std::endl
        << "OrdType: ";
    cin >> value;
    switch (value)
    {
    case'1':
        return OrdType(OrdType_MARKET);
    case'2':
        return OrdType(OrdType_LIMIT);
    case'3':
        return OrdType(OrdType_STOP);
    case'4':
        return OrdType(OrdType_STOP_LIMIT);
    default:
        throw exception();
    }
}
Price LeisaFixClient::queryPrice() 
{
    double price;
    cout << endl << "Price:";
    cin >> price;
    return Price(price);
}
StopPx LeisaFixClient::queryStopPx() 
{
    double value;
    cout << endl << "StopPx:";
    cin >> value;
    return StopPx(value);
}
TimeInForce LeisaFixClient::queryTimeInForce() 
{
    char value;
    std::cout << std::endl
        << "1) Day" << std::endl
        << "2) IOC" << std::endl
        << "3) OPG" << std::endl
        << "4) GTC" << std::endl
        << "5) GTX" << std::endl
        << "TimeInForce: ";
    cin >> value;
    switch (value)
    {
    case'1':
        return TimeInForce(TimeInForce_DAY);
    case'2':
        return TimeInForce(TimeInForce_IMMEDIATE_OR_CANCEL);
    case'3':
        return TimeInForce(TimeInForce_AT_THE_OPENING);
    case'4':
        return TimeInForce(TimeInForce_GOOD_TILL_CANCEL);
    case'5':
        return TimeInForce(TimeInForce_GOOD_TILL_CROSSING);
    default:
        throw exception();
    }
}
int main()
{
	string file = "fixclient.config";
	Initiator* initiator = 0;
	try 
    {
		SessionSettings settings(file);
		LeisaFixClient application;
		FileStoreFactory storeFactory(settings);
		ScreenLogFactory logFactory(settings);
		initiator = new SocketInitiator(application, storeFactory, settings, logFactory);
		initiator->start();
		application.run();
		initiator->stop();
		delete initiator;
		return 0;
	}
	catch(exception& e)
    {
		cout << e.what() << endl;
		delete initiator;
		return 1;
	}
}

