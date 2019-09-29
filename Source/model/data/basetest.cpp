#include "basetest.hpp"

#include <iostream>

const ViewType* BaseTest::getType() const {
  return mType;
}

QString BaseTest::getQuestion() const {
  return mQuestionText;
}

QString BaseTest::getAnswer() const {
  return mRightAnswer;
}

int BaseTest::getId() const {
    return mId;
}

bool BaseTest::checkResult(QString answer) {
  return answer == mRightAnswer;
}

int BaseTest::GLOBAL_ID = 0;

BaseTest::BaseTest(
    const ViewType *type,
    QString questionText,
    QString rightAnswer
): mId(GLOBAL_ID++), mType(type), mQuestionText(questionText), mRightAnswer(rightAnswer) { }

BaseTest::~BaseTest() {
    std::cout << "Destructing " << mId << " test\n";
}


