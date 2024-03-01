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
#include "tmfactory.h"
#include "taskmodel.h"
#include "session.h"
#include "commentmodel.h"
#include "qtablewidget.h"
#include "tasklist.h"
#include "QNetworkCookie"

int tStatus=-1;



QByteArray MainWidget::getCsrftoken() const
{
    return csrftoken;
}

void MainWidget::setCsrftoken(const QByteArray &newCsrftoken)
{
    csrftoken = newCsrftoken;
}

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
    delete m_listDone;
    delete m_listInProgress;
    delete m_listToDo;
}

void MainWidget::selectPage(int index)
{
    if(index<0 || index>=12){
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


    m_listToDo= new TaskList(this, taskStatus::TODO);
    m_listInProgress= new TaskList(this, taskStatus::INPROGRESS);
    m_listDone=new TaskList(this, taskStatus::FINISHED);

    connect(m_listToDo, SIGNAL(customItemClicked(QListWidgetItem*)),this, SLOT(itemClicked(QListWidgetItem*)));
    connect(m_listToDo, SIGNAL(customItemDoubleClicked(QListWidgetItem*)), this, SLOT(itemDoubleClicked(QListWidgetItem*)));
    connect(m_listToDo, SIGNAL(changeStatus(int)), this, SLOT(changeStatusSlot(int)));
    connect(m_listInProgress, SIGNAL(customItemClicked(QListWidgetItem*)),this, SLOT(itemClicked(QListWidgetItem*)));
    connect(m_listInProgress, SIGNAL(customItemDoubleClicked(QListWidgetItem*)), this, SLOT(itemDoubleClicked(QListWidgetItem*)));
    connect(m_listInProgress, SIGNAL(changeStatus(int)), this, SLOT(changeStatusSlot(int)));
    connect(m_listDone, SIGNAL(customItemClicked(QListWidgetItem*)),this, SLOT(itemClicked(QListWidgetItem*)));
    connect(m_listDone, SIGNAL(customItemDoubleClicked(QListWidgetItem*)), this, SLOT(itemDoubleClicked(QListWidgetItem*)));
    connect(m_listDone, SIGNAL(changeStatus(int)), this, SLOT(changeStatusSlot(int)));

    ui->horizontalLayout_19->addWidget(m_listToDo);
    ui->horizontalLayout_19->addWidget(m_listInProgress);
    ui->horizontalLayout_19->addWidget(m_listDone);

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
    ui->m_first_name_label->setText(ui->m_first_name_label->text() + Session::getInstance()->user()->getFirst_name());
    ui->m_last_name_label->setText(ui->m_last_name_label->text() + Session::getInstance()->user()->getLast_name());
    ui->m_email_label->setText(ui->m_email_label->text() + Session::getInstance()->user()->getEmail());
    if(Session::getInstance()->user()->getSex()){

        ui->m_sexLabel->setText(ui->m_sexLabel->text() + "M");
    }
    else{
        ui->m_sexLabel->setText(ui->m_sexLabel->text() + "F");
    }
    ui->m_birthDate_label->setText(ui->m_birthDate_label->text() + Session::getInstance()->user()->getBirthDate().toString());

    if(Session::getInstance()->user()->getIs_superuser()){
        ui->m_admin_label->setText(ui->m_admin_label->text() + "Admin");
    }
    else{
        ui->m_admin_label->setText(ui->m_admin_label->text() + "Ordinary profile");

    }
}


void MainWidget::logOut()
{
    m_networkManager->setCookieJar(new QNetworkCookieJar);
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(LogoutResponse(QNetworkReply*)));
    QUrlQuery query;
    QUrl url= QUrl("http://127.0.0.1:8000/logout/");
    QByteArray postData;
    postData=query.toString(QUrl::FullyEncoded).toUtf8();
    QNetworkRequest networkRequest(url);
    //networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data; boundary=<calculated when request is sent>");
    //networkRequest.setRawHeader("X-CSRFToken", getCsrftoken());
    m_networkManager->post(networkRequest, postData);
}



void MainWidget::clearProfile()
{
    ui->m_first_name_label->setText("First name: ");
    ui->m_last_name_label->setText("Last name: ");
    ui->m_email_label->setText("Email: ");
    ui->m_sexLabel->setText("Sex: ");
    ui->m_birthDate_label->setText("Birthdate: ");
    ui->m_admin_label->setText("Admin: ");
}


