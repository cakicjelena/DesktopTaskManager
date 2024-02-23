#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

class QNetworkAccessManager;
class QNetworkReply;
class UserModel;
class TMFactory;
class QListWidgetItem;


enum class pages{
    MAIN = 0,
    LOGIN= 1,
    REGISTRATION = 2,
    PROFILE= 3,
    EDITPROFILE=4,
    PROJECT=5,
    CREATEPROJECT=6,
    TASK=7,
    TASKDETAILS=8

};

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT
protected:
    QNetworkAccessManager* m_networkManager;
    TMFactory* m_tmFactory;
    UserModel* m_user;

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
    ///@brief Method for page selection
    ///
    void selectPage(int index);
protected:
    ///@brief Method for initialization
    ///
    void initialize();
    void initProfile();
    void clearProfile();
    void taskDetails(int id);
    void getAllUsers();

private slots:
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

    void on_m_logout_button_clicked();

    void on_m_editProfilebutton_clicked();

    void on_m_submitbutton_clicked();

    void on_m_projects_button_clicked();

    void on_m_projectslistWidget_itemClicked(QListWidgetItem *item);

    void on_m_editprojectbutton_clicked();

    void on_m_deleteprojectbutton_clicked();

    void on_m_createprojectbutton_clicked();

    void on_m_deletetaskbutton_clicked();

    void on_m_edittaskbutton_clicked();

    void on_m_createtaskbutton_clicked();

    void on_m_tasklistdoneWidget_itemClicked(QListWidgetItem *item);

    void on_m_tasklistinprogresWidget_itemClicked(QListWidgetItem *item);

    void on_m_tasklistWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::MainWidget *ui;
};

#endif // MAINWIDGET_H
