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
#include "QNetworkCookieJar"
#include "QJsonArray"
#include "projectmodel.h"
#include "json.h"
#include "tmfactory.h"
#include "taskmodel.h"


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
    delete m_tmFactory;
}

void MainWidget::selectPage(int index)
{
    if(index<0 || index>=9){
        return;
    }
    else{
        ui->stackedWidget->setCurrentIndex(index);
    }
}

void MainWidget::initialize()
{
    m_networkManager=new QNetworkAccessManager(this);

    m_tmFactory= new TMFactory();
    getAllUsers();

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

void MainWidget::clearProfile()
{
    ui->m_first_name_label->setText("First name: ");
    ui->m_last_name_label->setText("Last name: ");
    ui->m_email_label->setText("Email: ");
    ui->m_sexLabel->setText("Sex: ");
    ui->m_active_label->setText("Active: ");
    ui->m_birthDate_label->setText("Birthdate: ");
    ui->m_admin_label->setText("Admin: ");
}

void MainWidget::taskDetails(int id)
{
    TaskModel* t= m_tmFactory->getTaskById(id);
    ui->m_taskdetailsNameLineEdit->setText(t->getName());
    ui->m_taskdetailsTypeLineEdit->setText(QString::number(t->getType()));
    ui->m_taskdetailsDescriptionTextEdit->setPlainText(t->getDescription());
    ui->m_taskdetailsStatusLineEdit->setText(QString::number(t->getStatus()));
    ui->m_taskdetailsStartDateLabel->setText(ui->m_taskdetailsStartDateLabel->text() + t->getStartDate().toString());
    ui->m_taskdetailsFinishdateEdit->setDate(t->getFinishDate());
    ProjectModel* p= m_tmFactory->getProjectById(t->getProjectId());
    if(p){
        ui->m_taskdetailsProjectlineEdit->setText(p->getName());
    }
    UserModel* u= m_tmFactory->getUserById(t->getUserId());
    if(u){
        ui->m_taskdetailsUserlineEdit->setText(u->getEmail());
    }
    selectPage((int)pages::TASKDETAILS);

}

void MainWidget::getAllUsers()
{
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getAllUsersResponse(QNetworkReply*)));
    QUrl url= QUrl("http://127.0.0.1:8000/getallusers/");
    //m_networkManager->setCookieJar(new QNetworkCookieJar(m_networkManager));
    QNetworkRequest networkRequest(url);
    m_networkManager->get(networkRequest);
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
    selectPage((int)pages::LOGIN);
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
    clearProfile();
    selectPage((int)pages::MAIN);
    disconnect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(LogoutResponse(QNetworkReply*)));
}


void MainWidget::on_m_logout_button_clicked()
{
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(LogoutResponse(QNetworkReply*)));
    QUrlQuery query;
    QUrl url= QUrl("http://127.0.0.1:8000/logout/");
    m_networkManager->setCookieJar(new QNetworkCookieJar(m_networkManager));
    QByteArray postData;
    postData=query.toString(QUrl::FullyEncoded).toUtf8();
    QNetworkRequest networkRequest(url);
    m_networkManager->post(networkRequest, postData);
}

void MainWidget::on_m_editProfilebutton_clicked()
{
    selectPage((int)pages::EDITPROFILE);
}


void MainWidget::on_m_submitbutton_clicked()
{
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(EditProfileResponse(QNetworkReply*)));
    QUrlQuery query;
    QUrl url= QUrl("http://127.0.0.1:8000/editprofile/"+QString::number(m_user->getId()));
    QByteArray postData;

    m_networkManager->setCookieJar(new QNetworkCookieJar(m_networkManager));
    if(ui->m_editpasswordlineedit->text()==ui->m_editpasswordconfirmlineedit->text()){

    query.addQueryItem("first_name", ui->m_editfirstnamelineedit->text());
    query.addQueryItem("last_name",ui->m_editlastnamelineedit->text());
    query.addQueryItem("email",ui->m_editemaillineedit->text());
    query.addQueryItem("password",ui->m_editpasswordlineedit->text());
    postData=query.toString(QUrl::FullyEncoded).toUtf8();
    QNetworkRequest networkRequest(url);
    m_networkManager->post(networkRequest, postData);
    }
    else{
        QMessageBox::warning(this,"Unmatched passwords!","Unmatched passwords!");
    }

}

