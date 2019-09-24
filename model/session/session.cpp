#include "session.hpp"

#include <iostream>
#include <cmath>

Session::Session(bool attemptsMode):
    mTests({new BaseTest(ViewType::CHOICE, "ФЫВ Yeah!", "ФЫВ"), new BaseTest(ViewType::INPUT, "ФЫв is great?", "фыв")}),
    mPosition(0),
    mRight(0),
    mWrong(0),
    mPoints(0),
    mAttemptsMode(attemptsMode){
    std::cout << "Initializing session " << attemptsMode << "\n";
}

bool Session::isAttemptsMode() const {
    return mAttemptsMode;
}

int Session::getRightAnswersCount() const {
    return mRight;
}

int Session::getWrongAnswersCount() const {
    return mWrong;
}

int Session::getPoints() const{
    return mPoints;
}

int Session::getTestsCount() const {
    return (int) mTests.size();
}

Session::~Session() {
    std::cerr << "Session descruction initiated!!!!!!\n";
}

void Session::nextTest() {
  mPosition++;
}

void Session::checkTest(bool result) {
    auto test = currentTest();
    if (test == nullptr) return;

    int oldAttempts = mLog.count(test->getId()) > 0 ? mLog.at(test->getId()) : 0;
    mLog.insert_or_assign(test->getId(), ++oldAttempts);

    if (result) {
        mPoints += calculatePoints(oldAttempts, test);
        mRight++;
    } else {
        mWrong++;
    }
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

int Session::calculatePoints(int attempts, BaseTest *test) {
    int modifyer = std::min(attempts, 3);
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
