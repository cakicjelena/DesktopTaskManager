#include "session.h"
#include "taskmodel.h"
#include "usermodel.h"
#include "projectmodel.h"
Session* Session::m_instance=nullptr;
UserModel *Session::user() const
{
    return m_user;
}

void Session::setUser(UserModel *newUser)
{
    m_user = newUser;
}

TaskModel *Session::task() const
{
    return m_task;
}

void Session::setTask(TaskModel *newTask)
{
    m_task = newTask;
}

Session::~Session()
{
    if(m_instance){
        delete m_instance;
    }
}

Session *Session::getInstance()
{
    if(!m_instance){
        m_instance=new Session();
    }
    return m_instance;
}

ProjectModel *Session::project() const
{
    return m_project;
}

void Session::setProject(ProjectModel *newProject)
{
    m_project = newProject;
}

Session::Session() {}
