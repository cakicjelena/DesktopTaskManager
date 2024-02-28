#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include "sidemenu.h"

class MainWidget;
QT_BEGIN_NAMESPACE
namespace Ui {
class Window;
}
QT_END_NAMESPACE

class Window : public QMainWindow
{
    Q_OBJECT
protected:
    MainWidget* m_mainwidget; ///< Pointer to main widget object
    SideMenu* m_sidemenu; ///< Pointer to side menu widget object
    QDockWidget* m_sidemenu_dockwidget; ///< Pointer to side menu dock widget
public:
    Window(QWidget *parent = nullptr);
    ~Window();
protected:
    ///@brief Method for initialization
    ///
    void initialize();

protected slots:
    void sideBarShow();
    void menuItemClicked(int id);
private:
    Ui::Window *ui;
};
#endif // WINDOW_H
