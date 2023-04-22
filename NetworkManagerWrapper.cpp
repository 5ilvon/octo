#include "NetworkManagerWrapper.hpp"

#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDateTime>

void NetworkManagerWrapper::Start() {
    auto manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,
            this, &NetworkManagerWrapper::replyFinished);

    qDebug() << "API Requesting";
    manager->get(QNetworkRequest(QUrl("https://www.alphavantage.co/query?function=TIME_SERIES_INTRADAY&symbol=MSFT&interval=1min&apikey=CSZYXE21FDZF1B1B")));
}

void NetworkManagerWrapper::replyFinished(QNetworkReply* reply) {
    QByteArray receivedData = reply->readAll();

    auto data = QJsonDocument::fromJson(receivedData);
    if (!data["Meta Data"].isUndefined()) {
        qDebug() << "Received data for symbol is:" << data["Meta Data"]["2. Symbol"].toString();

        auto timeSequence = data["Time Series (1min)"].toObject();

        // fill vector for timeKeys in seconds from UTC format
        for (const auto& key : timeSequence.keys())
            dataArray.timeKeys.append(QDateTime::fromString(key, Qt::ISODateWithMs).toSecsSinceEpoch());

        for (const auto& timeEntry : timeSequence) {
            dataArray.open.append(timeEntry.toObject()["1. open"].toString().toDouble());
            dataArray.high.append(timeEntry.toObject()["2. high"].toString().toDouble());
            dataArray.low.append(timeEntry.toObject()["3. low"].toString().toDouble());
            dataArray.close.append(timeEntry.toObject()["4. close"].toString().toDouble());
            dataArray.volume.append(timeEntry.toObject()["5. volume"].toString().toDouble());
        }
        emit resultReady(dataArray);
    }

    reply->deleteLater();
}