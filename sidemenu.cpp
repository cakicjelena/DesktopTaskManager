#include "sidemenu.h"
#include "ui_sidemenu.h"
#include "QPushButton"
#include "mainwidget.h"

void SideMenu::initialize()
{
    m_menuItems.addButton(new QPushButton("Profile", this ),(int) pages::PROFILE);
    ui->verticalLayout->addWidget(m_menuItems.button((int) pages::PROFILE));

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
