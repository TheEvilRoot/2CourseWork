#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "view/viewtype.h"
#include "view/mainview.hpp"
#include "model/model.hpp"
#include "presenter/main/mainpresenter.hpp"
#include "model/settings.h"

#include <QMainWindow>
#include <QProgressBar>
#include <QPushButton>
#include <QLabel>

#ifdef Q_OS_MAC
#define setTextFor(__widget, __text) (__widget)->setText(__text); (__widget)->repaint()
#else
#define setTextFor(__widget, __text) (__widget)->setText(__text)
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow, public MainView {
    Q_OBJECT

public:
    MainWindow(Model *model, Settings *settings, QWidget *parent = nullptr);
    ~MainWindow() override;

    bool presentView(const ViewType *type) override;
    void showLoading() override;
    void hideLoading() override ;
    void showMessage(QString message = "") override;
    void enableContent() override;
    void disableContent() override;
    void askSession() override;

    void setupResultScreen(QString points, QString rightAnswers, QString wrongAnswers, QString result) override;
    void setupChoiceScreen(QString question, std::vector<QString> answers) override;
    void setupCheckScreen(QString question, std::vector<QString> answers) override;
    void setupInputScreen(QString question, QString prefill = "") override;
    void setupMenuScreen(bool hasActiveSession, int points) override;

private slots:

private:
    void initConnection();
    void initStatusBar();
    void optionSubmit(int position);
    void answerSubmit(QString answer);

    Ui::MainWindow *ui;
    MainPresenter *mPresenter;
    Settings *mSettings;
    QProgressBar *statusProgressBar;
    QLabel *statusBarLabel;
    QPushButton **mChoiceButtons;
};
#endif
