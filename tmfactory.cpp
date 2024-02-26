#include "tmfactory.h"
#include "qjsonobject.h"
#include "projectmodel.h"
#include "taskmodel.h"
#include "usermodel.h"
#include "commentmodel.h"


QVector<ProjectModel *> TMFactory::projectsList() const
{
    return m_projectsList;
}

QVector<TaskModel *> TMFactory::tasksList() const
{
    return m_tasksList;
}

QVector<UserModel *> TMFactory::usersList() const
{
    return m_usersList;
}

TMFactory::TMFactory() {}

TMFactory::~TMFactory()
{
    for (int i = 0; i < m_usersList.size(); ++i) {
        delete m_usersList[i];
    }
    for (int i = 0; i < m_projectsList.size(); ++i) {
        delete m_projectsList[i];
    }
    for (int i = 0; i < m_tasksList.size(); ++i) {
        delete m_tasksList[i];
    }
}

ProjectModel *TMFactory::createProject(QJsonObject object)
{
    return new ProjectModel(object);
}

TaskModel *TMFactory::createTask(QJsonObject object)
{
    return new TaskModel(object);
}

ProjectModel *TMFactory::getProjectById(int id)
{
    foreach (ProjectModel* p, m_projectsList) {
        if(p->getId()==id){
            return p;
        }
    }
    return nullptr;
}

ProjectModel *TMFactory::getProjectByName(QString name)
{
    foreach (ProjectModel* p, m_projectsList) {
        if(p->getName()==name){
            return p;
        }
    }
    return nullptr;
}

TaskModel *TMFactory::getTaskById(int id)
{
    foreach (TaskModel* t, m_tasksList) {
        if(t->getId()==id){
            return t;
        }
    }
    return nullptr;
}

void TMFactory::addProject(ProjectModel * p)
{
    if(!ifProjectExist(p)){
        m_projectsList.append(p);
    }
}

void TMFactory::addTask(TaskModel * t)
{
    if(!ifTaskExist(t)){
        m_tasksList.append(t);
    }
}

void TMFactory::deleteProject(ProjectModel *p)
{
    if(ifProjectExist(p)){
        m_projectsList.removeOne(p);
    }
    delete p;
}

void TMFactory::deleteTask(TaskModel *t)
{
    if(ifTaskExist(t)){
        m_tasksList.removeOne(t);
    }
    delete t;
}

bool TMFactory::ifProjectExist(ProjectModel *p)
{
    foreach (ProjectModel* pr, m_projectsList) {
        if(pr->getId()==p->getId()){
            return true;
        }
        return false;
    }
}

bool TMFactory::ifTaskExist(TaskModel *t)
{
    foreach (TaskModel* ta, m_tasksList) {
        if(ta->getId()==t->getId()){
            return true;
        }
        return false;
    }
}

UserModel *TMFactory::createUser(QJsonObject object)
{
    return new UserModel(object);
}

UserModel *TMFactory::getUserById(int id)
{
    foreach (UserModel* u, m_usersList) {
        if(u->getId()==id){
            return u;
        }
    }
    return nullptr;
}

UserModel *TMFactory::getUserByEmail(QString email)
{
    foreach (UserModel* u, m_usersList) {
        if(u->getEmail()==email){
            return u;
        }
    }
    return nullptr;
}


void TMFactory::addUser(UserModel *u)
{
    if(!ifUserExist(u)){
        m_usersList.append(u);
    }
}

void TMFactory::deleteUser(UserModel *u)
{
    if(ifUserExist(u)){
        m_usersList.removeOne(u);
    }
    delete u;
}

bool TMFactory::ifUserExist(UserModel *u)
{
    foreach (UserModel* us, m_usersList) {
        if(us->getId()==u->getId()){
            return true;
        }
        return false;
    }
}

CommentModel *TMFactory::createComment(QJsonObject object)
{
    return new CommentModel(object);
}

CommentModel *TMFactory::getCommentById(int id)
{
    foreach (CommentModel* c, m_commentsList) {
        if(c->getId()==id){
            return c;
        }
    }
    return nullptr;
}

void TMFactory::addComment(CommentModel *c)
{
    if(!ifCommentExist(c)){
        m_commentsList.append(c);
    }
}

void TMFactory::deleteComment(CommentModel *c)
{
    if(ifCommentExist(c)){
        m_commentsList.removeOne(c);
    }
    delete c;
}

bool TMFactory::ifCommentExist(CommentModel *c)
{
    foreach (CommentModel* cm, m_commentsList) {
        if(cm->getId()==c->getId()){
            return true;
        }
        return false;
    }
}

