#include "basetest.hpp"

const ViewType* BaseTest::getType() const {
  return mType;
}

std::string BaseTest::getQuestion() const {
  return mQuestionText;
}

std::string BaseTest::getAnswer() const {
  return mRightAnswer;
}

BaseTest::BaseTest(
    const ViewType *type,
    std::string questionText,
    std::string rightAnswer
    ): mType(type), mQuestionText(questionText), mRightAnswer(rightAnswer) { }

bool BaseTest::checkResult(std::string answer) {
  return answer == mRightAnswer;
}

BaseTest::~BaseTest() { }

