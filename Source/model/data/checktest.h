#ifndef CHECKTEST_H
#define CHECKTEST_H

#include "model/data/basetest.hpp"

class CheckTest: public BaseTest {
private:
    std::vector<QString> mAnswers;
    size_t mCorrectIndex;
public:
    CheckTest(QString questionText,
              std::vector<QString> answers,
              uint correctIndex);
    std::vector<QString> getAnswers() const;
};

#endif // CHECKTEST_H
