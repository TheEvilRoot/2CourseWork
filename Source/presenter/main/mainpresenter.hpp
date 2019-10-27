#pragma once

#include "model/model.hpp"
#include "view/mainview.hpp"

#include <QString>
#include <QObject>

class MainPresenter: public QObject {

    Q_OBJECT

private:
  Model *mModel;
  MainView *mView;
  size_t mResultTestIndex;

public:
  MainPresenter(Model *model, MainView *view);

  void proceedAnswer(QString answer, size_t index);
  void requestNewSession(bool force = false, bool continueSession = false);
  void requestSessionFinish(const ViewType *nextView = ViewType::MENU);
  void requestHistoryDetailUpdate(int index);
  void handleSession();
  void completeSession();
  void initView(const ViewType *type);
  void initApplication();
  void updateMenuTip();
  void nextResultTest();
  void prevResultTest();
  QString& getVersion();

private:
  void updateResultTestIndex(int delta);
public slots:
    void onSessionFinish(const ViewType *nextView);
    void onSessionError(QString message);
    void onProgressDone();
    void onError(QString message, bool fatal = true);
};
