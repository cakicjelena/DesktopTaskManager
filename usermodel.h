#ifndef USERMODEL_H
#define USERMODEL_H
#include "QDate"
class QString;
class QJsonObject;
class UserModel
{
protected:
   int id; ///< id
   QString first_name; ///< First name of user
   QString last_name; ///< Last name of user
   QString email; ///< User email
   QString password; ///< Password
   bool sex; ///< Male or female
   QDate birthDate; ///<Birth date of user
   bool is_active; ///< Indicate if user is active
   bool is_superuser; ///< Indicate if user is admin

public:
    UserModel();
    virtual ~UserModel();
    UserModel(QJsonObject& obj);
    UserModel(int id, QString first_name, QString last_name, QString email, QString password, bool sex, QDate birthDate, bool is_active, bool is_superuser);

    QString getFirst_name() const;
    void setFirst_name(const QString &newFirst_name);
    QString getLast_name() const;
    void setLast_name(const QString &newLast_name);
    QString getPassword() const;
    void setPassword(const QString &newPassword);
    bool getSex() const;
    void setSex(bool newSex);
    QDate getBirthDate() const;
    void setBirthDate(const QDate &newBirthDate);
    bool getIs_active() const;
    void setIs_active(bool newIs_active);
    bool getIs_superuser() const;
    void setIs_superuser(bool newIs_superuser);
    QString getEmail() const;
    void setEmail(const QString &newEmail);
    int getId() const;
    void setId(int newId);
};

#endif // USERMODEL_H
