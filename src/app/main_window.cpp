#include "main_window.h"
#include "./ui_main_window.h"
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::showMessage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showMessage()
{
    auto manager = new QNetworkAccessManager();

    QUrl url("http://localhost:2375/info");
    QNetworkRequest request(url);

    QNetworkReply *reply = manager->get(request);

    QObject::connect(reply, &QNetworkReply::finished, [reply]() {
        if (reply->error() == QNetworkReply::NoError) {

            QByteArray response_data = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(response_data);
            QJsonObject jsonObj = jsonDoc.object();
            qDebug().noquote() << "Docker Info:" << jsonObj;
        } else {
            qDebug() << "Error:" << reply->errorString();
        }
        reply->deleteLater();
    });
}
