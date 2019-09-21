#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <iostream>
#include <ostream>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    initConnection();
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
    connect(ui->go2exit, &QPushButton::clicked, [=]() {
        this->close();
    });
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

}

void MainWindow::hideLoading() {

}

void MainWindow::showMessage(std::string message) {

}


void MainWindow::on_back2MenuButton_clicked() {
    presentView(ViewType::MENU);
}
