#ifndef MAINWIDGET_H
#define MAINWIDGET_H
#include "window.h"

#include <QWidget>

class QNetworkAccessManager;
class QNetworkReply;
class UserModel;
class TMFactory;
class QListWidgetItem;
class TaskList;



enum class pages{
    MAIN = 0,
    LOGIN= 1,
    REGISTRATION = 2,
    PROFILE= 3,
    EDITPROFILE=4,
    PROJECT=5,
    CREATEPROJECT=6,
    TASK=7,
    TASKDETAILS=8,
    TASKCREATE=9,
    COMMENT=10,
    PUTUSERONPROJECT=11

};

enum class taskStatus{
    TODO=1,
    INPROGRESS=2,
    FINISHED=3
};

enum class taskType{
    ORDINARY=1,
    BUG=2,
    URGENT=3
};

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT
    friend class Window;
protected:
    QNetworkAccessManager* m_networkManager;
    TMFactory* m_tmFactory;
    UserModel* m_user;
    TaskList* m_listToDo;
    TaskList* m_listInProgress;
    TaskList* m_listDone;
    QByteArray csrftoken;

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
    ///@brief Method for page selection
    ///
    void selectPage(int index);

    QByteArray getCsrftoken() const;
    void setCsrftoken(const QByteArray &newCsrftoken);

protected:
    ///@brief Method for initialization
    ///
    void initialize();
    void initProfile();
    void logOut();
    void clearProfile();
    void taskDetails(int id);
    void getAllUsers();
    void initTasks();
    void initComments();
    void projects();
    void chooseMenuItem(int id);

private slots:
    void itemDoubleClicked(QListWidgetItem *item);
    void itemClicked(QListWidgetItem *item);
    void on_m_loginButton_2_clicked();

    void on_m_signupButton_clicked();

    void on_m_registerButton_clicked();

    void RequestResponse(
        QNetworkReply *reply); ///< Slot called when request is finished

    void on_m_loginButton_clicked();

    void LoginResponse(
        QNetworkReply *reply); ///< Slot called when login is finished
    void LogoutResponse(
        QNetworkReply *reply); ///< Slot called when logout is finished

    void EditProfileResponse(
        QNetworkReply *reply); ///< Slot called when edit profile is finished

    void ProjectResponse(
        QNetworkReply *reply); ///< Slot called when project is finished

    void TaskResponse(
        QNetworkReply *reply); ///< Slot called when task is finished

    void getAllUsersResponse(
        QNetworkReply *reply); ///< Slot called when getting all users is finished  

    void createTaskResponse(
        QNetworkReply *reply); ///< Slot called when creating task is finished
    void deleteTaskResponse(
        QNetworkReply *reply); ///< Slot called when deleting task is finished

    void editTaskResponse(
        QNetworkReply *reply); ///< Slot called when editing task is finished
    void deleteProjectResponse(
        QNetworkReply *reply); ///< Slot called when deleting project is finished
    void createProjectResponse(
        QNetworkReply *reply); ///< Slot called when creating project is finished
    void editProjectResponse(
        QNetworkReply *reply); ///< Slot called when editing project is finished
    void getAllCommentsResponse(
        QNetworkReply *reply); ///< Slot called when getting comments is finished

    void addCommentResponse(
        QNetworkReply *reply); ///< Slot called when adding comment is finished
    void changeStatusResponse(
        QNetworkReply *reply); ///< Slot called when changing status of task is finished
    void putUserOnProjectResponse(
        QNetworkReply *reply); ///< Slot called when putting user on project is finished

    void on_m_logout_button_clicked();

    void on_m_editProfilebutton_clicked();

    void on_m_submitbutton_clicked();

    void on_m_projects_button_clicked();

    void on_m_projectslistWidget_itemClicked(QListWidgetItem *item);

    void on_m_editprojectbutton_clicked();

    void on_m_deleteprojectbutton_clicked();

    void on_m_createprojectbutton_clicked();

    void on_m_deletetaskbutton_clicked();

    void on_m_createtaskbutton_clicked();

    void on_m_createProjectSubmitButton_clicked();

    void on_m_taskCreatepushButton_clicked();

    void on_m_projectslistWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_m_taskdetailsSubmitpushButton_clicked();

    void on_m_projectdetailsPushbutton_clicked();

    void on_m_projectcreateEditpushButton_clicked();

    void on_m_commentSubmitpushButton_clicked();

    void on_m_taskdetailsCommentpushButton_clicked();
    void on_m_tasklisttodoWidget_itemEntered(QListWidgetItem *item);

    void on_m_tasklistinprogresWidget_itemEntered(QListWidgetItem *item);

    void on_m_tasklistdoneWidget_itemEntered(QListWidgetItem *item);
    void changeStatusSlot(int task_status);

        void on_m_putUserOnProjectSubmitpushButton_clicked();

    void on_m_projectsPutUserOnProjectpushButton_clicked();

signals:
    void showSideBar();
private:
    Ui::MainWidget *ui;
};

#endif // MAINWIDGET_H
