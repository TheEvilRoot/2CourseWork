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
    bool checkAnswerByIndex(size_t index);
    std::vector<QString> getAnswers() const;
};
