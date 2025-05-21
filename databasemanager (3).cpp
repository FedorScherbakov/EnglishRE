#include "databasemanager.h"

DatabaseManager::DatabaseManager(const QString& dbName) : dbName(dbName) {
    initializeDatabase();
}

DatabaseManager::~DatabaseManager() {
    if (db.isOpen()) {
        db.close();
    }
}

bool DatabaseManager::initializeDatabase() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);

    if (!db.open()) {
        qDebug() << "Error opening database:" << db.lastError().text();
        return false;
    }

    createTables();
    return true;
}

void DatabaseManager::createTables() {
    QSqlQuery query;

    QString createGrammarTableSql = R"(
        CREATE TABLE IF NOT EXISTS grammar_questions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            level TEXT NOT NULL,
            question TEXT NOT NULL,
            answer TEXT NOT NULL
        )
    )";

    QString createTranslationTableSql = R"(
        CREATE TABLE IF NOT EXISTS translation_questions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            level TEXT NOT NULL,
            question_ru TEXT NOT NULL,
            answer_en TEXT NOT NULL
        )
    )";

    QString createExpressionTableSql = R"(
        CREATE TABLE IF NOT EXISTS expression_questions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            level TEXT NOT NULL,
            prompt TEXT NOT NULL,
            correct_answer TEXT NOT NULL
        )
    )";

    QString createProgressTableSql = R"(
        CREATE TABLE IF NOT EXISTS user_progress (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL,
            test_type TEXT NOT NULL,
            score INTEGER NOT NULL,
            level TEXT NOT NULL,
            date TEXT NOT NULL
        )
    )";
    QString createUsersTableSql = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL UNIQUE,
            password TEXT NOT NULL
        )
    )";

    if (!query.exec(createUsersTableSql)) {
        qDebug() << "Error creating users table:" << query.lastError().text();
    }

    if (!query.exec(createGrammarTableSql)) {
        qDebug() << "Error creating grammar_questions table:" << query.lastError().text();
    }

    if (!query.exec(createTranslationTableSql)) {
        qDebug() << "Error creating translation_questions table:" << query.lastError().text();
    }

    if (!query.exec(createExpressionTableSql)) {
        qDebug() << "Error creating expression_questions table:" << query.lastError().text();
    }

    if (!query.exec(createProgressTableSql)) {
        qDebug() << "Error creating user_progress table:" << query.lastError().text();
    }
}

bool DatabaseManager::saveUserProgress(const QString& username, const QString& testType, int score, const QString& level) {
    QSqlQuery query;
    query.prepare("INSERT INTO user_progress (username, test_type, score, level, date) "
                  "VALUES (:username, :test_type, :score, :level, datetime('now'))");
    query.bindValue(":username", username);
    query.bindValue(":test_type", testType);
    query.bindValue(":score", score);
    query.bindValue(":level", level);

    if (!query.exec()) {
        qDebug() << "Error saving user progress:" << query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::checkUserCredentials(const QString& username, const QString& password) {
    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "Error checking user credentials:" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        QString storedPassword = query.value(0).toString();
        return (storedPassword == password);
    }
    return false;
}

bool DatabaseManager::registerNewUser(const QString& username, const QString& password) {
    // Сначала проверяем, существует ли пользователь
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT username FROM users WHERE username = :username");
    checkQuery.bindValue(":username", username);

    if (!checkQuery.exec()) {
        qDebug() << "Error checking user existence:" << checkQuery.lastError().text();
        return false;
    }

    if (checkQuery.next()) {
        return false; // Пользователь уже существует
    }

    // Если пользователя нет, регистрируем нового
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO users (username, password) VALUES (:username, :password)");
    insertQuery.bindValue(":username", username);
    insertQuery.bindValue(":password", password);

    if (!insertQuery.exec()) {
        qDebug() << "Error registering new user:" << insertQuery.lastError().text();
        return false;
    }
    return true;
}

QVector<QStringList> DatabaseManager::getUserProgress(const QString& username) {
    QVector<QStringList> progress;
    QSqlQuery query;
    query.prepare("SELECT test_type, score, level, date FROM user_progress "
                  "WHERE username = :username ORDER BY date DESC");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "Error getting user progress:" << query.lastError().text();
        return progress;
    }

    while (query.next()) {
        QStringList record;
        record << query.value(0).toString(); // test_type
        record << query.value(1).toString(); // score
        record << query.value(2).toString(); // level
        record << query.value(3).toString(); // date
        progress.append(record);
    }

    return progress;
}

QVector<QStringList> DatabaseManager::getGrammarQuestions(const QString& level) {
    QVector<QStringList> questions;
    QString queryStr = "SELECT id, level, question, answer FROM grammar_questions";
    if (!level.isEmpty()) {
        queryStr += " WHERE level = :level";
    }

    QSqlQuery query;
    query.prepare(queryStr);
    if (!level.isEmpty()) {
        query.bindValue(":level", level);
    }

    if (!query.exec()) {
        qDebug() << "Error getting grammar questions:" << query.lastError().text();
        return questions;
    }

    while (query.next()) {
        QStringList question;
        question << query.value(0).toString();
        question << query.value(1).toString();
        question << query.value(2).toString();
        question << query.value(3).toString();
        questions.append(question);
    }

    return questions;
}

QVector<QStringList> DatabaseManager::getTranslationQuestions(const QString& level) {
    QVector<QStringList> questions;
    QString queryStr = "SELECT id, level, question_ru, answer_en FROM translation_questions";
    if (!level.isEmpty()) {
        queryStr += " WHERE level = :level";
    }

    QSqlQuery query;
    query.prepare(queryStr);
    if (!level.isEmpty()) {
        query.bindValue(":level", level);
    }

    if (!query.exec()) {
        qDebug() << "Error getting translation questions:" << query.lastError().text();
        return questions;
    }

    while (query.next()) {
        QStringList question;
        question << query.value(0).toString();
        question << query.value(1).toString();
        question << query.value(2).toString();
        question << query.value(3).toString();
        questions.append(question);
    }

    return questions;
}

QVector<QStringList> DatabaseManager::getExpressionQuestions(const QString& level) {
    QVector<QStringList> questions;
    QString queryStr = "SELECT id, level, prompt, correct_answer FROM expression_questions";
    if (!level.isEmpty()) {
        queryStr += " WHERE level = :level";
    }

    QSqlQuery query;
    query.prepare(queryStr);
    if (!level.isEmpty()) {
        query.bindValue(":level", level);
    }

    if (!query.exec()) {
        qDebug() << "Error getting expression questions:" << query.lastError().text();
        return questions;
    }

    while (query.next()) {
        QStringList question;
        question << query.value(0).toString();
        question << query.value(1).toString();
        question << query.value(2).toString();
        question << query.value(3).toString();
        questions.append(question);
    }

    return questions;
}
