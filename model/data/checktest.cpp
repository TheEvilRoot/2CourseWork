#include "model/data/checktest.h"

std::vector<QString> CheckTest::getAnswers() const {
    return mAnswers;
}

CheckTest::CheckTest(
        QString questionText,
        std::vector<QString> answers,
        uint correctIndex
): BaseTest(ViewType::CHECK, questionText, answers[correctIndex]), mAnswers(answers), mCorrectIndex(correctIndex) {}
