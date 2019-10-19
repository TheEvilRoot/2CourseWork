#pragma once

#include "model/data/basetest.hpp"
#include "model/session/sessionstate.hpp"

#include <vector>

typedef std::vector<BaseTest *> TestList;

class Session {
private:
  TestList mTests;
  size_t mPosition;
  int mMaxAttempts;
  int mMagicConstant;
  SessionState *mState;

public:
    Session(TestList tests, int maxAttempts = 0);
    ~Session();

    void nextTest();
    int submitTest(size_t index, QString answer);
    int calculatePoints(Result *result, BaseTest *test);
    void applyResult();

    BaseTest * currentTest() const;
    size_t getTestsCount() const;
    int getPoints() const;
    int getCorrectAnswersCount() const;
    int getWrongAnswersCount() const;
    bool isAttemptsMode() const;
    bool isFinished();
    size_t getTestPosition() const;
    void generateConclusion();
    int getMaxAttempts() const;
    int getMagic() const;

    SessionState * getState(); // Should be removed!
};
