#include "window.h"
#include "ui_window.h"
#include "mainwidget.h"

Window::Window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Window)
{
    ui->setupUi(this);
    initialize();
}

Window::~Window()
{
    delete ui;
    delete m_mainwidget;
    delete m_sidemenu;
    delete m_sidemenu_dockwidget;
}

void Window::initialize()
{
    m_mainwidget= new MainWidget(this);
    m_sidemenu= new SideMenu(this);
    m_sidemenu_dockwidget= new QDockWidget(this);
    setCentralWidget(m_mainwidget);
    m_sidemenu_dockwidget->setWidget(m_sidemenu);
    addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, m_sidemenu_dockwidget);
    m_sidemenu_dockwidget->hide();
}
