#include "mainpresenter.hpp"
#include "model/loader/wordsfileloader.hpp"
#include "model/loader/storeworker.hpp"
#include "model/data/choicetest.hpp"
#include "model/data/checktest.hpp"

#include <QApplication>

MainPresenter::MainPresenter(Model *model, MainView *view):
    mModel(model),
    mView(view),
    mResultTestIndex(0),
    mLoader(nullptr),
    mWorker(nullptr) {
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
void MainPresenter::proceedAnswer(const QString& answer, size_t index) {
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
    auto isCorrect = session->submitTest(index, std::move(answer));

    // I will refactor this later
    if (isCorrect == 1) {
        mView->showMessage("Правильный ответ!", true);
    } else if (isCorrect >= session->getMagic()) {
        mView->showMessage("Ответ неверный. Осталось " + QString::number(session->getMaxAttempts() - (isCorrect - session->getMagic())) + " попыток");
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

void MainPresenter::updateMenuTip() {
    auto tip = mModel->getRandomWords(1);
    mView->setTipWords(tip[0]);
}

void MainPresenter::initView(const ViewType *type) {
    mView->presentView(type);

    auto session = mModel->getSession();
    if (type == ViewType::MENU) { 
        updateMenuTip();
        mView->setupMenuScreen(session && !session->isFinished() ? session->getState() : nullptr);
    } else if (type == ViewType::RESULT) {
        mResultTestIndex = 0;
        auto lastSession = mModel->getLastSession();
        mView->setupResultScreen(lastSession);
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

// File loader callback
void MainPresenter::onProgressDone() {
    mView->hideLoading();
    mView->enableContent();
    mView->showMessage("Готов к работе");
    updateMenuTip();
    delete mLoader;
}

// File loader callback
void MainPresenter::onError(const QString& message, bool fatal) {
    mView->hideLoading();
    mView->showMessage(message);
    mView->initiateError(fatal, message);
    delete mLoader;
}

// Store worker callback
void MainPresenter::onSessionFinish(const ViewType *nextView) {
    mView->hideLoading();
    mView->enableContent();
    mView->showMessage("Готов к работе");
    updateMenuTip();
    delete mWorker;

    initView(nextView);
}

// Store worker callback
void MainPresenter::onSessionError(const QString& message) {
    mView->hideLoading();
    mView->showMessage(message);
    mView->initiateError(true, message);
    delete mWorker;

    initView(ViewType::MENU);
}

void MainPresenter::initApplication() {
    if (mLoader != nullptr) return;
    mView->showLoading();
    mView->disableContent();
    mView->showMessage("Загрузка данных...");

    mLoader = new WordsFileLoader(mModel);
    QObject::connect(mLoader, &WordsFileLoader::progressDone, this, &MainPresenter::onProgressDone);
    QObject::connect(mLoader, &WordsFileLoader::progressError, this, &MainPresenter::onError);
    mLoader->start();
}

void MainPresenter::requestSessionFinish(const ViewType *nextView) {
    if (mWorker != nullptr) return;
    mView->showLoading();
    mView->disableContent();
    mView->showMessage("Сохранение...");

    mWorker = new StoreWorker(mModel, nextView);
    QObject::connect(mWorker, &StoreWorker::progressDone, this, &MainPresenter::onSessionFinish);
    QObject::connect(mWorker, &StoreWorker::progressError, this, &MainPresenter::onSessionError);
    mWorker->start();
}

void MainPresenter::requestHistoryDetailUpdate(int index) {
    auto historyEntry = index < 0 || index  > mModel->getHistory().size() ? nullptr : mModel->getHistory()[index];

    mView->setupHistoryDetails(historyEntry);
}

QString& MainPresenter::getVersion() {
    return mModel->getVersion();
}
