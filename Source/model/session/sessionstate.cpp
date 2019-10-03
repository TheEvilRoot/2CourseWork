#include "sessionstate.hpp"

SessionState::SessionState(): mCorrect(0), mWrong(0), mResult("") {}

int SessionState::getPoints() {
    int res = 0;
    for (auto result : mTestResults) res+=result->mPointsForTest;
    return res;
}
