#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QProgressBar>
#include <QLabel>

#include "viewtype.h"
#include "mainview.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow, public MainView {

    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    bool presentView(const ViewType *type) override;
    void showLoading() override;
    void hideLoading() override ;
    void showMessage(std::string message) override;

private slots:

private:
    void initConnection();
    void initStatusBar();

    Ui::MainWindow *ui;

    QProgressBar *statusProgressBar;
    QLabel *statusBarLabel;
};
#endif
