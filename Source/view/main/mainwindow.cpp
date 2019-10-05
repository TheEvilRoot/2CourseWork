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
):  QMainWindow(parent),
    ui(new Ui::MainWindow),
    mPresenter(new MainPresenter(model, this)),
    mSettings(settings) {
    ui->setupUi(this);

    // I hate it!
    mChoiceButtons = new QPushButton*[6]{ ui->choiceOption1, ui->choiceOption2, ui->choiceOption3, ui->choiceOption4, ui->choiceOption5, ui->choiceOption6 };
    mDetailHeaders = new QStringList {"Question", "Answer", "Attempts", "Points", "Solve Time", "Your answers"};
    mHistoryHeaders = new QStringList {"Session time", "Correct", "Wrong", "Points", "Result"};
    mResultHeaders = new QStringList {"Question", "Answer", "Points", "Attempts", "Time"};

    initConnection();
    initStatusBar();
    initResultTable();
    initHistoryTables();

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
     // if (ui->inputAnswerInput->text().size() > 0) {
        answerSubmit(ui->inputAnswerInput->text());
      //}
    });

    // Check screen
    connect(ui->checkSubmitButton, &QPushButton::clicked, this, [=]() {
        optionSubmit(ui->checkBox->currentIndex());
    });

    // Result screen
    connect(ui->back2menu, &QPushButton::clicked, this, [=]() {
        mPresenter->requestSessionFinish();
        // presentView(ViewType::MENU);
    });

    connect(ui->showHistory, &QPushButton::clicked, this, [=]() {
        mPresenter->initView(ViewType::HISTORY);
    });

    connect(ui->historyBack2Menu, &QPushButton::clicked, this, [=]() {
        mPresenter->initView(ViewType::MENU);
    });

    connect(ui->historyTable, &QTableWidget::itemSelectionChanged, this, [=]() {
        mPresenter->requestHistoryDetailUpdate(ui->historyTable->currentRow());
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

void MainWindow::initResultTable() {
    ui->logTable->setColumnCount(5);
    ui->logTable->setHorizontalHeaderLabels(*mResultHeaders);
}

void MainWindow::initHistoryTables() {
    ui->historyTable->setHorizontalHeaderLabels(*mHistoryHeaders);
    ui->historyTable->setColumnCount(5);
    ui->historyTable->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    ui->historyTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->detailTable->setHorizontalHeaderLabels(*mDetailHeaders);
    ui->detailTable->setColumnCount(6);
    ui->detailTable->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    ui->detailTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
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
    } else if (type == ViewType::HISTORY) {
      ui->stackedWidget->setCurrentWidget(ui->history);
    }else {
        return false;
    }
    return true;
}

void MainWindow::askSession() {
  auto dialog = new SessionDialog(mPresenter);
  dialog->show();
}

void MainWindow::optionSubmit(int position) {
    size_t index = static_cast<size_t>(position);
    mPresenter->proceedAnswer("", index);
}

void MainWindow::answerSubmit(QString answer) {
  mPresenter->proceedAnswer(answer, 0);
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

void MainWindow::setupResultScreen(SessionState *state) {
    auto pointsString = QString::number(state->getPoints());
    auto correctString = QString::number(state->getCorrect());
    auto wrongString = QString::number(state->getWrong());

    setTextFor(ui->pointsLabel, pointsString);
    setTextFor(ui->rightLabel, correctString);
    setTextFor(ui->wrongLabel, wrongString);
    setTextFor(ui->resultLabel, state->getResultString());

    ui->logTable->setRowCount(static_cast<int>(state->getCount()));
    for (int i = 0; i < ui->logTable->rowCount(); i++) {
        auto res = state->at(static_cast<size_t>(i));
        int j = 0;
        ui->logTable->setItem(i, j++, new QTableWidgetItem(res->mQuestion));
        ui->logTable->setItem(i, j++, new QTableWidgetItem(res->mAnswer));
        ui->logTable->setItem(i, j++, new QTableWidgetItem(QString::number(res->mPointsForTest)));
        ui->logTable->setItem(i, j++, new QTableWidgetItem(QString::number(res->mAttempts)));
        ui->logTable->setItem(i, j++, new QTableWidgetItem(res->mSolveTime.toString(Qt::DateFormat::DefaultLocaleShortDate)));
    }
}

void MainWindow::setupHistoryList(std::deque<SessionState *> &states) {
    ui->historyTable->clear();
    ui->historyTable->setHorizontalHeaderLabels(*mHistoryHeaders);
    ui->historyTable->setRowCount(static_cast<int>(states.size()));
    for (int i = 0; i < ui->historyTable->rowCount(); i++) {
        auto state = states[static_cast<size_t>(i)];
        int j = 0;

        ui->historyTable->setItem(i, j++, new QTableWidgetItem(state->getTime().toString(Qt::DateFormat::DefaultLocaleShortDate)));
        ui->historyTable->setItem(i, j++, new QTableWidgetItem(QString::number(state->getCorrect())));
        ui->historyTable->setItem(i, j++, new QTableWidgetItem(QString::number(state->getWrong())));
        ui->historyTable->setItem(i, j++, new QTableWidgetItem(QString::number(state->getPoints())));
        ui->historyTable->setItem(i, j++, new QTableWidgetItem(state->getResultString()));
    }
}

void MainWindow::setupHistoryDetails(std::deque<Result *> &results) {
    ui->detailTable->clear();
    ui->detailTable->setHorizontalHeaderLabels(*mDetailHeaders);
    ui->detailTable->setRowCount(static_cast<int>(results.size()));

    for (int i = 0; i < ui->detailTable->rowCount(); i++) {
        auto result = results[static_cast<size_t>(i)];
        int j = 0;

        ui->detailTable->setItem(i, j++, new QTableWidgetItem(result->mQuestion));
        ui->detailTable->setItem(i, j++, new QTableWidgetItem(result->mAnswer));
        ui->detailTable->setItem(i, j++, new QTableWidgetItem(QString::number(result->mAttempts)));
        ui->detailTable->setItem(i, j++, new QTableWidgetItem(QString::number(result->mPointsForTest)));
        ui->detailTable->setItem(i, j++, new QTableWidgetItem(result->mSolveTime.toString(Qt::DateFormat::DefaultLocaleShortDate)));
        ui->detailTable->setItem(i, j++, new QTableWidgetItem(result->getJoinedAnswers(',')));
    }
}
