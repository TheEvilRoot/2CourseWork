#include "sessionstate.hpp"

SessionState::SessionState(): mCorrect(0), mWrong(0), mResult("") {}

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
