#pragma once

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QPropertyAnimation>

class QFloatingWidget: public QWidget {

    Q_OBJECT

    Q_PROPERTY(float popupOpacity READ getPopupOpacity WRITE setPopupOpacity)

    void setPopupOpacity(float opacity);
    float getPopupOpacity() const;

public:
    explicit QFloatingWidget(QWidget *parent = nullptr);


protected:
        void paintEvent(QPaintEvent *e);

public slots:
    void setPopupText(const QString& text);
    void show();

private:
    QLabel label;
    QGridLayout layout;
    QPropertyAnimation animation;
    float popupOpacity;
};
