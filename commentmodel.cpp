#include "commentmodel.h"
#include "QJsonObject"
#include "qstring.h"
int CommentModel::getId() const
{
    return id;
}

QString CommentModel::getEmail() const
{
    return email;
}

void CommentModel::setEmail(const QString &newEmail)
{
    email = newEmail;
}

QString CommentModel::getComment() const
{
    return comment;
}

void CommentModel::setComment(const QString &newComment)
{
    comment = newComment;
}

int CommentModel::getTaskId() const
{
    return taskId;
}

void CommentModel::setTaskId(int newTaskId)
{
    taskId = newTaskId;
}

CommentModel::~CommentModel()
{

}

CommentModel::CommentModel(QJsonObject &obj)
{
    id= obj.take("id").toInt();
    taskId= obj.take("taskId").toInt();
    email = obj.take("email").toString();
    comment=obj.take("comment").toString();
}

CommentModel::CommentModel(int id, int taskId, QString email, QString comment)
{
    id=id;
    taskId=taskId;
    email=email;
    comment=comment;
}

CommentModel::CommentModel() {}
