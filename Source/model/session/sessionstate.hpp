#pragma once

#include "model/data/result.hpp"

#include <QString>
#include <deque>

class SessionState {
private:
    int mCorrect;
    int mWrong;
    QString mResult;
    std::deque<Result *> mTestResults;

    SessionState();

    int getPoints();

    friend class Session;
};
