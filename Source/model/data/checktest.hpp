#pragma once

#include "model/data/choicetest.hpp"

class CheckTest: public ChoiceTest {
public:
    CheckTest(
        QString questionText,
        std::vector<QString> answers,
        uint correctIndex
    ): ChoiceTest(questionText, answers, correctIndex) {
        this->mType = ViewType::CHECK;
    }
};
