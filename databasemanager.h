#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVector>
#include <QStringList>
#include <QDebug>

class DatabaseManager {
public:
    DatabaseManager(const QString& dbName = "english_test3.db");
    ~DatabaseManager();

    bool initializeDatabase();
    QVector<QStringList> getGrammarQuestions(const QString& level = "");
    QVector<QStringList> getTranslationQuestions(const QString& level = "");
    QVector<QStringList> getExpressionQuestions(const QString& level = "");
    bool saveUserProgress(const QString& username, const QString& testType, int score, const QString& level);
    QVector<QStringList> getUserProgress(const QString& username);
    bool checkUserCredentials(const QString& username, const QString& password);
    bool registerNewUser(const QString& username, const QString& password);

private:
    void createTables();
    QSqlDatabase db;
    QString dbName;
};

#endif // DATABASEMANAGER_H
