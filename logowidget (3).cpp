#include "logowidget.h"
#include <QPainter>
#include <QPolygon>

LogoWidget::LogoWidget(QWidget *parent) : QWidget(parent) {
    setFixedSize(200, 200);
}

void LogoWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    // Draw perfect rhombus
    int size = qMin(width(), height()) - 20;
    int centerX = width() / 2;
    int centerY = height() / 2;

    QPolygon rhombus;
    rhombus << QPoint(centerX, centerY - size/2)
            << QPoint(centerX + size/2, centerY)
            << QPoint(centerX, centerY + size/2)
            << QPoint(centerX - size/2, centerY);

    painter.setBrush(QColor(240, 240, 240));
    painter.setPen(Qt::NoPen);
    painter.drawPolygon(rhombus);

    // Draw perfect cross
    painter.setPen(QPen(QColor(200, 0, 0), 8));
    painter.drawLine(centerX, centerY - size/2 + 10, centerX, centerY + size/2 - 10);
    painter.drawLine(centerX - size/2 + 10, centerY, centerX + size/2 - 10, centerY);

    // Draw centered "RE" text
    QFont font("Arial", 48, QFont::Bold);
    painter.setFont(font);
    painter.setPen(QColor(0, 0, 150));

    QFontMetrics fm(font);
    QString text = "RE";
    int textWidth = fm.horizontalAdvance(text);
    int textHeight = fm.height();

    painter.drawText(centerX - textWidth/2, centerY + textHeight/4, text);
}
