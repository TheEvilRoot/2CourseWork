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
    ~BaseTest();

    bool checkResult(std::string answer);
    const ViewType* getType() const;
    std::string getQuestion() const;
    std::string getAnswer() const;
};

#endif
