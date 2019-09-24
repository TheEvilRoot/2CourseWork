#include "model/data/choicetest.h"

std::vector<QString> ChoiceTest::getAnswers() const {
    return mAnswers;
}

ChoiceTest::ChoiceTest(
        QString questionText,
        std::vector<QString> answers,
        size_t correctIndex
): BaseTest(ViewType::CHOICE, questionText, answers[correctIndex]), mAnswers(answers), mCorrectIndex(correctIndex) {}
