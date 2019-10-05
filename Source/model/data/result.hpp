#pragma once

#include <QString>
#include <vector>
#include <QDateTime>
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
    QDateTime mSolveTime;

    Result(QString question, QString answer, size_t index, QDateTime dt):
        mQuestion(question),
        mAnswer(answer),
        mIndex(index),
        mAttempts(0),
        mPointsForTest(0),
        mSolveTime(dt){}

    Result(QJsonObject obj):
        mQuestion(obj.value("question").toString()),
        mAnswer(obj.value("answer").toString()),
        mIndex(obj.value("index").toString().toULongLong()),
        mAttempts(obj.value("attempts").toInt()),
        mPointsForTest(obj.value("points").toInt()),
        mSolveTime(QDateTime::fromMSecsSinceEpoch(obj.value("time").toString().toLongLong()))
    {
        auto userAnswers = obj.value("answers").toArray();
        for (auto ans : userAnswers) {
            mUserAnswers.push_back(ans.toString());
        }
    }

    QString getJoinedAnswers(char del) {
        QString res;
        for (size_t i = 0; i < mUserAnswers.size(); i++) {
            res += mUserAnswers[i];
            if (i < mUserAnswers.size() - 1) res += del;
        }
        return res;
    }

    QJsonObject toJson() {
        QJsonObject obj;
        obj.insert("question", mQuestion);
        obj.insert("answer", mAnswer);
        obj.insert("index", QString::number(mIndex));
        obj.insert("attempts", mAttempts);
        obj.insert("points", mPointsForTest);
        obj.insert("time", QString::number(mSolveTime.toMSecsSinceEpoch()));

        QJsonArray answersArray;
        for (auto ans : mUserAnswers) {
            answersArray.push_back(ans);
        }
        obj.insert("answers", answersArray);
        return obj;
    }

};
