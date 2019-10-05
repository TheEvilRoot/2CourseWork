#include "model/data/checktest.h"

CheckTest::CheckTest(
        QString questionText,
        std::vector<QString> answers,
        uint correctIndex
): ChoiceTest(questionText, answers, correctIndex) {
    this->mType = ViewType::CHECK;
}
