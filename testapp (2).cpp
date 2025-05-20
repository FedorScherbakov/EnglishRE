#include "testapp.h"
#include "logowidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QFont>
#include <QTime>
#include <random>
#include <algorithm>

TestApp::TestApp(const QString &username, QWidget *parent)
    : QWidget(parent), dbManager(), currentUsername(username)
{
    setupUI();
    loadQuestions();
}

void TestApp::setupUI() {
    setWindowTitle("English Test App");
    setFixedSize(800, 600);

    stackedWidget = new QStackedWidget(this);
    createMainPage();
    createTestPage();
    createSettingsPage();
    createResultsPage();
    createHistoryPage();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(stackedWidget);
    setLayout(mainLayout);
}

void TestApp::createMainPage() {
    mainPage = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(mainPage);

    LogoWidget *logoWidget = new LogoWidget();
    layout->addWidget(logoWidget);
    layout->setAlignment(logoWidget, Qt::AlignCenter);

    welcomeLabel = new QLabel(QString("Welcome, %1!").arg(currentUsername));
    welcomeLabel->setAlignment(Qt::AlignCenter);
    QFont welcomeFont = welcomeLabel->font();
    welcomeFont.setPointSize(18);
    welcomeLabel->setFont(welcomeFont);
    layout->addWidget(welcomeLabel);

    QHBoxLayout *topLayout = new QHBoxLayout();
    QPushButton *settingsButton = new QPushButton("Settings");
    settingsButton->setStyleSheet("QPushButton { border-radius: 20px; padding: 5px 15px; background-color: #D3D3D3; border: 1px solid #808080;} QPushButton:hover { background-color: #ADD8E6; }");
    connect(settingsButton, &QPushButton::clicked, this, &TestApp::showSettings);
    topLayout->addWidget(settingsButton);

    QPushButton *historyButton = new QPushButton("My Results");
    historyButton->setStyleSheet("QPushButton { border-radius: 20px; padding: 5px 15px; background-color: #D3D3D3; border: 1px solid #808080;} QPushButton:hover { background-color: #ADD8E6; }");
    connect(historyButton, &QPushButton::clicked, this, &TestApp::showHistory);
    topLayout->addWidget(historyButton);

    layout->addLayout(topLayout);

    QVBoxLayout *centerLayout = new QVBoxLayout();
    QPushButton *startButton = new QPushButton("Start Test");
    startButton->setStyleSheet("font-size: 24px; padding: 20px; border-radius: 20px; background-color: #D3D3D3; border: 1px solid #808080;} QPushButton:hover { background-color: #ADD8E6; }");
    connect(startButton, &QPushButton::clicked, this, &TestApp::startTest);
    centerLayout->addWidget(startButton);
    layout->addLayout(centerLayout);

    stackedWidget->addWidget(mainPage);
}

void TestApp::createTestPage() {
    testPage = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(testPage);

    questionTextEdit = new QTextEdit();
    questionTextEdit->setReadOnly(true);
    questionTextEdit->setStyleSheet("background-color: white; border-radius: 5px; padding: 10px;");
    questionTextEdit->setFixedHeight(150);
    layout->addWidget(questionTextEdit);

    QHBoxLayout *answerLayout = new QHBoxLayout();
    answerLineEdit = new QLineEdit();
    answerLineEdit->setStyleSheet("background-color: white; border-radius: 5px; padding: 5px;");
    answerLineEdit->setFixedHeight(40);
    answerLayout->addWidget(answerLineEdit);

    QPushButton *prevButton = new QPushButton("◀");
    prevButton->setFont(QFont("Arial", 16));
    prevButton->setStyleSheet("QPushButton { background-color: #D3D3D3; border: 1px solid #808080; border-radius: 20px; padding: 5px; } QPushButton:hover { background-color: #ADD8E6; }");
    prevButton->setFixedSize(40, 40);
    connect(prevButton, &QPushButton::clicked, this, &TestApp::showPreviousQuestion);
    answerLayout->addWidget(prevButton);

    QPushButton *nextButton = new QPushButton("▶");
    nextButton->setFont(QFont("Arial", 16));
    nextButton->setStyleSheet("QPushButton { background-color: #D3D3D3; border: 1px solid #808080; border-radius: 20px; padding: 5px; } QPushButton:hover { background-color: #ADD8E6; }");
    nextButton->setFixedSize(40, 40);
    connect(nextButton, &QPushButton::clicked, this, &TestApp::showNextQuestion);
    answerLayout->addWidget(nextButton);

    QPushButton *checkButton = new QPushButton("Check");
    checkButton->setStyleSheet("QPushButton { background-color: #D3D3D3; border: 1px solid #808080; border-radius: 20px; padding: 5px 15px; } QPushButton:hover { background-color: #ADD8E6; }");
    connect(checkButton, &QPushButton::clicked, this, &TestApp::checkAnswer);
    answerLayout->addWidget(checkButton);

    layout->addLayout(answerLayout);

    questionCounterLabel = new QLabel();
    questionCounterLabel->setAlignment(Qt::AlignCenter);
    QFont counterFont = questionCounterLabel->font();
    counterFont.setPointSize(14);
    questionCounterLabel->setFont(counterFont);
    layout->addWidget(questionCounterLabel);

    stackedWidget->addWidget(testPage);
}

