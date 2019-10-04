#pragma once

#include <QString>
#include <vector>
#include <QJsonObject>
#include <QJsonArray>

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

    Result(QJsonObject obj):
        mQuestion(obj.value("question").toString()),
        mAnswer(obj.value("answer").toString()),
        mIndex(obj.value("index").toString().toULongLong()),
        mAttempts(obj.value("attempts").toInt()),
        mPointsForTest(obj.value("points").toInt()),
        mSolveTime(obj.value("time").toString().toLong())
    {
        auto userAnswers = obj.value("answers").toArray();
        for (auto ans : userAnswers) {
            mUserAnswers.push_back(ans.toString());
        }
    }

    QJsonObject toJson() {
        QJsonObject obj;
        obj.insert("question", mQuestion);
        obj.insert("answer", mAnswer);
        obj.insert("index", static_cast<int>(mIndex));
        obj.insert("attempts", mAttempts);
        obj.insert("points", mPointsForTest);
        obj.insert("time", QString::number(mSolveTime));

        QJsonArray answersArray;
        for (auto ans : mUserAnswers) {
            answersArray.push_back(ans);
        }
        obj.insert("answers", answersArray);
        return obj;
    }

};
