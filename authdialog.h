#ifndef AUTHDIALOG_H
#define AUTHDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>

class AuthDialog : public QDialog {
    Q_OBJECT
public:
    explicit AuthDialog(QWidget *parent = nullptr);
    QString getUsername() const;

private slots:
    void attemptLogin();
    void attemptRegistration();

private:
    void showError(const QString &message);
    void showSuccess(const QString &message);

    QLineEdit *emailInput;
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    QLabel *statusLabel;
};

#endif // AUTHDIALOG_H
