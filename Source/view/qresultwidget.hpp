#pragma once

#include "model/session/sessionstate.hpp"

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class QResultWidget: public QWidget {

    Q_OBJECT

public:
    QResultWidget(SessionState *state = nullptr);

signals:
    void next();
    void prev();
    void stateUpdate(SessionState *state);

public slots:
    void setState(SessionState *state);
    void updateView();
    void setNext();
    void setPrev();
private:
    QGridLayout *gridLayout;
    QPushButton *nextBtn;
    QPushButton *prevBtn;
    QVBoxLayout *contentVIew;
    QLabel *questionLabel;
    QLabel *userAnswerLabel;
    QLabel *correctLabel;
    QLabel *titleLabel;

    QString mEmptyAnswer;
    size_t mIndex;

    SessionState *mState;
};
