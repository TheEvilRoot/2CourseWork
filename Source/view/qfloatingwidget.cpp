#include "qfloatingwidget.hpp"

#include <QPainter>
#include <QTimer>

QFloatingWidget::QFloatingWidget(QWidget *parent): QWidget(parent) {
    resize(200, 50);

    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);

    animation.setTargetObject(this);
    animation.setPropertyName("popupOpacity");
    animation.setDuration(700);

    label.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    layout.addWidget(&label, 0, 0);
    setLayout(&layout);
}

void QFloatingWidget::paintEvent(QPaintEvent *e) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRectF roundedRectDimensions;
    roundedRectDimensions.setX(rect().x() + 5);
    roundedRectDimensions.setY(rect().y() + 5);
    roundedRectDimensions.setWidth(rect().width() - 10);
    roundedRectDimensions.setHeight(rect().height() - 10);

    painter.setBrush(QBrush(Qt::lightGray));

    QPen pen;
    pen.setColor(Qt::gray);
    pen.setWidth(0);
    painter.setPen(pen);

    // Draw the popup body
    painter.drawRoundedRect(roundedRectDimensions, 15, 15);
}

void QFloatingWidget::setPopupText(const QString &text) {
    label.setText(text);
}

void QFloatingWidget::show() {
    animation.setStartValue(0);
    animation.setEndValue(1);
    QTimer::singleShot(2000, [=]() {
       if (!(popupOpacity < 1)) {
           animation.setStartValue(popupOpacity);
           animation.setEndValue(0);
           animation.start();
       }
    });
    setWindowOpacity(0.0);
    animation.start();
    QWidget::show();
}



void QFloatingWidget::setPopupOpacity(float opacity) {
    popupOpacity = opacity;

    setWindowOpacity(opacity);
}

float QFloatingWidget::getPopupOpacity() const {
    return popupOpacity;
}
