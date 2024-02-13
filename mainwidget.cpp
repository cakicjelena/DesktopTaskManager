#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "QNetworkAccessManager"
#include "QNetworkReply"
#include "QUrl"
#include "QUrlQuery"
#include "QDebug"


MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    initialize();
}

MainWidget::~MainWidget()
{
    delete ui;
    delete m_networkManager;
}

void MainWidget::selectPage(int index)
{
    if(index<0|| index>=3){
        return;
    }
    else{
        ui->stackedWidget->setCurrentIndex(index);
    }
}

void MainWidget::initialize()
{
    m_networkManager=new QNetworkAccessManager(this);
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(RequestResponse(QNetworkReply*)));

}

void MainWidget::on_m_loginButton_2_clicked()
{
    selectPage((int) pages::LOGIN);
}


void MainWidget::on_m_signupButton_clicked()
{
    selectPage((int)pages::REGISTRATION);
}


void MainWidget::on_m_registerButton_clicked()
{
    QUrlQuery query;
    QUrl url= QUrl("http://127.0.0.1:8000/");
    QByteArray postData;

    query.addQueryItem("firstName", ui->m_firstNameLineEdit->text());
    query.addQueryItem("lastName",ui->m_lastNameLineEdit->text());
    query.addQueryItem("email",ui->m_emailRegistrationLineEdit->text());
    query.addQueryItem("password",ui->m_passwordLineEditRegistration->text());
    postData=query.toString(QUrl::FullyEncoded).toUtf8();
    QNetworkRequest networkRequest(url);
    //networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    m_networkManager->post(networkRequest, postData);
}

void MainWidget::RequestResponse(QNetworkReply *reply)
{
    qDebug()<<reply->readAll();
}

