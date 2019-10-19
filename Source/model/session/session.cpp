#include "session.hpp"
#include "model/data/choicetest.h"

#include <iostream>
#include <cmath>
#include <QDateTime>

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
        auto mSecs = abs(QDateTime::currentDateTime().msecsTo(mState->mTestResults.back()->mSolveTime));
        mState->mTestResults.back()->mSolveTime = QDateTime::fromMSecsSinceEpoch(mSecs);
    }
    if (!currentTest()) return;
    auto test = currentTest();

    mState->mTestResults.push_back(new Result(test->getQuestion(), test->getAnswer(), mPosition, QDateTime::currentDateTime()));
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

SessionState * Session::getState() {
    return mState;
}

void Session::generateConclusion() {
    int countOfWordBasedTests = 0;
    int correctWordBased = 0;
    int countOfSentenceBasedTests = 0;
    int correctSentenceBased = 0;

    for (auto result : mState->getTestResults()) {
       bool isSentenceBased = mTests[result->mIndex]->isSentenceBased();
       countOfWordBasedTests += !isSentenceBased;
       countOfSentenceBasedTests += isSentenceBased;
       correctSentenceBased += isSentenceBased && result->mPointsForTest != 0;
       correctWordBased += !isSentenceBased && result->mPointsForTest != 0;
    }

    double percentCorrectOfWordBased = (correctWordBased * 1.0) / countOfWordBasedTests;
    double percentCorrectOfSentenceBased = (correctSentenceBased * 1.0) / countOfSentenceBasedTests;

    // A1: WB > 0.05
    // A2: WB > 0.2 & SB > 0.05
    // B1: WB > 0.5 & SB > 0.2
    // B2 WB > 0.8 & SB > 0.5
    // C1: WB > 0.9 & SB > 0.8
    // C2: WB == 1 && SB > 0.9
    if (percentCorrectOfWordBased >= 0.999 && percentCorrectOfSentenceBased > 0.9) {
        // C2
        mState->mCefrResult = CEFR::C2;
    } else if (percentCorrectOfWordBased >= 0.9 && percentCorrectOfSentenceBased > 0.8) {
        // C1
        mState->mCefrResult = CEFR::C1;
    } else if (percentCorrectOfWordBased > 0.8 && percentCorrectOfSentenceBased > 0.5) {
        // B2
        mState->mCefrResult = CEFR::B2;
    } else if (percentCorrectOfWordBased > 0.5 && percentCorrectOfSentenceBased > 0.2) {
        // B1
        mState->mCefrResult = CEFR::B1;
    } else if (percentCorrectOfWordBased > 0.2 && percentCorrectOfSentenceBased > 0.05) {
        // A2
        mState->mCefrResult = CEFR::A2;
    } else if (percentCorrectOfWordBased > 0.05) {
        // A1
        mState->mCefrResult = CEFR::A1;
    } else {
        // Need more practice
        mState->mCefrResult = CEFR::NOTHING;
    }

    mState->mWordBasedCorrect = percentCorrectOfWordBased;
    mState->mSentenceBasedCorrect = percentCorrectOfSentenceBased;
}