void MainWidget::EditProfileResponse(QNetworkReply *reply)
{
    qDebug()<<reply->readAll();
    if(ui->m_editfirstnamelineedit->text()!=""){
        m_user->setFirst_name(ui->m_editfirstnamelineedit->text());
    }
    if(ui->m_editlastnamelineedit->text()!=""){
        m_user->setLast_name(ui->m_editlastnamelineedit->text());
    }
    if(ui->m_editemaillineedit->text()!=""){
        m_user->setEmail(ui->m_editemaillineedit->text());
    }
    if(ui->m_editpasswordlineedit->text()!=""){
        m_user->setPassword(ui->m_editpasswordlineedit->text());
    }
    clearProfile();
    initProfile();
    selectPage((int)pages::PROFILE);
    disconnect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(EditProfileResponse(QNetworkReply*)));
}

void MainWidget::ProjectResponse(QNetworkReply *reply)
{
    QString response  = QString(reply->readAll());
    qDebug()<<response;
    QJsonArray obj;

    QJsonDocument doc= QJsonDocument::fromJson(response.toUtf8());
    obj = doc.array();
    // check validity of the document

        for (int i = 0; i < obj.size(); ++i) {
            ProjectModel* p = m_tmFactory->createProject(obj[i].toObject());
            qDebug()<<p->getName();
            m_tmFactory->addProject(p);
            QListWidgetItem* item = new QListWidgetItem(QString::number(p->getId())+":"+p->getName());
            ui->m_projectslistWidget->addItem(item);
            }

    selectPage((int)pages::PROJECT);
            disconnect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(ProjectResponse(QNetworkReply*)));
}


void MainWidget::on_m_projects_button_clicked()
{

    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(ProjectResponse(QNetworkReply*)));
    QUrl url= QUrl("http://127.0.0.1:8000/getallprojects/");
    //m_networkManager->setCookieJar(new QNetworkCookieJar(m_networkManager));
    QNetworkRequest networkRequest(url);
    m_networkManager->get(networkRequest);

}


void MainWidget::on_m_projectslistWidget_itemClicked(QListWidgetItem *item)
{
    QStringList projectId = item->text().split(":");
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(TaskResponse(QNetworkReply*)));
    QUrl url= QUrl("http://127.0.0.1:8000/getalltasksofproject/"+projectId[0]);
    //m_networkManager->setCookieJar(new QNetworkCookieJar(m_networkManager));
    QNetworkRequest networkRequest(url);
    m_networkManager->get(networkRequest);
}

void MainWidget::TaskResponse(QNetworkReply *reply)
{
    QString response  = QString(reply->readAll());
    qDebug()<<response;
    QJsonArray obj;

    QJsonDocument doc= QJsonDocument::fromJson(response.toUtf8());
    obj = doc.array();
    // check validity of the document

    for (int i = 0; i < obj.size(); ++i) {
        TaskModel* t = m_tmFactory->createTask(obj[i].toObject());
        qDebug()<<t->getName();
        m_tmFactory->addTask(t);
        QListWidgetItem* item = new QListWidgetItem(QString::number(t->getId())+":"+t->getName());
        ui->m_tasklistWidget->addItem(item);
    }

    selectPage((int)pages::TASK);
    disconnect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(TaskResponse(QNetworkReply*)));

}

void MainWidget::getAllUsersResponse(QNetworkReply *reply)
{
    QString response  = QString(reply->readAll());
    qDebug()<<response;
    QJsonArray obj;

    QJsonDocument doc= QJsonDocument::fromJson(response.toUtf8());
    obj = doc.array();
    // check validity of the document

    for (int i = 0; i < obj.size(); ++i) {
        UserModel* u = m_tmFactory->createUser(obj[i].toObject());
        //qDebug()<<u->getName();
        m_tmFactory->addUser(u);
    }

    //selectPage((int)pages::TASK);
    disconnect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getAllUsersResponse(QNetworkReply*)));

}




void MainWidget::on_m_editprojectbutton_clicked()
{

}


void MainWidget::on_m_deleteprojectbutton_clicked()
{

}


void MainWidget::on_m_createprojectbutton_clicked()
{
    selectPage((int)pages::CREATEPROJECT);
}


void MainWidget::on_m_deletetaskbutton_clicked()
{

}


void MainWidget::on_m_edittaskbutton_clicked()
{

}


void MainWidget::on_m_createtaskbutton_clicked()
{

}


void MainWidget::on_m_tasklistdoneWidget_itemClicked(QListWidgetItem *item)
{
    QStringList taskId = item->text().split(":");
    QString id= taskId[0];
    taskDetails(id.toInt());

}


void MainWidget::on_m_tasklistinprogresWidget_itemClicked(QListWidgetItem *item)
{
    QStringList taskId = item->text().split(":");
    QString id= taskId[0];
    taskDetails(id.toInt());
}


void MainWidget::on_m_tasklistWidget_itemClicked(QListWidgetItem *item)
{
    QStringList taskId = item->text().split(":");
    QString id= taskId[0];
    taskDetails(id.toInt());
}

