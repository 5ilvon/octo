#include "NetworkManagerWrapper.hpp"

#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


NetworkManagerWrapper::NetworkManagerWrapper() {
    auto manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,
            this, &NetworkManagerWrapper::replyFinished);

    qDebug() << "API Requesting";
    manager->get(QNetworkRequest(QUrl("https://www.alphavantage.co/query?function=TIME_SERIES_INTRADAY&symbol=MSFT&interval=1min&apikey=CSZYXE21FDZF1B1B")));
}

void NetworkManagerWrapper::replyFinished(QNetworkReply* reply) {
    QByteArray a = reply->readAll();

    auto data = QJsonDocument::fromJson(a);
    if (!data["Meta Data"].isUndefined()) {
        qDebug() << "Requested symbol is:" << data["Meta Data"]["2. Symbol"].toString();

        auto timeSequence = data["Time Series (1min)"].toObject();
        qDebug() << timeSequence.keys();

        for (const auto& timeEntry : timeSequence) {
            qDebug() << timeEntry;
            //m_array.append(timeEntry);
            //qDebug() << m_array[0];
        }
    }

    reply->deleteLater();
    //exit(0);
}

//void NetworkManagerWrapper::stocksDataProc() {
//}