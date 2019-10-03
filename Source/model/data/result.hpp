#pragma once

#include <QString>
#include <vector>

class Result {
public:
    QString mQuestion;
    QString mAnswer;
    std::vector<QString> mUserAnswers;
    size_t mIndex;
    int mAttempts;
    int mPointsForTest;
    long mSolveTime;

    Result(QString question, QString answer, size_t index):
        mQuestion(question),
        mAnswer(answer),
        mIndex(index),
        mAttempts(0),
        mPointsForTest(0),
        mSolveTime(0) {}

};
