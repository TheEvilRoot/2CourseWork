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

public:
  MainPresenter(Model *model, MainView *view);

  void proceedAnswer(QString answer, size_t index);
  void requestNewSession(bool force = false, bool continueSession = false);
  void requestSessionFinish();
  void requestHistoryDetailUpdate(int index);
  void handleSession();
  void completeTest();
  void initView(const ViewType *type);
  void initApplication();

  QString& getVersion();
public slots:
    void onSessionFinish();
    void onSessionError(QString message);
    void onProgressDone();
    void onError(QString message);
};
