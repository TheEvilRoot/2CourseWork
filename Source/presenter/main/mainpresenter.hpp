#pragma once

#include "model/model.hpp"
#include "view/mainview.hpp"
#include "model/loader/storeworker.hpp"
#include "model/loader/wordsfileloader.hpp"

#include <QString>
#include <QObject>

class MainPresenter: public QObject {

    Q_OBJECT

private:
  Model *mModel;
  MainView *mView;
  size_t mResultTestIndex;

  WordsFileLoader *mLoader;
  StoreWorker *mWorker;

public:
  MainPresenter(Model *model, MainView *view);

  void proceedAnswer(const QString& answer, size_t index);
  void requestNewSession(bool force = false, bool continueSession = false);
  void requestSessionFinish(const ViewType *nextView = ViewType::MENU);
  void requestHistoryDetailUpdate(int index);
  void handleSession();
  void completeSession();
  void initView(const ViewType *type);
  void initApplication();
  void updateMenuTip();
  QString& getVersion();

public slots:
    void onSessionFinish(const ViewType *nextView);
    void onSessionError(const QString& message);
    void onProgressDone();
    void onError(const QString& message, bool fatal = true);
};
