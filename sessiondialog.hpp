#ifndef SESSIONDIALOG_HPP
#define SESSIONDIALOG_HPP

#include <QDialog>

#include "mainpresenter.hpp"

namespace Ui {
  class SessionDialog;
}

class SessionDialog : public QDialog {
  Q_OBJECT

public:
  explicit SessionDialog(MainPresenter *presenter, QWidget *parent = nullptr);
  ~SessionDialog();

private:
  Ui::SessionDialog *ui;
  MainPresenter *mPresenter;

  void initDialog();
};

#endif // SESSIONDIALOG_HPP
