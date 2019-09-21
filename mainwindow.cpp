#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "sessiondialog.hpp"

#include <QRandomGenerator>
#include <iostream>
#include <sstream>

MainWindow::MainWindow(
    Model* model,
    QWidget *parent
): QMainWindow(parent), ui(new Ui::MainWindow), mPresenter(new MainPresenter(model, this)) {
    ui->setupUi(this);
    // I hate it!
    mChoiceButtons = new QPushButton*[6]{ ui->choiceOption1, ui->choiceOption2, ui->choiceOption3, ui->choiceOption4, ui->choiceOption5, ui->choiceOption6 };
    initConnection();
    initStatusBar();

    presentView(ViewType::MENU);
}

void MainWindow::initConnection() {
    /*connect(ui->back2MenuButton, &QPushButton::clicked, [=]() {
       presentView(ViewType::MENU);
    }); */// TODO: Debug!

    connect(ui->go2exit, &QPushButton::clicked, this, [=]() {
      this->close();
    });
    connect(ui->startSession, &QPushButton::clicked, this, [=]() {
      mPresenter->requestNewSession(false);
    });
    for (int i = 0; i < 6; i++) {
      connect(mChoiceButtons[i], &QPushButton::clicked, this, [=]() {
        optionSubmit(i);
      });
    }

    connect(ui->inputSubmitButton, &QPushButton::clicked, this, [=]() {
      if (ui->inputAnswerInput->text().size() > 0) {
        answerSubmit(ui->inputAnswerInput->text());
      }
    });

    connect(ui->checkSubmitButton, &QPushButton::clicked, this, [=]() {
      std::ostringstream osstr;
      osstr << ui->checkBox->currentIndex() << "\n";
      showMessage(osstr.str());

      answerSubmit(ui->checkBox->currentText());
    });

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
    statusBarLabel->setText("");

    ui->statusbar->addPermanentWidget(statusProgressBar);
    ui->statusbar->addPermanentWidget(statusBarLabel);
}

MainWindow::~MainWindow() {
    delete ui;
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

void MainWindow::prepareView(const ViewType *type) {
  if (type == ViewType::RESULT) {
    auto [points, wrong, right] = mPresenter->getResults();
    ui->pointsLabel->setText(points);
    ui->wrongLabel->setText(wrong);
    ui->rightLabel->setText(right);
  } else if (type == ViewType::CHOICE) {
    auto [question, answer] = mPresenter->getTestInfo();
    ui->questionLabelChoice->setText(question);

    int randomPos = mPresenter->getRandomPosition();
    for (int i = 0; i < 6; i++) {
      if (i == randomPos) {
        mChoiceButtons[i]->setText(answer);
      } else {
        char c = '0' + i;
        mChoiceButtons[i]->setText(QString::fromUtf8(&c)); // REFACTOR
      }
    }
  }
}

void MainWindow::optionSubmit(int position) {
  answerSubmit(mChoiceButtons[position]->text());
}

void MainWindow::answerSubmit(QString answer) {
  mPresenter->proceedAnswer(answer.toStdString());
}

void MainWindow::showLoading() {
    statusProgressBar->setVisible(true);
}

void MainWindow::hideLoading() {
    statusProgressBar->setVisible(false);
}

void MainWindow::showMessage(std::string message) {
    statusBarLabel->setText(QString::fromUtf8(message.c_str()));
}

void MainWindow::disableContent() {
  ui->stackedWidget->setEnabled(false);
}

void MainWindow::enableContent() {
  ui->stackedWidget->setEnabled(true);
}
