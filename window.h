#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>

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
public:
    Window(QWidget *parent = nullptr);
    ~Window();
protected:
    ///@brief Method for initialization
    ///
    void initialize();
private:
    Ui::Window *ui;
};
#endif // WINDOW_H
