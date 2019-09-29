#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "view/sessiondialog/sessiondialog.hpp"
#include "model/settings.h" // TODO: Create Presenter proxy!

#include <iostream>
#include <sstream>

MainWindow::MainWindow(
    Model* model,
    Settings *settings,
    QWidget *parent
): QMainWindow(parent), ui(new Ui::MainWindow), mPresenter(new MainPresenter(model, this)), mSettings(settings) {
    ui->setupUi(this);

    // I hate it!
    mChoiceButtons = new QPushButton*[6]{ ui->choiceOption1, ui->choiceOption2, ui->choiceOption3, ui->choiceOption4, ui->choiceOption5, ui->choiceOption6 };

    initConnection();
    initStatusBar();

    setupMenuScreen(false, 0);
    presentView(ViewType::MENU);

    mPresenter->initApplication();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::initConnection() {
    // Menu screen
    connect(ui->go2exit, &QPushButton::clicked, this, [=]() {
      this->close();
    });
    connect(ui->attemptsBox, &QCheckBox::toggled, this, [=]() {
       bool newState = ui->attemptsBox->isChecked();
       mSettings->isAttemptMode = newState;
    });

    connect(ui->startSession, &QPushButton::clicked, this, [=]() {
      mPresenter->requestNewSession(false);
    });

    // Choice screen
    for (int i = 0; i < 6; i++) {
      connect(mChoiceButtons[i], &QPushButton::clicked, this, [=]() {
        optionSubmit(i);
      });
    }

    // Input screen
    connect(ui->inputSubmitButton, &QPushButton::clicked, this, [=]() {
      if (ui->inputAnswerInput->text().size() > 0) {
        answerSubmit(ui->inputAnswerInput->text());
      }
    });

    // Check screen
    connect(ui->checkSubmitButton, &QPushButton::clicked, this, [=]() {
      answerSubmit(ui->checkBox->currentText());
    });

    // Result screen
    connect(ui->back2menu, &QPushButton::clicked, this, [=]() {
      presentView(ViewType::MENU);
    });
}

void MainWindow::initStatusBar() {
    statusProgressBar = new QProgressBar();
    statusProgressBar->setRange(0, 0);
    statusProgressBar->setValue(0);
    statusProgressBar->setVisible(false);

    statusBarLabel = new QLabel();
    setTextFor(statusBarLabel, "");

    ui->statusbar->addPermanentWidget(statusProgressBar);
    ui->statusbar->addPermanentWidget(statusBarLabel);
}

bool MainWindow::presentView(const ViewType *type) {
    if (type == ViewType::MENU) {
        ui->stackedWidget->setCurrentWidget(ui->menuPage);
    } else if (type == ViewType::CHECK) {
        ui->stackedWidget->setCurrentWidget(ui->checkTest);
    } else if (type == ViewType::INPUT) {
        ui->stackedWidget->setCurrentWidget(ui->inputTest);
    } else if (type == ViewType::CHOICE) {
        ui->stackedWidget->setCurrentWidget(ui->choiceTest);
    } else if (type == ViewType::RESULT) {
      ui->stackedWidget->setCurrentWidget(ui->result);
    } else {
        return false;
    }
    return true;
}

void MainWindow::askSession() {
  auto dialog = new SessionDialog(mPresenter);
  dialog->show();
}

void MainWindow::optionSubmit(int position) {
  answerSubmit(mChoiceButtons[position]->text());
}

void MainWindow::answerSubmit(QString answer) {
  mPresenter->proceedAnswer(answer);
}

void MainWindow::showLoading() {
    statusProgressBar->setVisible(true);
}

void MainWindow::hideLoading() {
    statusProgressBar->setVisible(false);
}

void MainWindow::showMessage(QString message) {
    setTextFor(statusBarLabel, message);
}

void MainWindow::disableContent() {
  ui->stackedWidget->setEnabled(false);
}

void MainWindow::enableContent() {
  ui->stackedWidget->setEnabled(true);
}

void MainWindow::setupMenuScreen(bool hasActiveSession, int points) {
    if (hasActiveSession) {
        ui->sessionInfo->setVisible(true);
        QString message = "You have unfinished session. Your points: " + QString::number(points);
        setTextFor(ui->sessionInfo, message);
    } else {
        ui->sessionInfo->setVisible(false);
    }
    ui->attemptsBox->setChecked(mSettings->isAttemptMode);
}

void MainWindow::setupChoiceScreen(QString question, std::vector<QString> answers) {
    setTextFor(ui->questionLabelChoice, question);
    for (size_t i = 0; i < 6; i++) {
        setTextFor(mChoiceButtons[i], answers[i]);
    }
 }

void MainWindow::setupInputScreen(QString question, QString prefill) {
    setTextFor(ui->questionLabelInput, question);
    setTextFor(ui->inputAnswerInput, prefill);
}

void MainWindow::setupCheckScreen(QString question, std::vector<QString> answers) {
    setTextFor(ui->questionLabelCheck, question);
    ui->checkBox->clear();
    for (auto ans : answers) {
        ui->checkBox->addItem(ans);
    }
}

void MainWindow::setupResultScreen(QString points, QString rightAnswers, QString wrongAnswers, QString result) {
    setTextFor(ui->pointsLabel, points);
    setTextFor(ui->rightLabel, rightAnswers);
    setTextFor(ui->wrongLabel, wrongAnswers);
    setTextFor(ui->resultLabel, result);
}
