#include "session.hpp"
#include "model/data/choicetest.h"

#include <iostream>
#include <cmath>

Session::Session(TestList tests, bool attemptsMode):
    mTests(tests),
    mPosition(0),
    mAttemptsMode(attemptsMode),
    mState(new SessionState) {
    applyResult();
    std::cout << "Initializing session " << attemptsMode << "\n";
}

Session::~Session() {
    std::cerr << "Session descruction initiated!!!!!!\n";
    for (size_t i = 0; i < mTests.size(); i++) if (mTests[i]) delete mTests[i];
}

bool Session::isAttemptsMode() const {
    return mAttemptsMode;
}

int Session::getCorrectAnswersCount() const {
    return mState->mCorrect;
}

int Session::getWrongAnswersCount() const {
    return mState->mWrong;
}

int Session::getPoints() const{
    return mState->getPoints();
}

size_t Session::getTestsCount() const {
    return mTests.size();
}

void Session::applyResult() {
    if (!mState->mTestResults.empty()) {
        mState->mTestResults.back()->mSolveTime = time(nullptr) - mState->mTestResults.back()->mSolveTime;
    }
    if (!currentTest()) return;
    auto test = currentTest();

    mState->mTestResults.push_back(new Result(test->getQuestion(), test->getAnswer(), mPosition));
}

void Session::nextTest() {
    mPosition++;
}
/**
 * @brief Session::submitTest
 * @param result - r
 * @param answer
 */
bool Session::submitTest(size_t index, QString answer) {
    auto test = currentTest();
    if (test == nullptr) return false;
    auto isCorrect = false;

    if (test->getType() == ViewType::CHOICE || test->getType() == ViewType::CHECK) {
        auto choiceTest = dynamic_cast<ChoiceTest *>(test);
        isCorrect = choiceTest->checkAnswerByIndex(index);
        answer = choiceTest->getAnswers()[index];
    } else {
        isCorrect = test->checkResult(answer);
    }

    auto result = mState->mTestResults.back();
    result->mUserAnswers.push_back(answer);
    result->mAttempts++;
    result->mPointsForTest = isCorrect ? calculatePoints(result, test) : 0;

    if (isCorrect) {
        mState->mCorrect++;
    } else {
        mState->mWrong++;
    }

    if (isCorrect || !mAttemptsMode) {
        nextTest();
        applyResult();
    }
    return isCorrect;
}

BaseTest* Session::currentTest() const {
  if (mPosition < mTests.size()){
    return mTests[mPosition];
  } else {
    return nullptr;
  }
}

bool Session::isFinished() {
  return mPosition >= mTests.size();
}

int Session::calculatePoints(Result *result, BaseTest *test) {
    int modifyer = std::min(result->mAttempts, 3);
    if (test->getType() == ViewType::CHOICE) {
        return 30 / modifyer;
    } else if (test->getType() == ViewType::INPUT) {
        return 60 / modifyer;
    } else if (test->getType() == ViewType::CHECK) {
        return 33 / modifyer;
    } else {
        return 27 / modifyer;
    }
}
