#pragma once

#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonArray>

class Result {
public:
    QString mQuestion;
    QString mAnswer;
    QStringList mUserAnswers;
    size_t mIndex;
    int mAttempts;
    int mPointsForTest;
    QDateTime mSolveTime;

    Result(QString question, QString answer, size_t index, QDateTime dt);

    Result(const QJsonObject& obj);

    QString getJoinedAnswers(char del);

    QJsonObject toJson();

protected:
    QStringList getUserAnswers(const QJsonObject& obj) {
        QStringList list;
        auto userAnswers = obj.value("answers").toArray();
        for (const auto& ans : userAnswers) {
            list.push_back(ans.toString());
        }
        return list;
    }
};
