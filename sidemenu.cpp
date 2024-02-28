#include "sidemenu.h"
#include "ui_sidemenu.h"
#include "QPushButton"
#include "mainwidget.h"


void SideMenu::initialize()
{
    m_menuItems.addButton(new QPushButton("Profile", this ),(int) pages::PROFILE);
    ui->verticalLayout->addWidget(m_menuItems.button((int) pages::PROFILE));
    m_menuItems.button((int) pages::PROFILE)->setFixedSize(m_size);
    m_menuItems.addButton(new QPushButton("Projects", this ),(int) pages::PROJECT);
    ui->verticalLayout->addWidget(m_menuItems.button((int) pages::PROJECT));
    m_menuItems.button((int) pages::PROJECT)->setFixedSize(m_size);
    m_menuItems.addButton(new QPushButton("Log out", this ), LOG_OUT);
    ui->verticalLayout->addWidget(m_menuItems.button((int) LOG_OUT));
    m_menuItems.button((int) LOG_OUT)->setFixedSize(m_size);
    connect(&m_menuItems, SIGNAL(idClicked(int)), this, SLOT(menuItemClicked(int)));


}

SideMenu::SideMenu(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SideMenu)
{
    ui->setupUi(this);
    initialize();
}

SideMenu::~SideMenu()
{
    delete ui;
}

void SideMenu::menuItemClicked(int id)
{
    emit menuItem(id);
}
