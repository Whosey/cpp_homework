#ifndef USER_H
#define USER_H

#include <QJsonObject>
#include <QString>

class User
{
public:
    User();
    User(const QString& userId, const QString& username, const QString& password);

    QString getUserId() const;
    QString getUsername() const;
    QString getPassword() const;

    void setUserId(const QString& userId);
    void setUsername(const QString& username);
    void setPassword(const QString& password);

    bool checkPassword(const QString& inputPassword) const;
    void changePassword(const QString& newPassword);

    QJsonObject toJson() const;
    static User fromJson(const QJsonObject& obj);

private:
    QString userId;
    QString username;
    QString password;
};

#endif
