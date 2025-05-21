#ifndef LOGOWIDGET_H
#define LOGOWIDGET_H

#include <QWidget>

class LogoWidget : public QWidget {
    Q_OBJECT
public:
    explicit LogoWidget(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // LOGOWIDGET_H