void TestApp::createSettingsPage() {
    settingsPage = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(settingsPage);
    layout->setAlignment(Qt::AlignCenter);

    QLabel *settingsLabel = new QLabel("Settings");
    settingsLabel->setAlignment(Qt::AlignCenter);
    QFont labelFont = settingsLabel->font();
    labelFont.setPointSize(20);
    settingsLabel->setFont(labelFont);
    layout->addWidget(settingsLabel);

    QPushButton *themeButton = new QPushButton("Change Theme");
    themeButton->setStyleSheet("QPushButton { border-radius: 20px; padding: 10px 20px; background-color: #D3D3D3; border: 1px solid #808080;} QPushButton:hover { background-color: #ADD8E6; }");
    connect(themeButton, &QPushButton::clicked, this, &TestApp::changeTheme);
    layout->addWidget(themeButton);

    QPushButton *backButton = new QPushButton("Back to Main Menu");
    backButton->setStyleSheet("QPushButton { border-radius: 20px; padding: 10px 20px; background-color: #D3D3D3; border: 1px solid #808080;} QPushButton:hover { background-color: #ADD8E6; }");
    connect(backButton, &QPushButton::clicked, this, &TestApp::goBack);
    layout->addWidget(backButton);

    stackedWidget->addWidget(settingsPage);
}

void TestApp::createResultsPage() {
    resultsPage = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(resultsPage);
    layout->setAlignment(Qt::AlignCenter);

    resultsLabel = new QLabel();
    resultsLabel->setAlignment(Qt::AlignCenter);
    resultsLabel->setWordWrap(true);
    QFont resultFont = resultsLabel->font();
    resultFont.setPointSize(16);
    resultsLabel->setFont(resultFont);
    layout->addWidget(resultsLabel);

    QPushButton *restartButton = new QPushButton("Restart Test");
    restartButton->setStyleSheet("font-size: 18px; padding: 15px; border-radius: 20px; background-color: #D3D3D3; border: 1px solid #808080;} QPushButton:hover { background-color: #ADD8E6; }");
    connect(restartButton, &QPushButton::clicked, this, &TestApp::startTest);
    layout->addWidget(restartButton);

    QPushButton *mainMenuButton = new QPushButton("Main Menu");
    mainMenuButton->setStyleSheet("font-size: 18px; padding: 15px; border-radius: 20px; background-color: #D3D3D3; border: 1px solid #808080;} QPushButton:hover { background-color: #ADD8E6; }");
    connect(mainMenuButton, &QPushButton::clicked, this, &TestApp::goBack);
    layout->addWidget(mainMenuButton);

    stackedWidget->addWidget(resultsPage);
}

void TestApp::createHistoryPage() {
    historyPage = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(historyPage);

    QLabel *historyLabel = new QLabel("Your Test History");
    historyLabel->setAlignment(Qt::AlignCenter);
    QFont labelFont = historyLabel->font();
    labelFont.setPointSize(20);
    historyLabel->setFont(labelFont);
    layout->addWidget(historyLabel);

    historyTextEdit = new QTextEdit();
    historyTextEdit->setReadOnly(true);
    historyTextEdit->setStyleSheet("background-color: white; border-radius: 5px; padding: 10px;");
    layout->addWidget(historyTextEdit);

    QPushButton *backButton = new QPushButton("Back to Main Menu");
    backButton->setStyleSheet("QPushButton { border-radius: 20px; padding: 10px 20px; background-color: #D3D3D3; border: 1px solid #808080;} QPushButton:hover { background-color: #ADD8E6; }");
    connect(backButton, &QPushButton::clicked, this, &TestApp::goBack);
    layout->addWidget(backButton);

    stackedWidget->addWidget(historyPage);
}

void TestApp::loadQuestions() {
    allQuestions = dbManager.getGrammarQuestions();
    shuffleQuestions();
    testQuestions = allQuestions.mid(0, TOTAL_TEST_QUESTIONS);
}

