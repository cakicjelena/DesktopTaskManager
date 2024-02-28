#include "window.h"
#include "ui_window.h"
#include "mainwidget.h"
#include "sidemenu.h"

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
    connect(m_mainwidget, SIGNAL(showSideBar()), this, SLOT(sideBarShow()));
    connect(m_sidemenu, SIGNAL(menuItem(int)), this, SLOT(menuItemClicked(int)));
}

void Window::sideBarShow()
{
    m_sidemenu_dockwidget->show();
}

void Window::menuItemClicked(int id)
{
    switch (id) {
    case LOG_OUT:
        m_mainwidget->logOut();
        break;
    default:
        m_mainwidget->chooseMenuItem(id);
        break;
    }
}
