#include "basetest.hpp"

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

int BaseTest::GLOBAL_ID = 0;

BaseTest::BaseTest(
    const ViewType *type,
    QString questionText,
    QString rightAnswer
    ): mId(GLOBAL_ID++), mType(type), mQuestionText(questionText), mRightAnswer(rightAnswer) { }

bool BaseTest::checkResult(QString answer) {
  return answer == mRightAnswer;
}

BaseTest::~BaseTest() { }

