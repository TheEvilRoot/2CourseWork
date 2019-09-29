#pragma once

#include "view/viewtype.h"

#include <vector>
#include <QString>

class BaseTest {
protected:
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
