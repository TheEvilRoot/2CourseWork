#pragma once

#include "api/utils.hpp"
#include "view/viewtype.hpp"
#include "view/mainview.hpp"
#include "model/model.hpp"
#include "presenter/main/mainpresenter.hpp"
#include "model/settings.hpp"
#include "view/qfloatingwidget.hpp"
#include "view/qresultwidget.hpp"

#include <QMainWindow>
#include <QProgressBar>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow, public MainView {
    Q_OBJECT

public:
    MainWindow(QApplication *application, Model *model, Settings *settings, QWidget *parent = nullptr);
    ~MainWindow();

    bool presentView(const ViewType *type) override;
    void showLoading() override;
    void hideLoading() override ;
    void showMessage(QString message = "", bool enablePopup = false) override;
    void enableContent() override;
    void disableContent() override;
    void askSession() override;
    void initiateError(bool fatal = false, QString message = "Unexpected error.") override;

    void setupResultScreen(SessionState *state) override;
    void setupChoiceScreen(QString question, std::vector<QString> answers) override;
    void setupCheckScreen(QString question, std::vector<QString> answers) override;
    void setupInputScreen(QString question, QString prefill = "") override;
    void setupMenuScreen(SessionState *currentState) override;

    void setupHistoryList(std::deque<SessionState *> &states) override;
    void setupHistoryDetails(SessionState *state) override;

    void setTestTitle(const ViewType *type, size_t index, size_t count) override;
    void setTipWords(std::pair<QString, QString> &words) override;

private slots:

private:
    void initSettings();
    void initMisc();
    void initResultWidgets();
    void initFloatingWidget();
    void initConnection();
    void initStatusBar();
    void initHistoryTables();
    void optionSubmit(int position);
    void answerSubmit(const QString& answer);
    void showPopup(const QString& message, unsigned int time = 1000);
    QTableWidgetItem* notEditableItem(const QString& content);

    Ui::MainWindow *ui;
    MainPresenter *mPresenter;
    Settings *mSettings;
    QProgressBar *statusProgressBar;
    QLabel *statusBarLabel;
    QPushButton **mChoiceButtons;
    QStringList *mStateHeaders;
    QStringList *mHistoryHeaders;
    QApplication *mApplication;
    QFloatingWidget *mFloating;
    QResultWidget *mHistoryResult;
    QResultWidget *mFinalResult;
};
