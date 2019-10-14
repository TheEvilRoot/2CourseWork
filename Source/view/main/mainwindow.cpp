#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "view/sessiondialog/sessiondialog.hpp"
#include "model/settings.h" // TODO: Create Presenter proxy!

MainWindow::MainWindow(
    QApplication *application,
    Model* model,
    Settings *settings,
    QWidget *parent
):  QMainWindow(parent),
    ui(new Ui::MainWindow),
    mPresenter(new MainPresenter(model, this)),
    mSettings(settings),
    mApplication(application) {
    ui->setupUi(this);

    // I hate it!
    mChoiceButtons = new QPushButton*[6]{ ui->choiceOption1, ui->choiceOption2, ui->choiceOption3, ui->choiceOption4, ui->choiceOption5, ui->choiceOption6 };
    mHistoryHeaders = new QStringList {"Время", "Верные ответы", "Неверные ответы", "Численных результат", "Результат"};
    mStateHeaders = new QStringList {"Вопрос", "Правильный ответ", "Попытки", "Численных результат", "Время", "Ваши ответы"};

    initConnection();
    initStatusBar();
    initStateTable(ui->logTable);
    initStateTable(ui->detailTable);
    initHistoryTables();

    setupMenuScreen(nullptr);
    presentView(ViewType::MENU);
    mApplication->setStyleSheet(*mSettings->style);
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

    // History screen
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

void MainWindow::initStateTable(QTableWidget *table) {
    table->setHorizontalHeaderLabels(*mStateHeaders);
    table->setColumnCount(6);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    table->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
}

void MainWindow::initHistoryTables() {
    ui->historyTable->setHorizontalHeaderLabels(*mHistoryHeaders);
    ui->historyTable->setColumnCount(5);
    ui->historyTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->historyTable->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    ui->historyTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
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

void MainWindow::setupMenuScreen(SessionState *currentState) {
    if (currentState != nullptr) {
        ui->sessionInfo->setVisible(true);
        setTextFor(ui->sessionInfo, "У вас есть незавершенное тестирование");
    } else {
        ui->sessionInfo->setVisible(false);
    }
    ui->versionLabel->setText(mPresenter->getVersion());
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

void MainWindow::setupHistoryList(std::deque<SessionState *> &states) {
    ui->historyTable->clear();
    ui->historyTable->setHorizontalHeaderLabels(*mHistoryHeaders);
    ui->historyTable->setRowCount(static_cast<int>(states.size()));
    for (int i = 0; i < ui->historyTable->rowCount(); i++) {
        auto state = states[static_cast<size_t>(i)];
        int j = 0;

        ui->historyTable->setItem(i, j++, notEditableItem(state->getTime().toString(Qt::DateFormat::DefaultLocaleShortDate)));
        ui->historyTable->setItem(i, j++, notEditableItem(QString::number(state->getCorrect())));
        ui->historyTable->setItem(i, j++, notEditableItem(QString::number(state->getWrong())));
        ui->historyTable->setItem(i, j++, notEditableItem(QString::number(state->getPoints())));
        ui->historyTable->setItem(i, j++, notEditableItem(state->getTime().toString(Qt::DateFormat::DefaultLocaleShortDate)));
    }
}

void MainWindow::setupHistoryDetails(SessionState *state) {
    ui->cefrResult->setText("Ваш результат: " + state->getCefr()->getName());
    ui->resultMessage->setText(state->getResultString());
    ui->wbpercentLabel->setText("Процент верных решений тестов со лексикой: " + QString::number(state->getWordBasedPercent() * 100) + "%%");
    ui->sbpercentLabel->setText("Процень верных решений тестов с текстами: " + QString::number(state->getSentenceBasedPercent() * 100) + "%%");
    setupStateTableForState(ui->detailTable, state);
}

void MainWindow::setupResultScreen(SessionState *state) {
    auto pointsString = QString::number(state->getPoints());
    auto correctString = QString::number(state->getCorrect());
    auto wrongString = QString::number(state->getWrong());

    setTextFor(ui->pointsLabel, pointsString);
    setTextFor(ui->rightLabel, correctString);
    setTextFor(ui->wrongLabel, wrongString);
    setTextFor(ui->resultLabel, state->getResultString());
    setupStateTableForState(ui->logTable, state);
}

void MainWindow::setupStateTableForState(QTableWidget *table, SessionState *state) {
    table->clear();
    table->setHorizontalHeaderLabels(*mStateHeaders);
    if (state == nullptr) return;

    table->setRowCount(static_cast<int>(state->getCount()));
    for (int i = 0; i < ui->logTable->rowCount(); i++) {
        auto res = state->at(static_cast<size_t>(i));
        int j = 0;
        table->setItem(i, j++, notEditableItem(res->mQuestion));
        table->setItem(i, j++, notEditableItem(res->mAnswer));
        table->setItem(i, j++, notEditableItem(res->getJoinedAnswers(',')));
        table->setItem(i, j++, notEditableItem(QString::number(res->mPointsForTest)));
        table->setItem(i, j++, notEditableItem(QString::number(res->mAttempts)));
        table->setItem(i, j++, notEditableItem(res->mSolveTime.toString("mm:ss")));
    }
}

QTableWidgetItem* MainWindow::notEditableItem(QString content) {
    auto item = new QTableWidgetItem(content);
    item->setFlags(item->flags() & (~Qt::ItemIsEditable));
    return item;
}
