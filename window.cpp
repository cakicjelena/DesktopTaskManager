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
}

void Window::initialize()
{
    m_mainwidget= new MainWidget(this);
    setCentralWidget(m_mainwidget);
}
