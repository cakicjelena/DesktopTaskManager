#include "usermodel.h"
#include "QJsonObject"
QString UserModel::getFirst_name() const
{
    return first_name;
}

void UserModel::setFirst_name(const QString &newFirst_name)
{
    first_name = newFirst_name;
}

QString UserModel::getLast_name() const
{
    return last_name;
}

void UserModel::setLast_name(const QString &newLast_name)
{
    last_name = newLast_name;
}

QString UserModel::getPassword() const
{
    return password;
}

void UserModel::setPassword(const QString &newPassword)
{
    password = newPassword;
}

bool UserModel::getSex() const
{
    return sex;
}

void UserModel::setSex(bool newSex)
{
    sex = newSex;
}

QDate UserModel::getBirthDate() const
{
    return birthDate;
}

void UserModel::setBirthDate(const QDate &newBirthDate)
{
    birthDate = newBirthDate;
}

bool UserModel::getIs_active() const
{
    return is_active;
}

void UserModel::setIs_active(bool newIs_active)
{
    is_active = newIs_active;
}

bool UserModel::getIs_superuser() const
{
    return is_superuser;
}

void UserModel::setIs_superuser(bool newIs_superuser)
{
    is_superuser = newIs_superuser;
}

QString UserModel::getEmail() const
{
    return email;
}

void UserModel::setEmail(const QString &newEmail)
{
    email = newEmail;
}

int UserModel::getId() const
{
    return id;
}

void UserModel::setId(int newId)
{
    id = newId;
}

UserModel::UserModel() {}

UserModel::~UserModel()
{

}

UserModel::UserModel(QJsonObject& obj)
{
    id= obj.take("id").toInt();
    first_name = obj.take("first_name").toString();
    last_name= obj.take("last_name").toString();
    email=obj.take("email").toString();
    password=obj.take("password").toString();
    sex=obj.take("sex").toBool();
    birthDate=QDate::fromString(obj.take("birthDate").toString(), "yyyy-MM-dd");
    is_active=obj.take("is_active").toBool();
    is_superuser=obj.take("is_superuser").toBool();
}

UserModel::UserModel(int id, QString first_name, QString last_name, QString email, QString password, bool sex, QDate birthDate, bool is_active, bool is_superuser)
{
    id=id;
    first_name=first_name;
    last_name=last_name;
    email=email;
    password=password;
    sex=sex;
    birthDate=birthDate;
    is_active=is_active;
    is_superuser=is_superuser;
}
