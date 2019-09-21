#ifndef BASETEST_HPP
#define BASETEST_HPP

#include <vector>
#include <string>

#include "viewtype.h"

class BaseTest {
private:
    ViewType *mType;
    std::string mQuestionText;
    std::string mRightAnswer;

public:
    BaseTest(std::string questionText,
             std::string rightAnswer);

    bool checkResult(std::string answer);

    ~BaseTest();
};

#endif