void MainWidget::taskDetails(int id)
{
    TaskModel* t= m_tmFactory->getTaskById(id);
    Session::getInstance()->setTask(t);
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
        ui->m_taskdetailsUsercomboBox->setCurrentText(u->getEmail());
    }
    ui->m_taskdetailsTypeLineEdit->setEnabled(false);
    ui->m_taskdetailsStatusLineEdit->setEnabled(false);
    ui->m_taskdetailsProjectlineEdit->setEnabled(false);
    QVector<CommentModel*> comments= m_tmFactory->getAllCommentsOfTask(id);
    ui->m_taskdetailsCommentslistWidget->clear();
    foreach (CommentModel* c, comments) {
        QListWidgetItem *newItem = new QListWidgetItem(c->getEmail()+ ":" +c->getComment());
        ui->m_taskdetailsCommentslistWidget->addItem(newItem);

    }
    selectPage((int)pages::TASKDETAILS);

}


void MainWidget::getAllUsers()
{
    m_networkManager->setCookieJar(new QNetworkCookieJar);
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getAllUsersResponse(QNetworkReply*)));
    QUrl url= QUrl("http://127.0.0.1:8000/getallusers/");

    QNetworkRequest networkRequest(url);
    //networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data; boundary=<calculated when request is sent>");
    //networkRequest.setRawHeader("X-CSRFToken", getCsrftoken());
    m_networkManager->get(networkRequest);
}


void MainWidget::initTasks()
{
    QStringList taskStatus;
    taskStatus<<"TODO"<<"INPROGRESS"<<"FINISHED";
    ui->m_taskCreateStatuscomboBox->addItems(taskStatus);

    QStringList taskType;
    taskType<<"ORDINARY"<<"BUG"<<"URGENT";
    ui->m_taskCreateTypecomboBox->addItems(taskType);

    foreach (ProjectModel* p, m_tmFactory->projectsList()) {
        ui->m_taskCreateProjectcomboBox->addItem(p->getName());
    }

    foreach(UserModel* u, m_tmFactory->usersList()){
        ui->m_taskCreateUsercomboBox->addItem(u->getEmail());
        ui->m_projectcreatecomboBox->addItem(u->getEmail());
        ui->m_taskdetailsUsercomboBox->addItem(u->getEmail());
    }

}


void MainWidget::initComments()
{
    m_networkManager->setCookieJar(new QNetworkCookieJar);
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getAllCommentsResponse(QNetworkReply*)));
    QUrl url= QUrl("http://127.0.0.1:8000/getallcomments/");

    QNetworkRequest networkRequest(url);
    //networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data; boundary=<calculated when request is sent>");
    //networkRequest.setRawHeader("X-CSRFToken", getCsrftoken());
    m_networkManager->get(networkRequest);
}


void MainWidget::projects()
{
    QUrl url;
    UserModel* u= Session::getInstance()->user();
    m_networkManager->setCookieJar(new QNetworkCookieJar);
    if(m_tmFactory->projectsList().size()<=0){
        connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(ProjectResponse(QNetworkReply*)));
        if(u->getIs_superuser()){
            url= QUrl("http://127.0.0.1:8000/getallprojects/");
        }
        else{
            url=QUrl("http://127.0.0.1:8000/getallprojectsofuser/"+ QString::number(u->getId()));
        }

        QNetworkRequest networkRequest(url);
        //networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data; boundary=<calculated when request is sent>");
        //networkRequest.setRawHeader("X-CSRFToken", getCsrftoken());
        m_networkManager->get(networkRequest);
    }
    qDebug()<<u->getId();
    selectPage((int)pages::PROJECT);
}


void MainWidget::chooseMenuItem(int id)
{
    switch (id) {
    case ((int) pages::PROJECT):
        projects();
        break;
    case ((int) pages::PROFILE):
        selectPage(id);
        break;
    default:
        break;
    }
}

void MainWidget::itemDoubleClicked(QListWidgetItem *item)
{
    qDebug()<<"Hvata";
    QStringList taskId = item->text().split(":");
    QString id= taskId[0];
    taskDetails(id.toInt());
}

