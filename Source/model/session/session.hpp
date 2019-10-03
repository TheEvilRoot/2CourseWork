#pragma once

#include "model/data/basetest.hpp"
#include "model/session/sessionstate.hpp"

#include <vector>

typedef std::vector<BaseTest *> TestList;

class Session {
private:
  TestList mTests;
  size_t mPosition;
  bool mAttemptsMode;
  SessionState *mState;

public:
    Session(TestList tests, bool attemptsMode = false);
    ~Session();

    void nextTest();
    bool submitTest(size_t index, QString answer);
    int calculatePoints(Result *result, BaseTest *test);
    void applyResult();

    BaseTest * currentTest() const;
    size_t getTestsCount() const;
    int getPoints() const;
    int getCorrectAnswersCount() const;
    int getWrongAnswersCount() const;
    bool isAttemptsMode() const;
    bool isFinished();
};
