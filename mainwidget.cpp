#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "QNetworkAccessManager"
#include "QNetworkReply"
#include "QUrl"
#include "QUrlQuery"
#include "QDebug"
#include "QMessageBox"


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
    for (int i = 1; i < 32; ++i) {
        ui->m_daycombobox->addItem(QString::number(i));
    }
    for (int i = 1; i < 13; ++i) {
        ui->m_monthcomboBox->addItem(QString::number(i));
    }
    for (int i = 1960; i < QDate::currentDate().year()+1; ++i) {
        ui->m_yearcomboBox->addItem(QString::number(i));
    }
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
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(RequestResponse(QNetworkReply*)));
    if(ui->m_passwordLineEditRegistration->text()==ui->m_repasswordLineEdit->text() && ui->m_firstNameLineEdit->text()!="" && ui->m_lastNameLineEdit->text()!="" && ui->m_emailRegistrationLineEdit->text()!="" && ui->m_passwordLineEditRegistration->text()!="" && ui->m_repasswordLineEdit->text()!=""){
        QUrlQuery query;
        QUrl url= QUrl("http://127.0.0.1:8000/");
        QByteArray postData;

        query.addQueryItem("first_name", ui->m_firstNameLineEdit->text());
        query.addQueryItem("last_name",ui->m_lastNameLineEdit->text());
        query.addQueryItem("email",ui->m_emailRegistrationLineEdit->text());
        query.addQueryItem("password",ui->m_passwordLineEditRegistration->text());
        if(ui->m_femaleRadioButton->isChecked()){
            query.addQueryItem("sex", QString::number(0));
        }
        else{
            query.addQueryItem("sex", QString::number(1));
        }
        query.addQueryItem("birthDate", QString(ui->m_yearcomboBox->currentText()+"-"+ui->m_monthcomboBox->currentText()+"-"+ui->m_daycombobox->currentText()));
        if(ui->m_isAdminCheckBox->isChecked()){
            query.addQueryItem("is_superuser", QString::number(1));
        }
        else{
            query.addQueryItem("is_superuser", QString::number(0));
        }
        postData=query.toString(QUrl::FullyEncoded).toUtf8();
        QNetworkRequest networkRequest(url);
        //networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        m_networkManager->post(networkRequest, postData);
    }
    else{
        QMessageBox::warning(this,"Unmatched passwords!","Unmatched passwords!");
    }
}

void MainWidget::RequestResponse(QNetworkReply *reply)
{
    QMessageBox::warning(this, "message", QString(reply->readAll()));
    disconnect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(RequestResponse(QNetworkReply*)));
}


void MainWidget::on_m_loginButton_clicked()
{
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(LoginResponse(QNetworkReply*)));
    QUrlQuery query;
    QUrl url= QUrl("http://127.0.0.1:8000/login/");
    QByteArray postData;
    query.addQueryItem("email",ui->m_emailLineEdit->text());
    query.addQueryItem("password",ui->m_passwordLineEdit->text());
    postData=query.toString(QUrl::FullyEncoded).toUtf8();
    QNetworkRequest networkRequest(url);
    m_networkManager->post(networkRequest, postData);
}

void MainWidget::LoginResponse(QNetworkReply *reply)
{
    QMessageBox::warning(this, "message", QString(reply->readAll()));
    disconnect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(LoginResponse(QNetworkReply*)));
}

