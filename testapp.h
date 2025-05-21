#ifndef TESTAPP_H
#define TESTAPP_H

#include <QWidget>
#include <QStackedWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QVector>
#include <QStringList>
#include <QTableWidget>
#include "databasemanager.h"

class TestApp : public QWidget {
    Q_OBJECT
public:
    explicit TestApp(const QString &username, QWidget *parent = nullptr);

private slots:
    void showNextQuestion();
    void showPreviousQuestion();
    void showDetailedResults();
    void checkAnswer();
    void startTest();
    void showSettings();
    void goBack();
    void changeTheme();
    void showResults();
    void showHistory();

private:
    void setupUI();
    void createMainPage();
    void createTestPage();
    void createSettingsPage();
    void createResultsPage();
    void createHistoryPage();
    void loadQuestions();
    void shuffleQuestions();
    void showCurrentQuestion();
    QString calculateLevel(int correctAnswers);
    QString getLevelDescription(const QString& level);
    void loadHistory();

    DatabaseManager dbManager;
    QStackedWidget *stackedWidget;
    QStringList userAnswers;
    QWidget *mainPage;
    QWidget *testPage;
    QWidget *settingsPage;
    QWidget *resultsPage;
    QWidget *historyPage;
    QTextEdit *questionTextEdit;
    QLineEdit *answerLineEdit;
    QLabel *questionCounterLabel;
    QLabel *resultsLabel;
    QLabel *welcomeLabel;
    QTextEdit *historyTextEdit;
    QTableWidget *historyTable;
    QVector<QStringList> allQuestions;
    QVector<QStringList> testQuestions;
    QString currentUsername;
    int currentQuestionIndex = 0;
    int currentTheme = 0;
    int correctAnswersCount = 0;
    const int TOTAL_TEST_QUESTIONS = 18;
};

#endif // TESTAPP_H
