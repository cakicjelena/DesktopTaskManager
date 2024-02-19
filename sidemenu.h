#ifndef SIDEMENU_H
#define SIDEMENU_H
#include "qbuttongroup.h"

#include <QWidget>
enum class pages;
namespace Ui {
class SideMenu;
}

class SideMenu : public QWidget
{
    Q_OBJECT
protected:
    QButtonGroup m_menuItems;
protected:
    void initialize();
public:
    explicit SideMenu(QWidget *parent = nullptr);
    ~SideMenu();

private:
    Ui::SideMenu *ui;
};

#endif // SIDEMENU_H
