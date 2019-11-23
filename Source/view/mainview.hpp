#pragma once

#include "viewtype.hpp"
#include "model/data/basetest.hpp"
#include "model/session/sessionstate.hpp"

class MainView { // TODO: Solve warning
public:
    virtual ~MainView() = default;

    virtual bool presentView(const ViewType *type) = 0;
    virtual void showLoading() = 0;
    virtual void hideLoading() = 0;
    virtual void showMessage(QString message, bool enablePopup = false) = 0;
    virtual void disableContent() = 0;
    virtual void enableContent() = 0;
    virtual void askSession() = 0;
    virtual void initiateError(bool fatal = false, QString message = "Unexpected error.") = 0;

    virtual void setupChoiceScreen(QString question, std::vector<QString> answers) = 0;
    virtual void setupInputScreen(QString question, QString prefill = "") = 0;
    virtual void setupCheckScreen(QString question, std::vector<QString> answers) = 0;
    virtual void setupResultScreen(SessionState *state) = 0;
    virtual void setupMenuScreen(SessionState *currentState) = 0;

    virtual void setupHistoryList(std::deque<SessionState *> &states) = 0;
    virtual void setupHistoryDetails(SessionState *state) = 0;

    virtual void setTipWords(std::pair<QString, QString>& words) = 0;

    virtual void setTestTitle(const ViewType *type, size_t index, size_t count) = 0;
};

