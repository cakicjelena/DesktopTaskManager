#include "taskmodel.h"
#include "QJsonObject"
#include "QJsonArray"
#include "commentmodel.h"
int TaskModel::getId() const
{
    return id;
}

QString TaskModel::getName() const
{
    return name;
}

void TaskModel::setName(const QString &newName)
{
    name = newName;
}

int TaskModel::getType() const
{
    return type;
}

void TaskModel::setType(int newType)
{
    type = newType;
}

QString TaskModel::getDescription() const
{
    return description;
}

void TaskModel::setDescription(const QString &newDescription)
{
    description = newDescription;
}

int TaskModel::getStatus() const
{
    return status;
}

void TaskModel::setStatus(int newStatus)
{
    status = newStatus;
}

QDate TaskModel::getStartDate() const
{
    return startDate;
}

void TaskModel::setStartDate(const QDate &newStartDate)
{
    startDate = newStartDate;
}

QDate TaskModel::getFinishDate() const
{
    return finishDate;
}

void TaskModel::setFinishDate(const QDate &newFinishDate)
{
    finishDate = newFinishDate;
}

int TaskModel::getProjectId() const
{
    return projectId;
}

void TaskModel::setProjectId(int newProjectId)
{
    projectId = newProjectId;
}

int TaskModel::getUserId() const
{
    return userId;
}

void TaskModel::setUserId(int newUserId)
{
    userId = newUserId;
}

QVector<CommentModel *> TaskModel::getComments() const
{
    return m_comments;
}

void TaskModel::setComments(const QVector<CommentModel *> &newComments)
{
    m_comments = newComments;
}

TaskModel::TaskModel() {}

TaskModel::~TaskModel()
{

}

TaskModel::TaskModel(QJsonObject& obj)
{
    id= obj.take("id").toInt();
    name = obj.take("name").toString();
    type= obj.take("type").toInt();
    description=obj.take("description").toString();
    status= obj.take("status").toInt();
    startDate= QDate::fromString(obj.take("startDate").toString(), "yyyy-MM-dd");
    finishDate=QDate::fromString(obj.take("finishDate").toString(), "yyyy-MM-dd");
    projectId= obj.take("projectId").toInt();
    userId= obj.take("userId").toInt();
    QJsonArray comments= obj.take("comments").toArray();
    if(!comments.isEmpty()){
        for (int i = 0; i < comments.size(); ++i) {
            QJsonObject obj = comments[i].toObject();
            m_comments.append(new CommentModel(obj));
        }
    }
    else{
        qDebug()<<"Invalid json array";
    }
}

TaskModel::TaskModel(int id, QString name, int type, QString description, int status, QDate startDate, QDate finishDate, int projectId, int userId, QVector<CommentModel*> m_comments)
{
    id=id;
    name=name;
    type=type;
    description=description;
    status=status;
    startDate=startDate;
    finishDate=finishDate;
    projectId=projectId;
    userId=userId;
    m_comments=m_comments;
}
