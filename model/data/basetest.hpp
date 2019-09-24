#ifndef BASETEST_HPP
#define BASETEST_HPP

#include <vector>
#include <QString>

#include "view/viewtype.h"

class BaseTest {
private:
    int mId;
    const ViewType *mType;
    QString mQuestionText;
    QString mRightAnswer;

public:

    static int GLOBAL_ID;

    BaseTest(const ViewType *type,
             QString questionText,
             QString rightAnswer);

    ~BaseTest();

    bool checkResult(QString answer);
    const ViewType* getType() const;
    QString getQuestion() const;
    QString getAnswer() const;
    int getId() const;
};

#endif
