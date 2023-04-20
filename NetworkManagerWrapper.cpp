#include "NetworkManagerWrapper.hpp"

#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>

NetworkManagerWrapper::NetworkManagerWrapper() {
    auto manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,
            this, &NetworkManagerWrapper::replyFinished);

    qDebug() << "API Requesting";
    manager->get(QNetworkRequest(QUrl("https://www.alphavantage.co/query?function=TIME_SERIES_INTRADAY&symbol=MSFT&interval=1min&apikey=CSZYXE21FDZF1B1B")));
}

void NetworkManagerWrapper::replyFinished(QNetworkReply* reply) {
    QByteArray a = reply->readAll();

    QJsonDocument loadData(QJsonDocument::fromJson(a));
    qDebug() << "Requested symbol is:" << loadData["Meta Data"]["2. Symbol"].toString();

    reply->deleteLater();
    //exit(0);
}