void MainWidget::itemClicked(QListWidgetItem *item)
{
    qDebug()<<"taskclcked";

    QStringList taskId = item->text().split(":");
    QString id= taskId[0];
    TaskModel* t = m_tmFactory->getTaskById(id.toInt());
    if(t)
    {
        Session::getInstance()->setTask(t);
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

void MainWidget::on_m_logout_button_clicked()
{
    logOut();
}

void MainWidget::on_m_editProfilebutton_clicked()
{
    selectPage((int)pages::EDITPROFILE);
}

void MainWidget::on_m_submitbutton_clicked()
{
    m_networkManager->setCookieJar(new QNetworkCookieJar);
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(EditProfileResponse(QNetworkReply*)));
    QUrlQuery query;
    QUrl url= QUrl("http://127.0.0.1:8000/editprofile/"+QString::number(Session::getInstance()->user()->getId()));
    QByteArray postData;


    if(ui->m_editpasswordlineedit->text()==ui->m_editpasswordconfirmlineedit->text()){

        query.addQueryItem("first_name", ui->m_editfirstnamelineedit->text());
        query.addQueryItem("last_name",ui->m_editlastnamelineedit->text());
        query.addQueryItem("email",ui->m_editemaillineedit->text());
        query.addQueryItem("password",ui->m_editpasswordlineedit->text());
        postData=query.toString(QUrl::FullyEncoded).toUtf8();
        QNetworkRequest networkRequest(url);
        //networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data; boundary=<calculated when request is sent>");
        //networkRequest.setRawHeader("X-CSRFToken", getCsrftoken());
        m_networkManager->post(networkRequest, postData);
    }
    else{
        QMessageBox::warning(this,"Unmatched passwords!","Unmatched passwords!");
    }

}

void MainWidget::on_m_projects_button_clicked()
{

    projects();

}

void MainWidget::on_m_projectslistWidget_itemClicked(QListWidgetItem *item)
{
    QStringList projectId = item->text().split(":");
    QString id= projectId[0];
    ProjectModel* p = m_tmFactory->getProjectById(id.toInt());
    if(p)
    {
        Session::getInstance()->setProject(p);
    }
}

void MainWidget::on_m_editprojectbutton_clicked()
{
    ui->m_projectcreateEditpushButton->setEnabled(true);
    ui->m_createProjectSubmitButton->setVisible(false);
    ProjectModel* p=Session::getInstance()->project();
    if(p)
    {
        ui->m_projectcreateNameLineEdit->setText(p->getName());
        ui->m_projectCreateDescriptionText->setPlainText(p->getDescription());
        ui->m_createProjectCreateDateLabel->setText(ui->m_createProjectCreateDateLabel->text() + p->getCreateDate().toString());
        ui->m_projectcreateDateEdit->setDate(p->getDeadlineDate());
        UserModel* u= m_tmFactory->getUserById(p->getProjectManagerId());
        if(u){
            ui->m_projectcreatecomboBox->setCurrentText(u->getEmail());
        }
    }
    ui->m_projectcreateNameLineEdit->setEnabled(false);
    selectPage((int)pages::CREATEPROJECT);
}

void MainWidget::on_m_deleteprojectbutton_clicked()
{
    m_networkManager->setCookieJar(new QNetworkCookieJar);
    QStringList projectId = ui->m_projectslistWidget->selectedItems().first()->text().split(":");
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(deleteProjectResponse(QNetworkReply*)));
    QUrl url= QUrl("http://127.0.0.1:8000/deleteproject/"+projectId[0]);
    QNetworkRequest networkRequest(url);
    //networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data; boundary=<calculated when request is sent>");
    //networkRequest.setRawHeader("X-CSRFToken", getCsrftoken());
    m_networkManager->deleteResource(networkRequest);
}

void MainWidget::on_m_createprojectbutton_clicked()
{
    ui->m_createProjectSubmitButton->setEnabled(true);
    ui->m_projectcreateEditpushButton->setVisible(false);
    ui->m_createProjectCreateDateLabel->setText("Start date:" + QDate::currentDate().toString());
    selectPage((int)pages::CREATEPROJECT);
}

void MainWidget::on_m_deletetaskbutton_clicked()
{
    m_networkManager->setCookieJar(new QNetworkCookieJar);
    int id= Session::getInstance()->task()->getId();
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(deleteTaskResponse(QNetworkReply*)));
    QUrl url= QUrl("http://127.0.0.1:8000/deletetask/"+QString::number(id));
    QNetworkRequest networkRequest(url);
    //networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data; boundary=<calculated when request is sent>");
    //networkRequest.setRawHeader("X-CSRFToken", getCsrftoken());
    m_networkManager->deleteResource(networkRequest);
}

