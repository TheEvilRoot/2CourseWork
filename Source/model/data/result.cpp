#include "model/data/result.hpp"

Result::Result(QString question,
               QString answer,
               size_t index,
               QDateTime dt):
    mQuestion(question),
    mAnswer(answer),
    mUserAnswers(),
    mIndex(index),
    mAttempts(0),
    mPointsForTest(0),
    mSolveTime(dt) { }

Result::Result(const QJsonObject& obj):
    mQuestion(obj.value("question").toString()),
    mAnswer(obj.value("answer").toString()),
    mUserAnswers(getUserAnswers(obj)),
    mIndex(obj.value("index").toString().toULongLong()),
    mAttempts(obj.value("attempts").toInt()),
    mPointsForTest(obj.value("points").toInt()),
    mSolveTime(QDateTime::fromMSecsSinceEpoch(obj.value("time").toString().toLongLong())) {
}

QString Result::getJoinedAnswers(char del) {
    QString res;
    for (int i = 0; i < mUserAnswers.size(); i++) {
        res += mUserAnswers[i];
        if (i < mUserAnswers.size() - 1) res += del;
    }
    return res;
}


QJsonObject Result::toJson() {
    QJsonObject obj;
    obj.insert("question", mQuestion);
    obj.insert("answer", mAnswer);
    obj.insert("index", QString::number(mIndex));
    obj.insert("attempts", mAttempts);
    obj.insert("points", mPointsForTest);
    obj.insert("time", QString::number(mSolveTime.toMSecsSinceEpoch()));

    QJsonArray answersArray;
    for (const auto& ans : mUserAnswers) {
        answersArray.push_back(ans);
    }

    obj.insert("answers", answersArray);
    return obj;
}
