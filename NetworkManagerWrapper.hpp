#ifndef NETWORKMANAGERWRAPPER_H
#define NETWORKMANAGERWRAPPER_H

#include <QWidget>
#include <QNetworkAccessManager>

class NetworkManagerWrapper : public QWidget {
    Q_OBJECT
public:
    NetworkManagerWrapper();
    ~NetworkManagerWrapper() override = default;

private slots:
    //void onGo();
    static void replyFinished(QNetworkReply* reply);

private:
    QNetworkAccessManager m_manager;
};

#endif //NETWORKMANAGERWRAPPER_H