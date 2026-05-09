#include "User.h"

User::User()
{
}

User::User(const QString& userId, const QString& username, const QString& password)
    : userId(userId), username(username), password(password)
{
}

QString User::getUserId() const
{
    return userId;
}

QString User::getUsername() const
{
    return username;
}

QString User::getPassword() const
{
    return password;
}

void User::setUserId(const QString& userId)
{
    this->userId = userId;
}

void User::setUsername(const QString& username)
{
    this->username = username;
}

void User::setPassword(const QString& password)
{
    this->password = password;
}

bool User::checkPassword(const QString& inputPassword) const
{
    return password == inputPassword;
}

void User::changePassword(const QString& newPassword)
{
    password = newPassword;
}

QJsonObject User::toJson() const
{
    QJsonObject obj;
    obj["userId"] = userId;
    obj["username"] = username;
    // 课程设计演示中为简化逻辑使用明文密码；真实项目必须使用哈希加盐保存密码。
    obj["password"] = password;
    return obj;
}

User User::fromJson(const QJsonObject& obj)
{
    return User(obj["userId"].toString(),
                obj["username"].toString(),
                obj["password"].toString());
}
