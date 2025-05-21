#include "authdialog.h"
#include "databasemanager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QFont>

AuthDialog::AuthDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Authorization");
    setFixedSize(600, 500);
    setStyleSheet("QDialog { background-color: #f8f9fa; }");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(20);

    // Logo and title
    QLabel *titleLabel = new QLabel("English Test App");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    mainLayout->addWidget(titleLabel);

    // Input fields
    QVBoxLayout *inputLayout = new QVBoxLayout();
    inputLayout->setSpacing(10);

    emailInput = new QLineEdit();
    emailInput->setPlaceholderText("Email");
    emailInput->setStyleSheet("padding: 10px; border-radius: 5px; border: 1px solid #ced4da;");
    inputLayout->addWidget(emailInput);

    usernameInput = new QLineEdit();
    usernameInput->setPlaceholderText("Username");
    usernameInput->setStyleSheet("padding: 10px; border-radius: 5px; border: 1px solid #ced4da;");
    inputLayout->addWidget(usernameInput);

    passwordInput = new QLineEdit();
    passwordInput->setPlaceholderText("Password");
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setStyleSheet("padding: 10px; border-radius: 5px; border: 1px solid #ced4da;");
    inputLayout->addWidget(passwordInput);

    mainLayout->addLayout(inputLayout);

    // Status label
    statusLabel = new QLabel();
    statusLabel->setWordWrap(true);
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("padding: 5px;");
    mainLayout->addWidget(statusLabel);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(15);

    QPushButton *loginButton = new QPushButton("Login");
    loginButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #007bff;"
        "   color: white;"
        "   padding: 10px 15px;"
        "   border-radius: 5px;"
        "   border: none;"
        "}"
        "QPushButton:hover {"
        "   background-color: #0069d9;"
        "}"
    );
    connect(loginButton, &QPushButton::clicked, this, &AuthDialog::attemptLogin);
    buttonLayout->addWidget(loginButton);

    QPushButton *registerButton = new QPushButton("Register");
    registerButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #28a745;"
        "   color: white;"
        "   padding: 10px 15px;"
        "   border-radius: 5px;"
        "   border: none;"
        "}"
        "QPushButton:hover {"
        "   background-color: #218838;"
        "}"
    );
    connect(registerButton, &QPushButton::clicked, this, &AuthDialog::attemptRegistration);
    buttonLayout->addWidget(registerButton);

    mainLayout->addLayout(buttonLayout);
}

QString AuthDialog::getUsername() const {
    return usernameInput->text().trimmed();
}

void AuthDialog::attemptLogin() {
    QString username = getUsername();
    QString password = passwordInput->text().trimmed();

    if (username.isEmpty() || password.isEmpty()) {
        showError("Please enter both username and password");
        return;
    }

    DatabaseManager dbManager;
    if (dbManager.checkUserCredentials(username, password)) {
        accept();
    } else {
        showError("Invalid username or password");
    }
}

void AuthDialog::attemptRegistration() {
    QString username = getUsername();
    QString password = passwordInput->text().trimmed();

    if (username.isEmpty() || password.isEmpty()) {
        showError("Please enter both username and password");
        return;
    }

    if (password.length() < 4) {
        showError("Password must be at least 4 characters long");
        return;
    }

    DatabaseManager dbManager;
    if (dbManager.registerNewUser(username, password)) {
        showSuccess("Registration successful! You can now login with your credentials.");
    } else {
        showError("Username already taken. Please choose a different username.");
    }
}

void AuthDialog::showError(const QString &message) {
    statusLabel->setText("<span style='color: #dc3545;'>" + message + "</span>");
}

void AuthDialog::showSuccess(const QString &message) {
    statusLabel->setText("<span style='color: #28a745;'>" + message + "</span>");
}
