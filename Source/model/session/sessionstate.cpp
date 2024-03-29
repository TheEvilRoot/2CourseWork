#include "sessionstate.hpp"

SessionState::SessionState():
    mCorrect(0),
    mWrong(0),
    mTime(QDateTime::currentDateTime()),
    mResult(""),
    mCefrResult(CEFR::NOTHING),
    mWordBasedCorrect(0),
    mSentenceBasedCorrect(0),
    mSolveTime(QDateTime::fromMSecsSinceEpoch(0)){}

SessionState::SessionState(const QJsonObject& obj):
    mCorrect(obj.value("correct").toInt()),
    mWrong(obj.value("wrong").toInt()),
    mTime(QDateTime::fromMSecsSinceEpoch(obj.value("time").toString().toLongLong())),
    mResult(obj.value("result").toString()),
    mCefrResult(CEFR::fromName(obj.value("cefr").toString())),
    mWordBasedCorrect(obj.value("wbcorrect").toDouble()),
    mSentenceBasedCorrect(obj.value("sbcorrect").toDouble()),
    mSolveTime(QDateTime::fromMSecsSinceEpoch(0)){
    auto resultsArray = obj.value("results").toArray();
    for (auto res : resultsArray) {
        auto result = new Result(res.toObject());
        mTestResults.push_back(result);
    }
    updateSolveTime();
}

QJsonObject SessionState::toJson() {
    QJsonObject obj;
    obj.insert("correct", QJsonValue(mCorrect));
    obj.insert("wrong", QJsonValue(mWrong));
    obj.insert("result", QJsonValue(mResult));
    obj.insert("time", QString::number(mTime.toMSecsSinceEpoch()));
    obj.insert("cefr", mCefrResult->getName());
    obj.insert("wbcorrect", mWordBasedCorrect);
    obj.insert("sbcorrect", mSentenceBasedCorrect);

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

QDateTime& SessionState::getTime() {
    return mTime;
}

size_t SessionState::getCount() {
    return mTestResults.size();
}

const CEFR* SessionState::getCefr() {
    return mCefrResult;
}

Result* SessionState::at(size_t index) {
    if (index >= getCount()) return nullptr;
    return mTestResults.at(index);
}

QString SessionState::getResultString() {
    return mResult;
}

QDateTime& SessionState::getSolveTime() {
    return mSolveTime;
}

std::deque<Result *>& SessionState::getTestResults() {
    return mTestResults;
}

double SessionState::getWordBasedPercent() {
    return mWordBasedCorrect;
}

double SessionState::getSentenceBasedPercent() {
    return mSentenceBasedCorrect;
}

void SessionState::updateSolveTime() {
    mSolveTime = QDateTime::fromMSecsSinceEpoch(0);
    for (auto res : mTestResults) {
        mSolveTime = mSolveTime.addMSecs(res->mSolveTime.toMSecsSinceEpoch());
    }
}
