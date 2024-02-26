#ifndef COMMENTMODEL_H
#define COMMENTMODEL_H
#include "qstring.h"
class QJsonObject;
class CommentModel
{
protected:
    int id; ///< id of comment
    QString email; ///< Email of user leaving comment
    QString comment; ///< Comment
public:
    virtual ~CommentModel();
    CommentModel(QJsonObject& obj);
    CommentModel(int id, QString email, QString comment);
    CommentModel();
    int getId() const;
    QString getEmail() const;
    void setEmail(const QString &newEmail);
    QString getComment() const;
    void setComment(const QString &newComment);
};

#endif // COMMENTMODEL_H
