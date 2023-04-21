#ifndef NETWORKMANAGERWRAPPER_H
#define NETWORKMANAGERWRAPPER_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QList>
#include <QJsonValue>

struct OHLCData {
    QList<QDateTime> timeSeq;
};

class NetworkManagerWrapper : public QWidget {
    Q_OBJECT
public:
    NetworkManagerWrapper();
    ~NetworkManagerWrapper() override = default;
    QPair<QDateTime, QJsonValue> m_array;

private slots:
    //void onGo();
    void replyFinished(QNetworkReply* reply);

private:
    QNetworkAccessManager m_manager;
};

#endif //NETWORKMANAGERWRAPPER_H