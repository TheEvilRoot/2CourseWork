#ifndef BASETEST_HPP
#define BASETEST_HPP

#include <vector>
#include <string>

#include "viewtype.h"

class BaseTest {
private:
    const ViewType *mType;
    std::string mQuestionText;
    std::string mRightAnswer;

public:
    BaseTest(const ViewType *type,
             std::string questionText,
             std::string rightAnswer);

    bool checkResult(std::string answer);

    ~BaseTest();

    const ViewType* getType() const;
};

#endif
