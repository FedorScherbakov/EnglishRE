#include "testapp.h"
#include "authdialog.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    if (!QSqlDatabase::isDriverAvailable("QSQLITE")) {
        QMessageBox::critical(nullptr, "Error",
            "SQLite driver not available. Please install Qt SQLite support.");
        return 1;
    }

    AuthDialog authDialog;
    while (true) {
        if (authDialog.exec() != QDialog::Accepted) {
            return 0;
        }

        QString username = authDialog.getUsername();
        if (username.isEmpty()) {
            QMessageBox::warning(nullptr, "Warning", "Please enter your name");
            continue;
        }

        break;
    }

    QString username = authDialog.getUsername();
    TestApp window(username);
    window.show();

    return app.exec();
}
