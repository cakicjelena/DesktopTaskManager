#ifndef SESSION_H
#define SESSION_H
class ProjectModel;
class UserModel;
class TaskModel;
class Session
{
protected:
    static Session* m_instance;
     ProjectModel* m_project;
     UserModel* m_user;
     TaskModel* m_task;
public:
    static Session* getInstance();
    ProjectModel *project() const;
    void setProject(ProjectModel *newProject);

    UserModel *user() const;
    void setUser(UserModel *newUser);

    TaskModel *task() const;
    void setTask(TaskModel *newTask);

private:
    Session();

};

#endif // SESSION_H
