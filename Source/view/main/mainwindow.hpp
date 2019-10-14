#pragma once

#include "view/viewtype.h"
#include "view/mainview.hpp"
#include "model/model.hpp"
#include "presenter/main/mainpresenter.hpp"
#include "model/settings.h"

#include <QMainWindow>
#include <QProgressBar>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>

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
    MainWindow(QApplication *application, Model *model, Settings *settings, QWidget *parent = nullptr);
    ~MainWindow() override;

    bool presentView(const ViewType *type) override;
    void showLoading() override;
    void hideLoading() override ;
    void showMessage(QString message = "") override;
    void enableContent() override;
    void disableContent() override;
    void askSession() override;

    void setupResultScreen(SessionState *state) override;
    void setupChoiceScreen(QString question, std::vector<QString> answers) override;
    void setupCheckScreen(QString question, std::vector<QString> answers) override;
    void setupInputScreen(QString question, QString prefill = "") override;
    void setupMenuScreen(SessionState *currentState) override;

    void setupHistoryList(std::deque<SessionState *> &states) override;
    void setupHistoryDetails(SessionState *state) override;

private slots:

private:
    void initConnection();
    void initStatusBar();
    void initStateTable(QTableWidget *table);
    void initHistoryTables();
    void setupStateTableForState(QTableWidget *table, SessionState *state);
    void optionSubmit(int position);
    void answerSubmit(QString answer);
    QTableWidgetItem* notEditableItem(QString content);

    Ui::MainWindow *ui;
    MainPresenter *mPresenter;
    Settings *mSettings;
    QProgressBar *statusProgressBar;
    QLabel *statusBarLabel;
    QPushButton **mChoiceButtons;
    QStringList *mStateHeaders;
    QStringList *mHistoryHeaders;
    QApplication *mApplication;
};
