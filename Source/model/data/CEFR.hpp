#pragma once

#include <QString>
#include <vector>

class CEFR {
private:
    static std::vector<CEFR*> list;
    static int GLOBAL_ID;
    int mId;
    QString mName;

public:
    CEFR(QString name): mId(GLOBAL_ID++), mName(name) {
        list.push_back(this);
    }

    QString getName() const {
        return mName;
    }

    int getId() const {
        return mId;
    }

    static const CEFR* fromName(QString name) {
        for (auto cefr : list) {
            if (cefr && cefr->getName() == name) return cefr;
        }
        return NOTHING;
    }

    const static CEFR *NOTHING;
    const static CEFR *A1;
    const static CEFR *A2;
    const static CEFR *B1;
    const static CEFR *B2;
    const static CEFR *C1;
    const static CEFR *C2;
};

