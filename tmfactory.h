#ifndef TMFACTORY_H
#define TMFACTORY_H
#include "qvector.h"
class QJsonObject;
class ProjectModel;
class UserModel;
class TaskModel;

class TMFactory
{
protected:
    QVector<ProjectModel*> m_projectsList;
    QVector<TaskModel*> m_tasksList;
    QVector<UserModel*> m_usersList;
public:
    TMFactory();
    virtual ~TMFactory();
public:
    ProjectModel* createProject(QJsonObject object);
    TaskModel* createTask(QJsonObject object);
    ProjectModel* getProjectById(int id);
    TaskModel* getTaskById(int id);
    void addProject(ProjectModel* p);
    void addTask(TaskModel* t);
    void deleteProject(ProjectModel* p);
    void deleteTask(TaskModel* t);
    bool ifProjectExist(ProjectModel* p);
    bool ifTaskExist(TaskModel* t);
    UserModel* createUser(QJsonObject object);
    UserModel* getUserById(int id);
    void addUser(UserModel* u);
    void deleteUser(UserModel* u);
    bool ifUserExist(UserModel* u);

};

#endif // TMFACTORY_H
