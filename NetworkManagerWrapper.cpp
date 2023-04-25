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
    // 1 hour scale breaks
    manager->get(QNetworkRequest(QUrl("https://api.polygon.io/v2/aggs/ticker/AAPL/range/1/hour/2023-01-01/2023-04-25?adjusted=true&sort=asc&limit=5000&apiKey=0vT2L9jHNnSfaQtaksA_KV51ijYg5Tun")));
}

void NetworkManagerWrapper::replyFinished(QNetworkReply* reply) {
    QByteArray receivedData = reply->readAll();

    auto data = QJsonDocument::fromJson(receivedData);

    if (!data["results"].toArray().isEmpty()) {
        qDebug() << "Received data for symbol is:" << data["ticker"].toString();

        for (const auto& timeEntry : data["results"].toArray()) {
            dataArray.timeKeys.append(timeEntry.toObject()["t"].toDouble() / 1000); // from milliseconds to seconds
            dataArray.open.append(timeEntry.toObject()["o"].toDouble());
            dataArray.high.append(timeEntry.toObject()["h"].toDouble());
            dataArray.low.append(timeEntry.toObject()["l"].toDouble());
            dataArray.close.append(timeEntry.toObject()["c"].toDouble());
            dataArray.volume.append(timeEntry.toObject()["v"].toDouble());
        }
        qDebug() << "values number:" << dataArray.close.size();
        emit resultReady(dataArray);
    }

    reply->deleteLater();
}