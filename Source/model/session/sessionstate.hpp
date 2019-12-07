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
    QDateTime mSolveTime;

    SessionState();
public:
    SessionState(const QJsonObject& obj);

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
    QDateTime& getSolveTime();
    void updateSolveTime();

    size_t getCount();
    Result *at(size_t index);

    friend class Session;
};
