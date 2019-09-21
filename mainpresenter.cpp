#include "mainpresenter.hpp"

#include <QApplication>

MainPresenter::MainPresenter(Model *model, MainView *view): mModel(model), mView(view) {
  Q_ASSERT(model);
  Q_ASSERT(view);
}

void MainPresenter::proceedAnswer(std::string answer) {
  if (!mModel->getSession()) {
    mView->showMessage("Session is not initialized");
    return;
  }
  if (!mModel->getSession()->currentTest()) {
    mView->showMessage("Test not found");
    completeTest();
    return;
  }

  if (mModel->getSession()->currentTest()->checkResult(answer)) {
    mView->showMessage("Right answer");
    mModel->getSession()->checkTest(true);
    mModel->getSession()->nextTest();
    initSession();
  } else {
    mView->showMessage("Answer is wrong!!");
    mModel->getSession()->checkTest(false);
    // TODO: mView->resetInputs();
  }
}

void MainPresenter::requestNewSession(bool force, bool continueSession) {
  int result = mModel->newSession(force);
  if (result == 0) {
    initSession();
  } else if (result == 1) {
    if (force) {
      mModel->newSession(true);
      initSession();
      return;
    }
    if (continueSession) {
      initSession();
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

void MainPresenter::initSession() {
  if (!mModel->getSession()) {
    mView->showMessage("Session is not initialized");
    return;
  }
  if (mModel->getSession()->isFinished()) {
    completeTest();
    return;
  }

  mView->presentView(mModel->getSession()->currentTest()->getType());
  mView->prepareView((mModel->getSession()->currentTest()->getType()));
}

void MainPresenter::completeTest() {
  mView->presentView(ViewType::RESULT);
  mView->prepareView(ViewType::RESULT);
}

std::tuple<QString, QString, QString> MainPresenter::getResults() {
  return std::tuple<QString, QString, QString>("1", "2", "3");
}

// I WILL REFACTOR IT LATER!
std::tuple<QString, QString> MainPresenter::getTestInfo() {
  if (mModel->getSession() && !mModel->getSession()->isFinished()) {
    return std::tuple<QString, QString>(QString::fromUtf8(mModel->getSession()->currentTest()->getQuestion().c_str()),
                                      QString::fromUtf8(mModel->getSession()->currentTest()->getAnswer().c_str()));
  }
  return std::tuple<QString, QString>("", "");
}

int MainPresenter::getRandomPosition() {
  return mModel->getRandomPosition();
}
