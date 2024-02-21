#include "ProjectModel.h"
#include "QJsonObject"
int ProjectModel::getId() const
{
    return id;
}

QString ProjectModel::getName() const
{
    return name;
}

void ProjectModel::setName(const QString &newName)
{
    name = newName;
}

QDate ProjectModel::getCreateDate() const
{
    return createDate;
}

void ProjectModel::setCreateDate(const QDate &newCreateDate)
{
    createDate = newCreateDate;
}

QDate ProjectModel::getDeadlineDate() const
{
    return deadlineDate;
}

void ProjectModel::setDeadlineDate(const QDate &newDeadlineDate)
{
    deadlineDate = newDeadlineDate;
}

QString ProjectModel::getDescription() const
{
    return description;
}

void ProjectModel::setDescription(const QString &newDescription)
{
    description = newDescription;
}

int ProjectModel::getProjectManagerId() const
{
    return projectManagerId;
}

void ProjectModel::setProjectManagerId(int newProjectManagerId)
{
    projectManagerId = newProjectManagerId;
}

ProjectModel::ProjectModel() {}

ProjectModel::~ProjectModel()
{

}

ProjectModel::ProjectModel(QJsonObject &obj)
{
    id= obj.take("id").toInt();
    name = obj.take("name").toString();
    createDate= QDate::fromString(obj.take("createDate").toString(), "yyyy-MM-dd");
    deadlineDate=QDate::fromString(obj.take("deadlineDate").toString(), "yyyy-MM-dd");
    description=obj.take("description").toString();
    projectManagerId=obj.take("projectManagerId").toInt();
}

ProjectModel::ProjectModel(int id, QString name, QDate createDate, QDate deadlineDate, QString description, int projectManagerId)
{
    id=id;
    name=name;
    createDate=createDate;
    deadlineDate=deadlineDate;
    description=description;
    projectManagerId=projectManagerId;
}
