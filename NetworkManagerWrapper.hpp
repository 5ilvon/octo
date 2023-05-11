#ifndef NETWORKMANAGERWRAPPER_H
#define NETWORKMANAGERWRAPPER_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QList>
#include <QJsonValue>
#include <QThread>

struct OHLCData {
    QVector<double> timeKeys, open, high, low, close, volume;
};

class NetworkManagerWrapper : public QObject {
    Q_OBJECT
public:
    NetworkManagerWrapper() = default;
    ~NetworkManagerWrapper() = default;

    void Start();
    OHLCData dataArray;

private slots:
    void requestData(const QString&);
    void replyFinished(QNetworkReply* reply);

private:
    QNetworkAccessManager* m_manager;

signals:
    void resultReady(const OHLCData& result);
};

#endif //NETWORKMANAGERWRAPPER_H