#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <iostream>
#include <ostream>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    initConnection();
    initStatusBar();
    ui->stackedWidget->setCurrentWidget(ui->checkTest);
}

void MainWindow::initConnection() {
    connect(ui->go2check, &QPushButton::clicked, [=]() {
        presentView(ViewType::CHECK);
    });
    connect(ui->go2input, &QPushButton::clicked, [=]() {
        presentView(ViewType::INPUT);
    });
    connect(ui->go2choice, &QPushButton::clicked, [=]() {
        presentView(ViewType::CHOICE);
    });
    connect(ui->back2MenuButton, &QPushButton::clicked, [=]() {
       presentView(ViewType::MENU);
    });

    connect(ui->go2exit, &QPushButton::clicked, [=]() {
        this->close();
    });
}

void MainWindow::initStatusBar() {
    statusProgressBar = new QProgressBar();
    statusProgressBar->setRange(0, 100);
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
    } else {
        std::cout << "Invalid type " << type << " ";
        if (type != nullptr) std::cout << type->getViewName();
        std::cout << std::endl;
        return false;
    }
    return true;
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