void MainWidget::on_m_createtaskbutton_clicked()
{
    ui->m_taskCreateStartDateLabel_2->setText("Start date:" + QDate::currentDate().toString());
    ui->m_taskCreateProjectcomboBox->setCurrentText(Session::getInstance()->project()->getName());
    ui->m_taskCreateProjectcomboBox->setEnabled(false);
    selectPage((int)pages::TASKCREATE);
}

void MainWidget::on_m_createProjectSubmitButton_clicked()
{
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(createProjectResponse(QNetworkReply*)));
    m_networkManager->setCookieJar(new QNetworkCookieJar() );
    if(ui->m_projectcreateNameLineEdit->text()!="" && ui->m_projectCreateDescriptionText->toPlainText()!="" && ui->m_projectcreateDateEdit->date()>QDate::currentDate()){
        QUrlQuery query;
        QUrl url= QUrl("http://127.0.0.1:8000/createproject/");
        QByteArray postData;

        query.addQueryItem("namee", ui->m_projectcreateNameLineEdit->text());
        query.addQueryItem("description",ui->m_projectCreateDescriptionText->toPlainText());
        query.addQueryItem("createDate",QString::number(QDate::currentDate().year())+"-"+QString::number(QDate::currentDate().month())+"-"+QString::number(QDate::currentDate().day()));
        query.addQueryItem("deadlineDate",QString::number(ui->m_projectcreateDateEdit->date().year())+"-"+QString::number(ui->m_projectcreateDateEdit->date().month())+"-"+QString::number(ui->m_projectcreateDateEdit->date().day()));
        UserModel* u= m_tmFactory->getUserByEmail(ui->m_projectcreatecomboBox->currentText());
        if(u){
            query.addQueryItem("projectManagerId",QString::number( u->getId()));
        }
        else{
            query.addQueryItem("projectManagerId", "");
        }
        qDebug()<<query.allQueryItemValues("namee");
        postData=query.toString(QUrl::FullyEncoded).toUtf8();
        QNetworkRequest networkRequest(url);
        //networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data; boundary=<calculated when request is sent>");
        //networkRequest.setRawHeader("X-CSRFToken", getCsrftoken());
        //networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        m_networkManager->post(networkRequest, postData);
    }
    else{
        QMessageBox::warning(this,"Invalid input!", "Invalid input!");
    }
}

void MainWidget::on_m_taskCreatepushButton_clicked()
{
    m_networkManager->setCookieJar(new QNetworkCookieJar);
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(createTaskResponse(QNetworkReply*)));
    if(ui->m_taskCreateNameLineEdit_2->text()!="" && ui->m_taskCreateDescriptionTextEdit_2->toPlainText()!="" && ui->m_taskCreateFinishdateEdit_2->date()>QDate::currentDate()){
        QUrlQuery query;
        QUrl url= QUrl("http://127.0.0.1:8000/createtask/"+QString::number(Session::getInstance()->project()->getId())+"/"+QString::number(Session::getInstance()->user()->getId()));
        QByteArray postData;

        query.addQueryItem("name", ui->m_taskCreateNameLineEdit_2->text());
        query.addQueryItem("type",QString::number(ui->m_taskCreateTypecomboBox->currentIndex()+1));
        query.addQueryItem("description",ui->m_taskCreateDescriptionTextEdit_2->toPlainText());
        query.addQueryItem("status",QString::number(ui->m_taskCreateStatuscomboBox->currentIndex()+1));
        query.addQueryItem("startDate",QString::number(QDate::currentDate().year())+"-"+QString::number(QDate::currentDate().month())+"-"+QString::number(QDate::currentDate().day()));
        query.addQueryItem("finishDate",QString::number(ui->m_taskCreateFinishdateEdit_2->date().year())+"-"+QString::number(ui->m_taskCreateFinishdateEdit_2->date().month())+"-"+QString::number(ui->m_taskCreateFinishdateEdit_2->date().day()));
        postData=query.toString(QUrl::FullyEncoded).toUtf8();
        QNetworkRequest networkRequest(url);
        //networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data; boundary=<calculated when request is sent>");
        //networkRequest.setRawHeader("X-CSRFToken", getCsrftoken());

        //networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        m_networkManager->post(networkRequest, postData);
    }
    else{
        QMessageBox::warning(this,"Invalid input!", "Invalid input!");
    }
}

