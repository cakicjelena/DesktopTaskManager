#ifndef ProjectModel_H
#define ProjectModel_H
#include "QDate"
class QString;
class QJsonObject;
class ProjectModel
{
protected:
    int id; ///< id
    QString name; ///< Name of project
    QDate createDate; ///< Create date of project
    QDate deadlineDate; ///< Deadline date of project
    QString description; ///< Description of project
    int projectManagerId; ///< Who is manager on project

public:
    ProjectModel();
    virtual ~ProjectModel();
    ProjectModel(QJsonObject& obj);
    ProjectModel(int id, QString name, QDate createDate, QDate deadlineDate, QString description, int projectManagerId);
    int getId() const;
    QString getName() const;
    void setName(const QString &newName);
    QDate getCreateDate() const;
    void setCreateDate(const QDate &newCreateDate);
    QDate getDeadlineDate() const;
    void setDeadlineDate(const QDate &newDeadlineDate);
    QString getDescription() const;
    void setDescription(const QString &newDescription);
    int getProjectManagerId() const;
    void setProjectManagerId(int newProjectManagerId);
};

#endif // ProjectModel_H
