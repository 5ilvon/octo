#ifndef NETWORKMANAGERWRAPPER_H
#define NETWORKMANAGERWRAPPER_H

#include <QWidget>
#include <QNetworkAccessManager>

class NetworkManagerWrapper : public QWidget {
    Q_OBJECT
    public:
    NetworkManagerWrapper();
    ~NetworkManagerWrapper() = default;

    private slots:
    //void onGo();
    void replyFinished(QNetworkReply* reply);

    private:
    QNetworkAccessManager m_manager;
};

#endif //NETWORKMANAGERWRAPPER_H