void MainWidget::on_m_projectslistWidget_itemDoubleClicked(QListWidgetItem *item)
{
    m_networkManager->setCookieJar(new QNetworkCookieJar);
    QStringList projectId = item->text().split(":");
    QString id=projectId[0];
    ProjectModel* p=m_tmFactory->getProjectById(id.toInt());
    if(p){
        Session::getInstance()->setProject(p);
    }
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(TaskResponse(QNetworkReply*)));
    QUrl url= QUrl("http://127.0.0.1:8000/getalltasksofproject/"+projectId[0]);
    QNetworkRequest networkRequest(url);
    //networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data; boundary=<calculated when request is sent>");
    //networkRequest.setRawHeader("X-CSRFToken", getCsrftoken());

    m_networkManager->get(networkRequest);

}

void MainWidget::on_m_taskdetailsSubmitpushButton_clicked()
{
    m_networkManager->setCookieJar(new QNetworkCookieJar);
    int id= Session::getInstance()->task()->getId();
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(editTaskResponse(QNetworkReply*)));
    QUrlQuery query;
    QUrl url= QUrl("http://127.0.0.1:8000/edittask/"+QString::number(id));
    QByteArray postData;
    query.addQueryItem("name", ui->m_taskdetailsNameLineEdit->text());
    query.addQueryItem("description",ui->m_taskdetailsDescriptionTextEdit->toPlainText());
    query.addQueryItem("finishDate",QString::number(ui->m_taskdetailsFinishdateEdit->date().year())+"-"+QString::number(ui->m_taskdetailsFinishdateEdit->date().month())+"-"+QString::number(ui->m_taskdetailsFinishdateEdit->date().day()));
    UserModel* u= m_tmFactory->getUserByEmail(ui->m_taskdetailsUsercomboBox->currentText());
    if(u){
        query.addQueryItem("userId",QString::number( u->getId()));
    }
    else{
        query.addQueryItem("userId", "");
    }
    postData=query.toString(QUrl::FullyEncoded).toUtf8();
    QNetworkRequest networkRequest(url);
    //networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data; boundary=<calculated when request is sent>");
    //networkRequest.setRawHeader("X-CSRFToken", getCsrftoken());

    m_networkManager->post(networkRequest, postData);
}

void MainWidget::on_m_projectdetailsPushbutton_clicked()
{
    ui->m_createProjectSubmitButton->setEnabled(false);
    ui->m_projectcreateEditpushButton->setEnabled(false);
    ProjectModel* p=Session::getInstance()->project();
    if(p)
    {
        ui->m_projectcreateNameLineEdit->setText(p->getName());
        ui->m_projectCreateDescriptionText->setPlainText(p->getDescription());
        //ui->m_taskdetailsStatusLineEdit->setText(QString::number(t->getStatus()));
        ui->m_createProjectCreateDateLabel->setText("Create date: " + p->getCreateDate().toString());
        ui->m_projectcreateDateEdit->setDate(p->getDeadlineDate());
        UserModel* u= m_tmFactory->getUserById(p->getProjectManagerId());
        if(u){
            ui->m_projectcreatecomboBox->setCurrentText(u->getEmail());
        }
    }
    selectPage((int)pages::CREATEPROJECT);
}

void MainWidget::on_m_projectcreateEditpushButton_clicked()
{
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(editProjectResponse(QNetworkReply*)));
    QUrlQuery query;
    m_networkManager->setCookieJar(new QNetworkCookieJar());
    int id= Session::getInstance()->project()->getId();
    QUrl url= QUrl("http://127.0.0.1:8000/editproject/"+QString::number(id));
    QByteArray postData;
    query.addQueryItem("description",ui->m_projectCreateDescriptionText->toPlainText());
    query.addQueryItem("deadlineDate",QString::number(ui->m_projectcreateDateEdit->date().year())+"-"+QString::number(ui->m_projectcreateDateEdit->date().month())+"-"+QString::number(ui->m_projectcreateDateEdit->date().day()));
    UserModel* u= m_tmFactory->getUserByEmail(ui->m_projectcreatecomboBox->currentText());
    if(u){
        query.addQueryItem("projectManagerId", QString::number(u->getId()));
    }
    else{
        query.addQueryItem("projectManagerId", "");
    }
    postData=query.toString(QUrl::FullyEncoded).toUtf8();
    QNetworkRequest networkRequest(url);
    //networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data; boundary=<calculated when request is sent>");
    //networkRequest.setRawHeader("X-CSRFToken", getCsrftoken());

    //networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    m_networkManager->post(networkRequest, postData);

}

