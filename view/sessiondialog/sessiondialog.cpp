#include "sessiondialog.hpp"
#include "ui_sessiondialog.h"

#include <QDialogButtonBox>
#include <QPushButton>

SessionDialog::SessionDialog(
    MainPresenter *presenter,
    QWidget *parent
): QDialog(parent), ui(new Ui::SessionDialog), mPresenter(presenter) {
  ui->setupUi(this);
  initDialog();
}

void SessionDialog::initDialog() {
  connect(ui->dialogSubmitBox->button(QDialogButtonBox::Discard), &QPushButton::clicked, this, [=]() {
    mPresenter->requestNewSession(true, false);
    this->close();
  });
  connect(ui->dialogSubmitBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, [=]() {
    mPresenter->requestNewSession(false, true);
    this->close();
  });
}

SessionDialog::~SessionDialog() {
  delete ui;
}
