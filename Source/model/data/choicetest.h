#pragma once

#include "model/data/basetest.hpp"

class ChoiceTest: public BaseTest {
private:
    std::vector<QString> mAnswers;
    size_t mCorrectIndex;
public:
    ChoiceTest(QString questionText,
               std::vector<QString> answers,
               uint correctIndex);
    std::vector<QString> getAnswers() const;
};