void MainWidget::on_m_commentSubmitpushButton_clicked()
{
    m_networkManager->setCookieJar(new QNetworkCookieJar);
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(addCommentResponse(QNetworkReply*)));
    QUrlQuery query;
    int tid= Session::getInstance()->task()->getId();
    int uid=Session::getInstance()->user()->getId();
    QUrl url= QUrl("http://127.0.0.1:8000/createcommentontask/"+QString::number(uid)+"/"+QString::number(tid));
    QByteArray postData;
    query.addQueryItem("comment_on_task", ui->m_commentplainTextEdit->toPlainText());
    postData=query.toString(QUrl::FullyEncoded).toUtf8();
    QNetworkRequest networkRequest(url);
    //networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data; boundary=<calculated when request is sent>");
    //networkRequest.setRawHeader("X-CSRFToken", getCsrftoken());

    //networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    m_networkManager->post(networkRequest, postData);
}

void MainWidget::on_m_taskdetailsCommentpushButton_clicked()
{
    selectPage((int)pages::COMMENT);
}


void MainWidget::on_m_tasklisttodoWidget_itemEntered(QListWidgetItem *item)
{
    qDebug()<<"item to do entered";
}


void MainWidget::on_m_tasklistinprogresWidget_itemEntered(QListWidgetItem *item)
{
    qDebug()<<"item in progress entered";
}


void MainWidget::on_m_tasklistdoneWidget_itemEntered(QListWidgetItem *item)
{
    qDebug()<<"item done entered";
}

void MainWidget::changeStatusSlot(int task_status)
{
    m_networkManager->setCookieJar(new QNetworkCookieJar);
    tStatus=task_status;
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(changeStatusResponse(QNetworkReply*)));
    QUrlQuery query;
    int tid= Session::getInstance()->task()->getId();
    //qDebug()<<QString::number((int)type);
    QUrl url= QUrl("http://127.0.0.1:8000/changetaskstatus/"+QString::number(tid));
    QByteArray postData;
    query.addQueryItem("status",QString::number(task_status));
    postData=query.toString(QUrl::FullyEncoded).toUtf8();
    QNetworkRequest networkRequest(url);
    //networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data; boundary=<calculated when request is sent>");
    //networkRequest.setRawHeader("X-CSRFToken", getCsrftoken());

    //networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    m_networkManager->post(networkRequest, postData);
}

void MainWidget::on_m_putUserOnProjectSubmitpushButton_clicked()
{
    m_networkManager->setCookieJar(new QNetworkCookieJar);
    QStringList projectId = ui->m_putUserOnProjectProjectslistWidget->currentItem()->text().split(":");
    QString id= projectId[0];
    QString email= ui->m_putUserOnProjectUserscomboBox->currentText();
    UserModel* u = m_tmFactory->getUserByEmail(email);
    QString uId= QString::number(u->getId());
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(putUserOnProjectResponse(QNetworkReply*)));
    QUrlQuery query;
    QUrl url= QUrl("http://127.0.0.1:8000/createuseronproject/"+uId+"/"+id);
    QNetworkRequest networkRequest(url);
    //networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data; boundary=<calculated when request is sent>");
    //networkRequest.setRawHeader("X-CSRFToken", getCsrftoken());

    QByteArray postData;
    m_networkManager->post(networkRequest, postData);
}


void MainWidget::on_m_projectsPutUserOnProjectpushButton_clicked()
{
    selectPage((int)pages::PUTUSERONPROJECT);
}

void MainWidget::RequestResponse(QNetworkReply *reply)
{
    QMessageBox::warning(this, "message", QString(reply->readAll()));
    selectPage((int)pages::LOGIN);
    disconnect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(RequestResponse(QNetworkReply*)));
}