void TestApp::shuffleQuestions() {
    auto rng = std::default_random_engine(std::random_device{}());
    std::shuffle(allQuestions.begin(), allQuestions.end(), rng);
}

void TestApp::showCurrentQuestion() {
    if (currentQuestionIndex >= 0 && currentQuestionIndex < testQuestions.size()) {
        QStringList question = testQuestions[currentQuestionIndex];
        questionTextEdit->setText(question[2]);
        answerLineEdit->clear();
        questionCounterLabel->setText(QString("Question %1/%2").arg(currentQuestionIndex + 1).arg(testQuestions.size()));
    }
}

void TestApp::showNextQuestion() {
    if (currentQuestionIndex < testQuestions.size() - 1) {
        currentQuestionIndex++;
        showCurrentQuestion();
    } else {
        showResults();
    }
}

void TestApp::showPreviousQuestion() {
    if (currentQuestionIndex > 0) {
        currentQuestionIndex--;
        showCurrentQuestion();
    }
}

void TestApp::checkAnswer() {
    QString userAnswer = answerLineEdit->text().trimmed();
    QString correctAnswer = testQuestions[currentQuestionIndex][3];

    // Сохраняем ответ пользователя
    if (currentQuestionIndex < userAnswers.size()) {
        userAnswers[currentQuestionIndex] = userAnswer;
    } else {
        // Если currentQuestionIndex выходит за пределы userAnswers,
        // увеличиваем размер вектора и добавляем ответ
        userAnswers.append(userAnswer);
    }

    if (userAnswer.compare(correctAnswer, Qt::CaseInsensitive) == 0) {
        QMessageBox::information(this, "Result", "Correct!");
        correctAnswersCount++;
    } else {
        QMessageBox::information(this, "Result",
            QString("Incorrect. The correct answer is: %1").arg(correctAnswer));
    }
}

void TestApp::startTest() {
    shuffleQuestions();
    testQuestions = allQuestions.mid(0, TOTAL_TEST_QUESTIONS);
    currentQuestionIndex = 0;
    correctAnswersCount = 0;
    userAnswers.clear();
       for (int i = 0; i < TOTAL_TEST_QUESTIONS; ++i) {
            userAnswers.append(""); // Заполняем вектор пустыми строками
        }

    stackedWidget->setCurrentWidget(testPage);
    showCurrentQuestion();
}

void TestApp::showSettings() {
    stackedWidget->setCurrentWidget(settingsPage);
}

void TestApp::goBack() {
    stackedWidget->setCurrentWidget(mainPage);
}

void TestApp::changeTheme() {
    currentTheme = (currentTheme + 1) % 3;

    switch(currentTheme) {
        case 0: // Pink
            setStyleSheet("QWidget { background-color: #FFB6C1; color: #000000; }"
                         "QPushButton { background-color: #D3D3D3; color: #000000; border: 1px solid #808080; padding: 5px; border-radius: 20px; }"
                         "QPushButton:hover { background-color: #ADD8E6; }"
                         "QTextEdit, QLineEdit { background-color: #FFFFFF; border-radius: 5px; }");
            break;
        case 1: // Dark
            setStyleSheet("QWidget { background-color: #343a40; color: #e9ecef; }"
                         "QPushButton { background-color: #495057; color: #fff; border: none; padding: 5px; border-radius: 20px; }"
                         "QPushButton:hover { background-color: #6c757d; }"
                         "QTextEdit, QLineEdit { background-color: #212529; color: #e9ecef; border-radius: 5px; }");
            break;
        case 2: // White
            setStyleSheet("QWidget { background-color: #FFFFFF; color: #000000; }"
                         "QPushButton { background-color: #D3D3D3; color: #000000; border: 1px solid #808080; padding: 5px; border-radius: 20px; }"
                         "QPushButton:hover { background-color: #ADD8E6; }"
                         "QTextEdit, QLineEdit { background-color: #F8F9FA; border-radius: 5px; }");
            break;
    }
}

