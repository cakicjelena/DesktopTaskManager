#ifndef TMFACTORY_H
#define TMFACTORY_H
#include "qvector.h"
class QJsonObject;
class ProjectModel;
class UserModel;
class TaskModel;
class CommentModel;

class TMFactory
{
protected:
    QVector<ProjectModel*> m_projectsList;
    QVector<TaskModel*> m_tasksList;
    QVector<UserModel*> m_usersList;
    QVector<CommentModel*> m_commentsList;
public:
    TMFactory();
    virtual ~TMFactory();
public:
    ProjectModel* createProject(QJsonObject object);
    TaskModel* createTask(QJsonObject object);
    ProjectModel* getProjectById(int id);
    ProjectModel* getProjectByName(QString name);
    TaskModel* getTaskById(int id);
    void addProject(ProjectModel* p);
    void addTask(TaskModel* t);
    void deleteProject(ProjectModel* p);
    void deleteTask(TaskModel* t);
    bool ifProjectExist(ProjectModel* p);
    bool ifTaskExist(TaskModel* t);
    UserModel* createUser(QJsonObject object);
    UserModel* getUserById(int id);
    UserModel* getUserByEmail(QString email);
    void addUser(UserModel* u);
    void deleteUser(UserModel* u);
    bool ifUserExist(UserModel* u);
    CommentModel* createComment(QJsonObject object);
    CommentModel* getCommentById(int id);
    QVector<CommentModel*> getAllCommentsOfTask(int taskId);
    void addComment(CommentModel* c);
    void deleteComment(CommentModel* c);
    bool ifCommentExist(CommentModel* c);

    QVector<ProjectModel *> projectsList() const;
    QVector<TaskModel *> tasksList() const;
    QVector<UserModel *> usersList() const;
    QVector<CommentModel*> commentsList() const;
};

#endif // TMFACTORY_H