void MainWidget::LoginResponse(QNetworkReply *reply)
{
    QList<QNetworkCookie> listOfCookies= m_networkManager->cookieJar()->cookiesForUrl(QUrl("http://127.0.0.1:8000/login/"));
    foreach (QNetworkCookie c, listOfCookies) {
        if(c.name()=="csrftoken"){
            setCsrftoken(c.value());
        }

    }
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

    Session::getInstance()->setUser(new UserModel(obj));

    initProfile();
    getAllUsers();
    qDebug()<<Session::getInstance()->user()->getFirst_name();
    emit showSideBar();
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



void MainWidget::EditProfileResponse(QNetworkReply *reply)
{
    qDebug()<<reply->readAll();
    if(ui->m_editfirstnamelineedit->text()!=""){
        Session::getInstance()->user()->setFirst_name(ui->m_editfirstnamelineedit->text());
    }
    if(ui->m_editlastnamelineedit->text()!=""){
        Session::getInstance()->user()->setLast_name(ui->m_editlastnamelineedit->text());
    }
    if(ui->m_editemaillineedit->text()!=""){
        Session::getInstance()->user()->setEmail(ui->m_editemaillineedit->text());
    }
    if(ui->m_editpasswordlineedit->text()!=""){
        Session::getInstance()->user()->setPassword(ui->m_editpasswordlineedit->text());
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
        ui->m_putUserOnProjectProjectslistWidget->addItem(new QListWidgetItem(QString::number(p->getId())+":"+p->getName()));
    }
    initTasks();
    initComments();
    selectPage((int)pages::PROJECT);
    disconnect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(ProjectResponse(QNetworkReply*)));
}

void MainWidget::TaskResponse(QNetworkReply *reply)
{
    QString response  = QString(reply->readAll());
    qDebug()<<response;
    QJsonArray obj;

    QJsonDocument doc= QJsonDocument::fromJson(response.toUtf8());
    obj = doc.array();
    // check validity of the document
    m_listDone->clear();
    m_listInProgress->clear();
    m_listToDo->clear();
    for (int i = 0; i < obj.size(); ++i) {
        TaskModel* t = m_tmFactory->createTask(obj[i].toObject());
        qDebug()<<t->getName();
        m_tmFactory->addTask(t);

        if(t->getStatus()==(int)taskStatus::TODO)
        {
            m_listToDo->addItem(new QListWidgetItem(QString::number(t->getId())+":"+t->getName()));
        }
        if(t->getStatus()==(int)taskStatus::INPROGRESS)
        {
            m_listInProgress->addItem(new QListWidgetItem(QString::number(t->getId())+":"+t->getName()));
        }
        if(t->getStatus()==(int)taskStatus::FINISHED)
        {
            m_listDone->addItem(new QListWidgetItem(QString::number(t->getId())+":"+t->getName()));
        }

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
        qDebug()<<u->getEmail();
        m_tmFactory->addUser(u);
        ui->m_putUserOnProjectUserscomboBox->addItem(u->getEmail());
    }

    disconnect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getAllUsersResponse(QNetworkReply*)));
}

void MainWidget::createTaskResponse(QNetworkReply *reply)
{
    QString response= reply->readAll();
    QJsonObject obj;
    QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());

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
    TaskModel* t = m_tmFactory->createTask(obj);
    if(t){
        m_tmFactory->addTask(t);
        if(t->getStatus()==(int)taskStatus::TODO)
        {
            m_listToDo->addItem(new QListWidgetItem(QString::number(t->getId())+":"+t->getName()));
        }
        if(t->getStatus()==(int)taskStatus::INPROGRESS)
        {
            m_listInProgress->addItem(new QListWidgetItem(QString::number(t->getId())+":"+t->getName()));
        }
        if(t->getStatus()==(int)taskStatus::FINISHED)
        {
            m_listDone->addItem(new QListWidgetItem(QString::number(t->getId())+":"+t->getName()));
        }
    }
    selectPage((int)pages::TASK);
    disconnect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(createTaskResponse(QNetworkReply*)));
}

void MainWidget::deleteTaskResponse(QNetworkReply *reply)
{
    TaskModel* t= Session::getInstance()->task();
    if(t->getStatus()==(int)taskStatus::TODO && !m_listToDo->selectedItems().empty())
        delete m_listToDo->selectedItems().first();
    if(t->getStatus()==(int)taskStatus::INPROGRESS && !m_listInProgress->selectedItems().empty())
        delete m_listInProgress->selectedItems().first();
    if(t->getStatus()==(int)taskStatus::FINISHED && !m_listDone->selectedItems().empty())
        delete m_listDone->selectedItems().first();
    Session::getInstance()->setTask(nullptr);
    disconnect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(deleteTaskResponse(QNetworkReply*)));
}

