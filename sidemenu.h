#ifndef SIDEMENU_H
#define SIDEMENU_H
#include "qbuttongroup.h"

#include <QWidget>
enum class pages;
#define LOG_OUT 45
namespace Ui {
class SideMenu;
}

class SideMenu : public QWidget
{
    Q_OBJECT
protected:
    const QSize m_size=QSize(100,100);
    QButtonGroup m_menuItems;
protected:
    void initialize();
public:
    explicit SideMenu(QWidget *parent = nullptr);
    ~SideMenu();

private:
    Ui::SideMenu *ui;

protected slots:
    void menuItemClicked(int id);

signals:
    void menuItem(int id);
};

#endif // SIDEMENU_H
