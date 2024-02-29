#ifndef TASKMODEL_H
#define TASKMODEL_H
#include "QDate"
class QString;
class QJsonObject;
class TaskModel
{
protected:
    int id; ///< id of task
    QString name; ///< Name of task
    int type; ///< type of task
    QString description; ///< task description
    int status; ///< status of task
    QDate startDate; ///< start date of task
    QDate finishDate; ///< finish date of task
    int projectId; ///< id of project where task is
    int userId; ///< id of user who works on task
public:
    TaskModel();
    virtual ~TaskModel();
    TaskModel(QJsonObject& obj);
    TaskModel(int id, QString name, int type, QString description, int status, QDate startDate, QDate finishDate, int projectId, int userId);
    int getId() const;
    QString getName() const;
    void setName(const QString &newName);
    int getType() const;
    void setType(int newType);
    QString getDescription() const;
    void setDescription(const QString &newDescription);
    int getStatus() const;
    void setStatus(int newStatus);
    QDate getStartDate() const;
    void setStartDate(const QDate &newStartDate);
    QDate getFinishDate() const;
    void setFinishDate(const QDate &newFinishDate);
    int getProjectId() const;
    void setProjectId(int newProjectId);
    int getUserId() const;
    void setUserId(int newUserId);
};

#endif // TASKMODEL_H