void MainWidget::editTaskResponse(QNetworkReply *reply)
{
    QMessageBox::warning(this, "Task edited", QString(reply->readAll()));
    TaskModel* t= Session::getInstance()->task();
    t->setName(ui->m_taskdetailsNameLineEdit->text());
    t->setDescription(ui->m_taskdetailsDescriptionTextEdit->toPlainText());
    t->setFinishDate(ui->m_taskdetailsFinishdateEdit->date());
    UserModel* u= m_tmFactory->getUserByEmail(ui->m_taskdetailsUsercomboBox->currentText());
    if(u){
        t->setUserId(u->getId());
    }
    disconnect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(editTaskResponse(QNetworkReply*)));
    selectPage((int)pages::TASK);
}

void MainWidget::deleteProjectResponse(QNetworkReply *reply)
{
    if(ui->m_projectslistWidget->selectedItems().size()>0)
        delete ui->m_projectslistWidget->selectedItems().first();
    disconnect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(deleteProjectResponse(QNetworkReply*)));
}

void MainWidget::createProjectResponse(QNetworkReply *reply)
{
    QString response= reply->readAll();
    QJsonObject obj;
    QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());

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
    ProjectModel* p = m_tmFactory->createProject(obj);
    if(p){
        m_tmFactory->addProject(p);
        ui->m_projectslistWidget->addItem(new QListWidgetItem(QString::number(p->getId())+":"+p->getName()));
        selectPage((int)pages::PROJECT);
        disconnect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(createProjectResponse(QNetworkReply*)));
    }

}

void MainWidget::editProjectResponse(QNetworkReply *reply)
{
    QMessageBox::warning(this, "Project changed!", QString(reply->readAll()));
    ProjectModel* p= Session::getInstance()->project();
    p->setDescription(ui->m_projectCreateDescriptionText->toPlainText());
    p->setDeadlineDate(ui->m_projectcreateDateEdit->date());
    UserModel* u= m_tmFactory->getUserByEmail(ui->m_projectcreatecomboBox->currentText());
    if(u){
        p->setProjectManagerId(u->getId());
    }
    selectPage((int)pages::PROJECT);
    disconnect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(editProjectResponse(QNetworkReply*)));
}

void MainWidget::getAllCommentsResponse(QNetworkReply *reply)
{
    QString response  = QString(reply->readAll());
    qDebug()<<response;
    QJsonArray obj;

    QJsonDocument doc= QJsonDocument::fromJson(response.toUtf8());
    obj = doc.array();
    // check validity of the document

    for (int i = 0; i < obj.size(); ++i) {
        CommentModel* c = m_tmFactory->createComment(obj[i].toObject());
        qDebug()<<c->getComment();
        m_tmFactory->addComment(c);
    }

    disconnect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getAllCommentsResponse(QNetworkReply*)));
}

void MainWidget::addCommentResponse(QNetworkReply *reply)
{
    QMessageBox::warning(this, "Comment added", "Comment added");
    QString response= reply->readAll();
    QJsonObject obj;
    QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());

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
    CommentModel* c= m_tmFactory->createComment(obj);
    if(c){
        m_tmFactory->addComment(c);
        qDebug()<<c->getComment();
        ui->m_taskdetailsCommentslistWidget->addItem(new QListWidgetItem(c->getEmail()+":"+c->getComment()));
        selectPage((int)pages::TASKDETAILS);
        disconnect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(addCommentResponse(QNetworkReply*)));
    }
}

void MainWidget::changeStatusResponse(QNetworkReply *reply)
{
    QMessageBox::warning(this, "Changed status", "Changed status");
    QString response= reply->readAll();
    QJsonObject obj;
    QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());

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
    TaskModel* t= m_tmFactory->createTask(obj);
    if(t){
        TaskModel* ct=Session::getInstance()->task();
        ct->setStatus(t->getStatus());
        delete t;
    }
    disconnect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(changeStatusResponse(QNetworkReply*)));
}

void MainWidget::putUserOnProjectResponse(QNetworkReply *reply)
{
    QMessageBox::warning(this,"Succesfully", "Succesfully put user on project!");
    disconnect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(putUserOnProjectResponse(QNetworkReply*)));
    selectPage((int)pages::PROJECT);
}




