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
        mView->showMessage("Тестирование не начато!", true);
        return;
    }

    if (!mModel->getSession()->currentTest()) {
        mView->showMessage("Тест не найден!", true);
        completeSession();
        return;
    }

    auto session = mModel->getSession();
    auto isCorrect = session->submitTest(index, answer);

    if (isCorrect) {
        mView->showMessage("Правильный ответ!", true);
    } else {
        mView->showMessage("Ответ не верный...", true);
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
    mView->showMessage("Тестирование не начато!", true);
    return;
  }
  if (mModel->getSession()->isFinished()) {
    completeSession();
    return;
  }
  auto type = mModel->getSession()->currentTest()->getType();

  initView(type);
}

void MainPresenter::completeSession() {
    mModel->storeSession(mModel->getSession()->getState());
    mModel->sessionConclude();
    initView(ViewType::RESULT);
}

void MainPresenter::initView(const ViewType *type) {
    mView->presentView(type);

    auto session = mModel->getSession();
    if (type == ViewType::MENU) {
        mView->setupMenuScreen(session && !session->isFinished() ? session->getState() : nullptr);
    } else if (type == ViewType::RESULT) {
        mView->setupResultScreen(mModel->getLastSession());
    } else if (type == ViewType::HISTORY) {
        mView->setupHistoryList(mModel->getHistory());
    } else {
        auto testsCount = session->getTestsCount();
        auto testIndex = session->getTestPosition();
        auto test = session->currentTest();
        if (test == nullptr) return;

        if (type == ViewType::INPUT) {
            mView->setupInputScreen(test->getQuestion(), "");
        }
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
        mView->setTestTitle(type, testIndex, testsCount);
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
    mView->showMessage("Тестирование уже начато", true);
    mView->askSession();
  } else {
    mView->hideLoading();
    mView->initiateError(true, "New session erro");
  }
}

void MainPresenter::onProgressDone() {
    mView->hideLoading();
    mView->enableContent();
    mView->showMessage("Готов к работе");
}

void MainPresenter::onError(QString message) {
    mView->hideLoading();
    mView->showMessage(message);
    mView->initiateError(true, message);
}

void MainPresenter::onSessionFinish(const ViewType *nextView) {
    onProgressDone();
    initView(nextView);
}

void MainPresenter::onSessionError(QString message) {
    onError(message);
    initView(ViewType::MENU);
}

void MainPresenter::initApplication() {
    mView->showLoading();
    mView->disableContent();
    mView->showMessage("Загрузка данных...");
    WordsFileLoader *loader = new WordsFileLoader(mModel);
    QObject::connect(loader, &WordsFileLoader::progressDone, this, &MainPresenter::onProgressDone);
    QObject::connect(loader, &WordsFileLoader::progressError, this, &MainPresenter::onError);
    loader->start();
}

void MainPresenter::requestSessionFinish(const ViewType *nextView) {
    mView->showLoading();
    mView->disableContent();
    mView->showMessage("Сохранение...");
    StoreWorker *worker = new StoreWorker(mModel, nextView);
    QObject::connect(worker, &StoreWorker::progressDone, this, &MainPresenter::onSessionFinish);
    QObject::connect(worker, &StoreWorker::progressError, this, &MainPresenter::onSessionError);
    worker->start();
}

void MainPresenter::requestHistoryDetailUpdate(int index) {
    auto historyEntry = index < 0 || index  > mModel->getHistory().size() ? nullptr : mModel->getHistory()[index];

    mView->setupHistoryDetails(historyEntry);
}

QString& MainPresenter::getVersion() {
    return mModel->getVersion();
}
