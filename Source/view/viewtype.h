#pragma once

#include <string>

class ViewType {
private:
    std::string mViewName;
    int mViewId;

    static int GLOBAL_ID;

    ViewType(std::string name): mViewName(name), mViewId(GLOBAL_ID++) {}
public:
    static const ViewType* MENU;
    static const ViewType* CHOICE;
    static const ViewType* INPUT;
    static const ViewType* CHECK;
    static const ViewType* RESULT;
    static const ViewType* HISTORY;
    static const ViewType* HELP;
    static const ViewType* ABOUT;

    bool operator==(ViewType &a) {
        return this->mViewId == a.mViewId;
    }

    bool operator!=(ViewType &a) {
        return !(this->operator==(a));
    }

    std::string getViewName() const {
        return mViewName;
    }

    int getViewId() const {
        return mViewId;
    }
};
