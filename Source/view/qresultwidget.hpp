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
    QGridLayout *gridLayout_12;
    QPushButton *resTestNext;
    QPushButton *resTestPrev;
    QVBoxLayout *verticalLayout_2;
    QLabel *resTestQuestionLabel;
    QLabel *resTestUserAnswerLabel;
    QLabel *resTestCorrectLabel;
    QLabel *resTestTitle;
};
