#include "trade.h"
#include <QDebug>
qint16 Trade::tradeID = 0;

QByteArray Trade::buildOffer()
{
    QJsonObject offerData;

    // set proper type
    offerData["type"] = static_cast<qint16>(codes_t::TRADE_OFFER);

    // data of offered resources
    offerData["offeredWood"] = m_amountOffered.wood;
    offerData["offeredStone"]= m_amountOffered.stone;
    offerData["offeredIron"] = m_amountOffered.iron;
    offerData["offeredFood"] = m_amountOffered.food;

    // data about requested resources
    offerData["requestedWood"] = m_amountRequested.wood;
    offerData["requestedStone"]= m_amountRequested.stone;
    offerData["requestedIron"] = m_amountRequested.iron;
    offerData["requestedFood"] = m_amountRequested.food;

    // data about the sender
    offerData["sender"] = m_senderPlayer->getUsername();

    // trade metadata
    offerData["tradeID"] = m_tradeID;

    QJsonDocument data;
    data.setObject(offerData);
    return data.toBinaryData();
}


qint16 Trade::getTradeID()
{
    return tradeID++;
}

Trade::Trade(Player *sender, Player *reciever, Resources amountOffered, Resources amountRequested, Resources amountGiven, QObject *parent) : QObject(parent)
{
    // set data about the trade
    m_senderPlayer      = sender;
    m_recieverPlayer    = reciever;
    m_amountOffered     = amountOffered;
    m_amountRequested   = amountRequested;
    m_amountGiven       = amountGiven;
    m_tradeID           = getTradeID();
    hasTradeBeenAccepted= false;
    logConsole << "New trade with ID: " + QString::number(m_tradeID) + " has just been created!";
}

void Trade::startTrade()
{
    // send request to reciever
    qDebug() << m_senderPlayer->getUsername();
    emit sendOffer(m_recieverPlayer, buildOffer());
}

void Trade::checkAccept(qint16 tradeID, Resources amountAccepted)
{
    if(tradeID != m_tradeID) return;

    m_amountAccepted = amountAccepted;
    hasTradeBeenAccepted = true;
}

void Trade::executeTrade()
{
    if(isAccepted())
    {
        m_senderPlayer->takeResources(m_amountGiven);
        m_senderPlayer->giveResources(m_amountAccepted);

        m_recieverPlayer->takeResources(m_amountAccepted);
        m_recieverPlayer->giveResources(m_amountGiven);
    }

}
