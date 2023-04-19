#include "NetworkManagerWrapper.hpp"

#include <QNetworkReply>

NetworkManagerWrapper::NetworkManagerWrapper() {
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,
            this, &NetworkManagerWrapper::replyFinished);

    qDebug() << "API Requesting";
    manager->get(QNetworkRequest(QUrl("https://www.alphavantage.co/query?function=TIME_SERIES_INTRADAY&symbol=MSFT&interval=1min&apikey=CSZYXE21FDZF1B1B")));
}

void NetworkManagerWrapper::replyFinished(QNetworkReply* reply) {
    QByteArray a = reply->readAll();
    qDebug() << a;
    reply->deleteLater();
    exit(0);
}