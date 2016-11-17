#include <QEventLoop>
#include "osHttpDevice.h"
#include "../httpClient/httpClient.h"

OsHttpDevice::OsHttpDevice(QUrl _url){
    qDebug("OsHttpDevice Constructor");
    name = "HTTP Device";
    httpClient = new HttpClient(this);
    url = _url;

    connect(httpClient, SIGNAL(complete(QNetworkReply*)), this, SLOT(onHttpComplete(QNetworkReply*)));
}

void OsHttpDevice::execCommand(QString cmd) {    
    qDebug() << "OsHttpDevice::execCommand() - " << name;
    qDebug(cmd.toUtf8());

    //Build HTTP POST request
    QNetworkRequest request = QNetworkRequest(url);
    request.setRawHeader("Content-Type", "application/json");

    QEventLoop loop;
    connect(this, SIGNAL(commandComplete()), &loop, SLOT(quit()));
    httpClient->post(request, cmd);

    //Wait for signal that proxy call has returned


    loop.exec();
    qDebug("OsHttpDevice Done");
}

void OsHttpDevice::onHttpComplete(QNetworkReply *reply) {
    qDebug("OsHttpDevice::onHttpComplete()");
    emit commandComplete();
    emit execCommandComplete(QString(reply->readAll()));
}
