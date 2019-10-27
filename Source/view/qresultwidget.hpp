#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class QResultWidget: public QWidget {

    Q_OBJECT

public:
    QResultWidget();

private:
    QGridLayout *gridLayout;
    QPushButton *nextBtn;
    QPushButton *prevBtn;
    QVBoxLayout *contentVIew;
    QLabel *questionLabel;
    QLabel *userAnswerLabel;
    QLabel *correctLabel;
    QLabel *titleLabel;
};
