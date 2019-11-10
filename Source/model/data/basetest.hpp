#pragma once

#include "view/viewtype.hpp"

#include <vector>
#include <QString>

class BaseTest {
protected:
    int mId;
    const ViewType *mType;
    QString mQuestionText;
    QString mRightAnswer;
    bool mIsSentenceBased;
    char _padding[7];

public:

    static int GLOBAL_ID;

    BaseTest(const ViewType *type,
             QString questionText,
             QString rightAnswer,
             bool isSentenceBased = false): 
             mId(GLOBAL_ID++),
             mType(type),
             mQuestionText(questionText),
             mRightAnswer(rightAnswer),
             mIsSentenceBased(isSentenceBased) { }

    virtual ~BaseTest() = default;

    bool checkResult(QString answer) {
        return answer.toLower() == mRightAnswer.toLower();
    }

    const ViewType* getType() const {
        return mType;
    }

    QString getQuestion() const {
        return mQuestionText;
    }

    QString getAnswer() const {
        return mRightAnswer;
    }

    int getId() const {
        return mId;
    }

    bool isSentenceBased() {
        return mIsSentenceBased;
    }

    void setSentenceBased() {
        mIsSentenceBased = true;
    }
};
