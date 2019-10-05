#include "mainpresenter.hpp"
#include "model/loader/wordsfileloader.h"
#include "model/loader/storeworker.hpp"
#include "model/data/choicetest.h"
#include "model/data/checktest.h"

#include <QApplication>

MainPresenter::MainPresenter(Model *model, MainView *view): mModel(model), mView(view) {
  Q_ASSERT(model);
  Q_ASSERT(view);
}

/**
 * @brief MainPresenter::proceedAnswer
 * @param answer - answer string, empty string if test has index-based check (like Choice or Check)
 * @param index - index of user answer. 0 if test has only string based check (like Input)
 *
 * Calls on user answer to current test.
 * Should check answer and display result to screen.
 */
void MainPresenter::proceedAnswer(QString answer, size_t index) {
    if (!mModel->getSession()) {
        mView->showMessage("Session is not initialized");
        return;
    }

    if (!mModel->getSession()->currentTest()) {
        mView->showMessage("Test not found");
        completeTest();
        return;
    }

    auto session = mModel->getSession();
    auto isCorrect = session->submitTest(index, answer);

    if (isCorrect) {
        mView->showMessage("Correct answer!");
    } else {
        mView->showMessage("Answer is wrong!");
    }

    handleSession();
}

/**
 * @brief MainPresenter::handleSession
 *
 * Calls to work with current test
 * If session is finished (current test is null), show the results
 * Otherwise display and setup specific screen for test.
 */
void MainPresenter::handleSession() {
  if (!mModel->getSession()) {
    mView->showMessage("Session is not initialized");
    return;
  }
  if (mModel->getSession()->isFinished()) {
    completeTest();
    return;
  }
  auto type = mModel->getSession()->currentTest()->getType();

  initView(type);
}

void MainPresenter::completeTest() {
    mModel->storeSession(mModel->getSession()->getState());
    initView(ViewType::RESULT);
}

void MainPresenter::initView(const ViewType *type) {
    mView->presentView(type);

    auto session = mModel->getSession();
    bool hasSession = session != nullptr;

    int points = hasSession ? session->getPoints() : 0;
    int right = hasSession ? session->getCorrectAnswersCount() : 0;
    int wrong = hasSession ? session->getWrongAnswersCount() : 0;

    if (type == ViewType::MENU) {
        mView->setupMenuScreen(hasSession && !session->isFinished(), points);
    } else if (type == ViewType::RESULT) {
        mView->setupResultScreen(mModel->getLastSession());
    } else if (type == ViewType::HISTORY) {
        mView->setupHistoryList(mModel->getHistory());
    } else {
        auto test = session->currentTest();
        if (test == nullptr) return;

        if (type == ViewType::INPUT) mView->setupInputScreen(test->getQuestion(), "");

        if (type == ViewType::CHOICE) {
            ChoiceTest *choiceTest;
            if ((choiceTest = dynamic_cast<ChoiceTest *>(test)) != nullptr) {
                mView->setupChoiceScreen(test->getQuestion(), choiceTest->getAnswers());
            }
        }
        if (type == ViewType::CHECK) {
            CheckTest *checkTest;
            if ((checkTest = dynamic_cast<CheckTest *>(test)) != nullptr) {
                mView->setupCheckScreen(test->getQuestion(), checkTest->getAnswers());
            }
        }
    }
}

void MainPresenter::requestNewSession(bool force, bool continueSession) {
  int result = mModel->newSession(force);

  if (result == 0) {
    handleSession();
  } else if (result == 1) {
    if (force) {
      mModel->newSession(true);
      handleSession();
      return;
    }
    if (continueSession) {
      handleSession();
      return;
    }
    mView->hideLoading();
    mView->showMessage("Session already exists");
    mView->askSession();
  } else {
    mView->hideLoading();
    mView->showMessage("Error!");
  }
}

void MainPresenter::onProgressDone() {
    mView->hideLoading();
    mView->enableContent();
    mView->showMessage("Ready to work!");
}

void MainPresenter::onError(QString message) {
    mView->hideLoading();
    mView->enableContent();
    mView->showMessage(message);
}

void MainPresenter::onSessionFinish() {
    onProgressDone();
    initView(ViewType::MENU);
}

void MainPresenter::onSessionError(QString message) {
    onError(message);
    initView(ViewType::MENU);
}

void MainPresenter::initApplication() {
    mView->showLoading();
    mView->disableContent();
    mView->showMessage("Loading words...");
    WordsFileLoader *loader = new WordsFileLoader(mModel);
    QObject::connect(loader, &WordsFileLoader::progressDone, this, &MainPresenter::onProgressDone);
    QObject::connect(loader, &WordsFileLoader::progressError, this, &MainPresenter::onError);
    loader->start();
}

void MainPresenter::requestSessionFinish() {
    mView->showLoading();
    mView->disableContent();
    mView->showMessage("Saving session...");
    StoreWorker *worker = new StoreWorker(mModel);
    QObject::connect(worker, &StoreWorker::progressDone, this, &MainPresenter::onSessionFinish);
    QObject::connect(worker, &StoreWorker::progressError, this, &MainPresenter::onSessionError);
    worker->start();
}

void MainPresenter::requestHistoryDetailUpdate(int index) {
    mView->setupHistoryDetails(mModel->getHistory()[index]->getTestResults());
}

QString& MainPresenter::getVersion() {
    return mModel->getVersion();
}
