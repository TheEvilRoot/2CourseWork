#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QProgressBar>
#include <QPushButton>
#include <QLabel>

#include "viewtype.h"
#include "mainview.hpp"
#include "model.hpp"
#include "mainpresenter.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow, public MainView {
    Q_OBJECT

public:
    MainWindow(Model *model, QWidget *parent = nullptr);
    ~MainWindow() override;
    bool presentView(const ViewType *type) override;
    void showLoading() override;
    void hideLoading() override ;
    void showMessage(std::string message = "") override;
    void enableContent() override;
    void disableContent() override;
    void askSession() override;
    void prepareView(const ViewType *type) override;

private slots:

private:
    void initConnection();
    void initStatusBar();

    void optionSubmit(int position);
    void answerSubmit(QString answer);

    Ui::MainWindow *ui;
    MainPresenter *mPresenter;

    QProgressBar *statusProgressBar;
    QLabel *statusBarLabel;

    QPushButton **mChoiceButtons;
};
#endif
