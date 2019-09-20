#include "basetest.hpp"

BaseTest::BaseTest(
        std::string questionText,
        std::string rightAnswer
): mQuestionText(questionText), mRightAnswer(rightAnswer) { }

BaseTest::~BaseTest() { }

