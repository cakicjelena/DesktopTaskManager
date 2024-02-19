#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

class QNetworkAccessManager;
class QNetworkReply;
class UserModel;


enum class pages{
    MAIN = 0,
    LOGIN= 1,
    REGISTRATION = 2,
    PROFILE= 3
};

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT
protected:
    QNetworkAccessManager* m_networkManager;
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

    void on_m_logout_button_clicked();

    void on_m_tasks_button_clicked();

    void on_m_editProfilebutton_clicked();

private:
    Ui::MainWidget *ui;
};

#endif // MAINWIDGET_H
