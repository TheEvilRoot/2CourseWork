#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "view/sessiondialog/sessiondialog.hpp"
#include "model/settings.hpp" // TODO: Create Presenter proxy!

#include <QMessageBox>
#include <QSpinBox>

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

    mFloating = new QFloatingWidget(this);

    initConnection();
    initStatusBar();
    initHistoryTables();

    mHistoryResult = new QResultWidget();
    mHistoryResult->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
    ui->resultLayout->addWidget(mHistoryResult);

    mFinalResult = new QResultWidget();
    mFinalResult->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
    ui->finalResult->addWidget(mFinalResult);

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
    connect(ui->attemptsBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [=]() {
        mSettings->attemptsCount = ui->attemptsBox->value();
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
        answerSubmit(ui->inputAnswerInput->text());
    });

    // Check screen
    connect(ui->checkSubmitButton, &QPushButton::clicked, this, [=]() {
        optionSubmit(ui->checkBox->currentIndex());
    });

    // Result screen
    connect(ui->back2menu, &QPushButton::clicked, this, [=]() {
        mPresenter->requestSessionFinish(ViewType::MENU);
    });
    connect(ui->resGo2History, &QPushButton::clicked, this, [=]() {
       mPresenter->requestSessionFinish(ViewType::HISTORY);
    });
    // History screen
    connect(ui->showHistory, &QPushButton::clicked, this, [=]() {
        mPresenter->initView(ViewType::HISTORY);
    });

    connect(ui->historyBack2Menu, &QPushButton::clicked, this, [=]() {
        mPresenter->initView(ViewType::MENU);
    });
    connect(ui->historyTable, &QTableWidget::itemSelectionChanged, this, [=]() {
        auto selectedIndex = ui->historyTable->currentRow();
        auto isSelected = ui->historyTable->selectedRanges().size();
        mPresenter->requestHistoryDetailUpdate(isSelected ? selectedIndex : -1);
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

void MainWindow::showMessage(QString message, bool enablePopup) {
    setTextFor(statusBarLabel, message);
    if (enablePopup)
        showPopup(message);
}

void MainWindow::initiateError(bool fatal, QString message) {
   if (fatal) {
       disableContent();
       QMessageBox *errorBox = new QMessageBox(this);
       errorBox->setText("Во время работы приложения произошла ошибка. К сожалению дальнейшая работа невозможна.\nДалее представлена информация об ошибке, которая может помочь решить ее:\n\n" + message);
       errorBox->setWindowTitle("Произошла ошибка");
       errorBox->setButtonText(QMessageBox::Ok, "Закрыть приложение");
       errorBox->button(QMessageBox::Ok)->setStyleSheet("padding-left: 20px; padding-right: 20px;");
       connect(errorBox->button(QMessageBox::Ok), &QPushButton::clicked, [=]() {
           exit(0);
       });
       errorBox->setWindowFlags((errorBox->windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowCloseButtonHint);
       errorBox->show();
   } else {
       showPopup("Произошла ошибка: " + message);
   }
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
    mHistoryResult->setState(state);

    if (state == nullptr) {
        setTextFor(ui->cefrResult, "");
        setTextFor(ui->resultMessage, "");
        setTextFor(ui->wbpercentLabel, "");
        setTextFor(ui->sbpercentLabel, "");
        return;
    }

    if (state->getCefr() == CEFR::NOTHING) {
        setTextFor(ui->cefrResult, "Нам пока не удалось определить Ваш уровень знаний. Больше практикуйтесь и в следующий раз у Вас все получится!");
        setTextFor(ui->resultMessage, "");
    } else {
        setTextFor(ui->cefrResult, "Ваш результат: " + state->getCefr()->getName());
        setTextFor(ui->resultMessage, state->getResultString());
    }
    setTextFor(ui->wbpercentLabel, "Процент верных решений тестов со лексикой: " + QString::number(state->getWordBasedPercent() * 100) + "%%");
    setTextFor(ui->sbpercentLabel, "Процень верных решений тестов с текстами: " + QString::number(state->getSentenceBasedPercent() * 100) + "%%");
}

void MainWindow::setupResultScreen(SessionState *state) {
    if (state == nullptr) return;
    auto pointsString = QString::number(state->getPoints());
    auto correctString = QString::number(state->getCorrect());
    auto wrongString = QString::number(state->getWrong());
    auto time = state->getSolveTime().toString("mm:ss");

    setTextFor(ui->pointsLabel, pointsString);
    setTextFor(ui->rightLabel, correctString);
    setTextFor(ui->wrongLabel, wrongString);
    setTextFor(ui->resTimeLabel, time);

    if (state->getCefr() == CEFR::NOTHING) {
        setTextFor(ui->resCefrResult, "Нам пока не удалось определить Ваш уровень знаний. Больше практикуйтесь и в следующий раз у Вас все получится!");
        setTextFor(ui->resultLabel, "");
    } else {
        setTextFor(ui->resCefrResult, "Ваш результат: " + state->getCefr()->getName());
        setTextFor(ui->resultLabel, state->getResultString());
    }

    mFinalResult->setState(state);
}

QTableWidgetItem* MainWindow::notEditableItem(QString content) {
    auto item = new QTableWidgetItem(content);
    item->setFlags(item->flags() & (~Qt::ItemIsEditable));
    return item;
}

void MainWindow::showPopup(QString message, unsigned int time) {
    mFloating->setPopupText(message);

    const QPoint point(0, 0);
    const QPoint globalPos = ui->centralwidget->mapToGlobal(point);
    const int posX = globalPos.x() + ((width() - mFloating->width()) >> 1);
    const int posY = globalPos.y() + ((height() - mFloating->height()) >> 1);

    mFloating->setGeometry(posX, posY,
                       mFloating->width(),
                       mFloating->height());

    mFloating->show();
}

void MainWindow::setTestTitle(const ViewType *type, size_t index, size_t count) {
    QLabel *titleView = nullptr;
    if (type == ViewType::INPUT) titleView = ui->inputTestTitle;
    if (type == ViewType::CHECK) titleView = ui->checkTestTitle;
    if (type == ViewType::CHOICE) titleView = ui->choiceTestTitle;

    if (titleView) setTextFor(titleView, "Тест " + QString::number(index + 1) + "/" + QString::number(count));
}

void MainWindow::setTipWords(std::pair<QString, QString> &words) {
    setTextFor(ui->menuTipView, "Интересный факт<br><b>" + words.first + "</b> переводится как <b>" + words.second + "</b>");
}
