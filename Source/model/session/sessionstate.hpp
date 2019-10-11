#pragma once

#include "model/data/result.hpp"
#include "model/data/CEFR.hpp"

#include <QJsonObject>
#include <QString>
#include <deque>

class SessionState {
private:
    int mCorrect;
    int mWrong;
    QDateTime mTime;
    QString mResult;
    const CEFR *mCefrResult;
    std::deque<Result *> mTestResults;
    double mWordBasedCorrect;
    double mSentenceBasedCorrect;

    SessionState();
public:
    SessionState(QJsonObject obj);

    QJsonObject toJson();

    int getPoints();
    int getCorrect();
    int getWrong();
    QDateTime& getTime();
    QString getResultString();
    std::deque<Result *>& getTestResults();
    const CEFR* getCefr();
    double getWordBasedPercent();
    double getSentenceBasedPercent();

    size_t getCount();
    Result *at(size_t index);

    friend class Session;
};
