#pragma once

#include "model/data/result.hpp"

#include <QJsonObject>
#include <QString>
#include <deque>

class SessionState {
private:
    int mCorrect;
    int mWrong;
    QString mResult;
    std::deque<Result *> mTestResults;

    SessionState();
public:
    SessionState(QJsonObject obj);

    QJsonObject toJson();

    int getPoints();
    int getCorrect();
    int getWrong();
    QString getResultString();

    size_t getCount();
    Result *at(size_t index);

    friend class Session;
};
