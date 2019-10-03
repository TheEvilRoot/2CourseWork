#pragma once

#include "viewtype.h"
#include "model/data/basetest.hpp"
#include "model/session/sessionstate.hpp"

class MainView { // TODO: Solve warning
public:
    virtual ~MainView() {}

    virtual bool presentView(const ViewType *type) = 0;
    virtual void showLoading() = 0;
    virtual void hideLoading() = 0;
    virtual void showMessage(QString message) = 0;
    virtual void disableContent() = 0;
    virtual void enableContent() = 0;
    virtual void askSession() = 0;

    virtual void setupChoiceScreen(QString question, std::vector<QString> answers) = 0;
    virtual void setupInputScreen(QString question, QString prefill = "") = 0;
    virtual void setupCheckScreen(QString question, std::vector<QString> answers) = 0;
    virtual void setupResultScreen(SessionState *state) = 0;
    virtual void setupMenuScreen(bool hasActiveSession, int points) = 0;
};
