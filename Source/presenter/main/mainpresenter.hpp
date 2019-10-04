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
  void handleSession();
  void completeTest();
  void initView(const ViewType *type);
  void initApplication();

public slots:
    void onSessionFinish();
    void onSessionError(QString message);
    void onProgressDone();
    void onError(QString message);
};
