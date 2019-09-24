#ifndef BASETEST_HPP
#define BASETEST_HPP

#include "view/viewtype.h"

#include <vector>
#include <QString>

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

    virtual ~BaseTest();

    bool checkResult(QString answer);
    const ViewType* getType() const;
    QString getQuestion() const;
    QString getAnswer() const;
    int getId() const;
};

#endif
