#pragma once

#include "model/data/choicetest.h"

class CheckTest: public ChoiceTest {
public:
    CheckTest(QString questionText,
              std::vector<QString> answers,
              uint correctIndex);
};
