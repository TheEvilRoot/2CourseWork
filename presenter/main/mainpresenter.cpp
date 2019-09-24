#include "mainpresenter.hpp"

#include <QApplication>

#include "model/loader/wordsfileloader.h"

MainPresenter::MainPresenter(Model *model, MainView *view): mModel(model), mView(view) {
  Q_ASSERT(model);
  Q_ASSERT(view);
}

void MainPresenter::proceedAnswer(QString answer) {
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
  if (session->currentTest()->checkResult(answer)) {
    session->checkTest(true);
    session->nextTest();
    mView->showMessage("Right answer");
    handleSession();
  } else {
      session->checkTest(false);
      mView->showMessage("Answer is wrong!!");

      if (!session->isAttemptsMode()) {
          session->nextTest();
          handleSession();
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
  initView(ViewType::RESULT);
}

void MainPresenter::initView(const ViewType *type) {
    mView->presentView(type);

    auto session = mModel->getSession();
    bool hasSession = session != nullptr;

    int points = hasSession ? session->getPoints() : 0;
    int right = hasSession ? session->getRightAnswersCount() : 0;
    int wrong = hasSession ? session->getWrongAnswersCount() : 0;

    if (type == ViewType::MENU) {
        mView->setupMenuScreen(hasSession && !session->isFinished(), points);
    } else if (type == ViewType::RESULT) {
        mView->setupResultScreen(QString::number(points), QString::number(right), QString::number(wrong), "");
    } else {
        auto test = session->currentTest();
        if (test == nullptr) return;

        if (type == ViewType::INPUT) mView->setupInputScreen(test->getQuestion(), "");

        std::vector<QString> answers;

        for (auto [ans, wer] : mModel->getRandomWords(5)) {
            answers.push_back(ans);
        }

        answers.push_back(test->getAnswer());

        if (type == ViewType::CHOICE) mView->setupChoiceScreen(test->getQuestion(), answers);
        if (type == ViewType::CHECK) mView->setupCheckScreen(test->getQuestion(), answers);
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

void MainPresenter::initApplication() {
    mView->showLoading();
    mView->disableContent();
    mView->showMessage("Loading words...");
    WordsFileLoader *loader = new WordsFileLoader(mModel);
    QObject::connect(loader, &WordsFileLoader::progressDone, this, &MainPresenter::onProgressDone);
    QObject::connect(loader, &WordsFileLoader::progressError, this, &MainPresenter::onError);
    loader->start();
}