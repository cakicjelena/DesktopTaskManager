#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

class QNetworkAccessManager;
class QNetworkReply;


enum class pages{
    MAIN = 0,
    LOGIN= 1,
    REGISTRATION = 2
};

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT
protected:
    QNetworkAccessManager* m_networkManager;
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

private slots:
    void on_m_loginButton_2_clicked();

    void on_m_signupButton_clicked();

    void on_m_registerButton_clicked();

    void RequestResponse(
        QNetworkReply *reply); ///< Slot called when request is finished

private:
    Ui::MainWidget *ui;
};

#endif // MAINWIDGET_H
