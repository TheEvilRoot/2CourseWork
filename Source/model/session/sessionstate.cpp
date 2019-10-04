#include "sessionstate.hpp"

#include <ctime>

SessionState::SessionState():  mCorrect(0), mWrong(0), mTime(time(nullptr)), mResult(""){}

SessionState::SessionState(QJsonObject obj):
    mCorrect(obj.value("correct").toInt()),
    mWrong(obj.value("wrong").toInt()),
    mTime(obj.value("time").toString().toLong()),
    mResult(obj.value("result").toString()) {
    auto resultsArray = obj.value("results").toArray();
    for (auto res : resultsArray) {
        auto result = new Result(res.toObject());
        mTestResults.push_back(result);
    }
}

QJsonObject SessionState::toJson() {
    QJsonObject obj;
    obj.insert("correct", QJsonValue(mCorrect));
    obj.insert("wrong", QJsonValue(mWrong));
    obj.insert("result", QJsonValue(mResult));
    obj.insert("time", QString::number(mTime));

    QJsonArray resultsArray;
    for (auto result : mTestResults) {
        resultsArray.push_back(result->toJson());
    }

    obj.insert("results", resultsArray);
    return obj;
}

int SessionState::getPoints() {
    int res = 0;
    for (auto result : mTestResults) res+=result->mPointsForTest;
    return res;
}

int SessionState::getCorrect() {
    return mCorrect;
}

int SessionState::getWrong() {
    return mWrong;
}

long SessionState::getTime() {
    return mTime;
}

size_t SessionState::getCount() {
    return mTestResults.size();
}

Result* SessionState::at(size_t index) {
    if (index >= getCount()) return nullptr;
    return mTestResults.at(index);
}

QString SessionState::getResultString() {
    return mResult;
}

std::deque<Result *>& SessionState::getTestResults() {
    return mTestResults;
}