void TestApp::showResults() {
    QString level = calculateLevel(correctAnswersCount);
    dbManager.saveUserProgress(currentUsername, "Grammar Test", correctAnswersCount, level);

    // Собираем статистику по вопросам
    QString resultText = QString("<h2>Test Completed, %1!</h2>"
                               "<p>You answered <b>%2</b> out of <b>%3</b> questions correctly.</p>"
                               "<p>Your English level is: <b>%4</b></p>"
                               "<p>%5</p>"
                               "<h3>Detailed Results:</h3>"
                               "<table border='1' cellpadding='5' style='border-collapse: collapse; width: 100%;'>"
                               "<tr style='background-color: #f2f2f2;'>"
                               "<th>#</th><th>Question</th><th>Your Answer</th><th>Correct Answer</th><th>Result</th>"
                               "</tr>")
                               .arg(currentUsername)
                               .arg(correctAnswersCount)
                               .arg(TOTAL_TEST_QUESTIONS)
                               .arg(level)
                               .arg(getLevelDescription(level));

    // Добавляем информацию по каждому вопросу
    for (int i = 0; i < testQuestions.size(); ++i) {
        QString question = testQuestions[i][2];
        QString correctAnswer = testQuestions[i][3];
        QString userAnswer = (i < userAnswers.size()) ? userAnswers[i] : "No answer";
        bool isCorrect = (userAnswer.compare(correctAnswer, Qt::CaseInsensitive) == 0);

        // Определяем цвет строки в зависимости от правильности ответа
        QString rowColor = isCorrect ? "#e6ffe6" : "#ffe6e6";
        QString result = isCorrect ?
                         "<span style='color:green; font-weight:bold;'>✓ Correct</span>" :
                         "<span style='color:red; font-weight:bold;'>✗ Incorrect</span>";

        resultText += QString("<tr style='background-color: %1;'>"
                             "<td>%2</td><td>%3</td><td>%4</td><td>%5</td><td>%6</td>"
                             "</tr>")
                             .arg(rowColor)
                             .arg(i + 1)
                             .arg(question)
                             .arg(userAnswer)
                             .arg(correctAnswer)
                             .arg(result);
    }

    resultText += "</table>";

    // Добавляем сравнение с предыдущими результатами
    QVector<QStringList> history = dbManager.getUserProgress(currentUsername);
    if (history.size() > 1) {
        int previousScore = history[1][1].toInt();
        int difference = correctAnswersCount - previousScore;
        QString trend;

        if (difference > 0) trend = QString("↑ Improved by %1").arg(difference);
        else if (difference < 0) trend = QString("↓ Decreased by %1").arg(-difference);
        else trend = "→ No change";

        resultText += QString("<p>Compared to your last attempt: <b>%1</b></p>").arg(trend);
    }

    resultText += "<p><i>Your result has been saved.</i></p>";
    resultsLabel->setText(resultText);
    stackedWidget->setCurrentWidget(resultsPage);
}
void TestApp::showHistory() {
    loadHistory();
    stackedWidget->setCurrentWidget(historyPage);
}

void TestApp::loadHistory() {
    QVector<QStringList> history = dbManager.getUserProgress(currentUsername);
    QString historyText = "<h2>Your Test History</h2><table border='1' cellpadding='5'><tr><th>Test Type</th><th>Score</th><th>Level</th><th>Date</th></tr>";

    if (history.isEmpty()) {
        historyText += "<tr><td colspan='4' align='center'>No test results found</td></tr>";
    } else {
        for (const auto &record : history) {
            historyText += QString("<tr><td>%1</td><td>%2/%3</td><td>%4</td><td>%5</td></tr>")
                              .arg(record[0])
                              .arg(record[1])
                              .arg(TOTAL_TEST_QUESTIONS)
                              .arg(record[2])
                              .arg(record[3]);
        }
    }

    historyText += "</table>";
    historyTextEdit->setText(historyText);
}

QString TestApp::calculateLevel(int correctAnswers) {
    double percentage = (double)correctAnswers / TOTAL_TEST_QUESTIONS * 100;

    if (percentage >= 90) return "C2 (Proficient)";
    else if (percentage >= 75) return "C1 (Advanced)";
    else if (percentage >= 60) return "B2 (Upper Intermediate)";
    else if (percentage >= 45) return "B1 (Intermediate)";
    else if (percentage >= 30) return "A2 (Pre-Intermediate)";
    else return "A1 (Beginner)";
}

QString TestApp::getLevelDescription(const QString& level) {
    if (level.startsWith("A1")) return "You're at beginner level. Keep practicing basic vocabulary and grammar.";
    else if (level.startsWith("A2")) return "You're at pre-intermediate level. Work on expanding your vocabulary and mastering basic grammar.";
    else if (level.startsWith("B1")) return "You're at intermediate level. Focus on fluency and more complex grammar structures.";
    else if (level.startsWith("B2")) return "You're at upper-intermediate level. Work on nuance and more advanced vocabulary.";
    else if (level.startsWith("C1")) return "You're at advanced level. Focus on mastering subtle language differences.";
    else return "You're at proficient level. Your English is excellent!";
}
