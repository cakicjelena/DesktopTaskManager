#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "QNetworkAccessManager"
#include "QNetworkReply"
#include "QUrl"
#include "QUrlQuery"
#include "QDebug"
#include "QMessageBox"
#include "QJsonObject"
#include "QJsonDocument"
#include "usermodel.h"


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
    if(index<0 || index>=4){
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

void MainWidget::initProfile()
{
    ui->m_first_name_label->setText(ui->m_first_name_label->text() + m_user->getFirst_name());
    ui->m_last_name_label->setText(ui->m_last_name_label->text() + m_user->getLast_name());
    //ui->m_passwordLabel->setText(ui->m_passwordLabel->text()+ m_user->getPassword());
    ui->m_email_label->setText(ui->m_email_label->text() + m_user->getEmail());
    if(m_user->getSex()){

        ui->m_sexLabel->setText(ui->m_sexLabel->text() + "M");
    }
    else{
        ui->m_sexLabel->setText(ui->m_sexLabel->text() + "F");
    }
    ui->m_birthDate_label->setText(ui->m_birthDate_label->text() + m_user->getBirthDate().toString());
    if(m_user->getIs_active()){

        ui->m_active_label->setText(ui->m_active_label->text() + "Active");
        ui->m_active_label->setStyleSheet("background-color: rgb(144, 238, 144)");

    }
    else {
        ui->m_active_label->setText(ui->m_active_label->text() + "Inactive");
        ui->m_active_label->setStyleSheet("background-color: rgb(255, 127, 127)");

    }
    if(m_user->getIs_superuser()){
        ui->m_admin_label->setText(ui->m_admin_label->text() + "Admin");
    }
    else{
        ui->m_admin_label->setText(ui->m_admin_label->text() + "Ordinary profile");

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
    QJsonObject obj;
    QJsonDocument doc = QJsonDocument::fromJson(QString(reply->readAll()).toUtf8());

    // check validity of the document
    if(!doc.isNull())
    {
        if(doc.isObject())
        {
            obj = doc.object();
        }
        else
        {
            qDebug() << "Document is not an object" << endl;
        }
    }
    else
    {
        qDebug() << "Invalid JSON...\n" << QString(reply->readAll()) << endl;
    }

   //MessageBox::warning(this, "message", obj.take("first_name
    //qDebug()<<obj.take("first_name");
    m_user= new UserModel(obj);
    initProfile();
    qDebug()<<m_user->getFirst_name();
    selectPage((int)pages::PROFILE);
    disconnect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(LoginResponse(QNetworkReply*)));
}

void MainWidget::LogoutResponse(QNetworkReply *reply)
{
    qDebug()<<reply->readAll();
    disconnect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(LogoutResponse(QNetworkReply*)));
}


void MainWidget::on_m_logout_button_clicked()
{
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(LogoutResponse(QNetworkReply*)));
    QUrlQuery query;
    QUrl url= QUrl("http://127.0.0.1:8000/logout/");
    QByteArray postData;
    postData=query.toString(QUrl::FullyEncoded).toUtf8();
    QNetworkRequest networkRequest(url);
    m_networkManager->post(networkRequest, postData);
}


void MainWidget::on_m_tasks_button_clicked()
{

}


void MainWidget::on_m_editProfilebutton_clicked()
{

}

