#ifndef SESSION_HPP
#define SESSION_HPP

#include "model/data/basetest.hpp"

#include <vector>
#include <unordered_map>

class Session {
private:
  std::vector<BaseTest*> mTests;
  unsigned int mPosition;
  int mRight;
  int mWrong;
  int mPoints;
  bool mAttemptsMode;
  std::unordered_map<int, int> mLog;


public:
    Session(std::vector<BaseTest *> tests, bool attemptsMode = false);
    ~Session();

    void nextTest();
    void checkTest(bool result);
    int calculatePoints(int attempts, BaseTest *test);

    BaseTest * currentTest() const;
    int getTestsCount() const;
    int getPoints() const;
    int getRightAnswersCount() const;
    int getWrongAnswersCount() const;
    bool isAttemptsMode() const;
    bool isFinished();
};

#endif // SESSION_HPP
