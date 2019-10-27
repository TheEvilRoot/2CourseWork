#include "qresultwidget.hpp"

#include "api/utils.h"

QResultWidget::QResultWidget(SessionState *state) : mEmptyAnswer("<i>Пустой ответ</i>"), mIndex(0), mState(state) {
    QFont font;
    QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Minimum);
    sizePolicy3.setHorizontalStretch(0);
    sizePolicy3.setVerticalStretch(0);
    QSizePolicy sizePolicy8(QSizePolicy::Preferred, QSizePolicy::Minimum);
    sizePolicy8.setHorizontalStretch(0);
    sizePolicy8.setVerticalStretch(0);

    gridLayout = new QGridLayout();
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
    nextBtn = new QPushButton(this);
    nextBtn->setObjectName(QString::fromUtf8("nextBtn"));
    nextBtn->setMinimumSize(QSize(50, 50));
    nextBtn->setMaximumSize(QSize(50, 50));
    gridLayout->addWidget(nextBtn, 1, 2, 1, 1);
    prevBtn = new QPushButton(this);
    prevBtn->setObjectName(QString::fromUtf8("prevBtn"));
    prevBtn->setMinimumSize(QSize(50, 50));
    prevBtn->setMaximumSize(QSize(50, 50));
    gridLayout->addWidget(prevBtn, 1, 0, 1, 1);
    contentView = new QVBoxLayout();
    contentView->setObjectName(QString::fromUtf8("contentVIew"));
    questionLabel = new QLabel(this);
    questionLabel->setObjectName(QString::fromUtf8("questionLabel"));
    questionLabel->setSizePolicy(sizePolicy8);
    font.setPointSize(18);
    font.setBold(false);
    questionLabel->setFont(font);
    questionLabel->setScaledContents(false);
    questionLabel->setAlignment(Qt::AlignCenter);
    questionLabel->setWordWrap(true);
    questionLabel->setScaledContents(true);
    contentView->addWidget(questionLabel);
    userAnswerLabel = new QLabel(this);
    userAnswerLabel->setObjectName(QString::fromUtf8("userAnswerLabel"));
    sizePolicy3.setHeightForWidth(userAnswerLabel->sizePolicy().hasHeightForWidth());
    userAnswerLabel->setSizePolicy(sizePolicy3);
    font.setPointSize(15);
    font.setBold(false);
    userAnswerLabel->setFont(font);
    userAnswerLabel->setStyleSheet(QString::fromUtf8("color: red;"));
    userAnswerLabel->setAlignment(Qt::AlignCenter);
    userAnswerLabel->setWordWrap(true);
    contentView->addWidget(userAnswerLabel);
    correctLabel = new QLabel(this);
    correctLabel->setObjectName(QString::fromUtf8("correctLabel"));
    sizePolicy3.setHeightForWidth(correctLabel->sizePolicy().hasHeightForWidth());
    correctLabel->setSizePolicy(sizePolicy3);
    correctLabel->setAlignment(Qt::AlignCenter);
    correctLabel->setWordWrap(true);
    contentView->addWidget(correctLabel);
    gridLayout->addLayout(contentView, 1, 1, 1, 1);
    titleLabel = new QLabel(this);
    titleLabel->setObjectName(QString::fromUtf8("titleLabel"));
    sizePolicy3.setHeightForWidth(titleLabel->sizePolicy().hasHeightForWidth());
    titleLabel->setSizePolicy(sizePolicy3);
    font.setPointSize(16);
    font.setBold(true);
    titleLabel->setFont(font);
    titleLabel->setAlignment(Qt::AlignCenter);
    gridLayout->addWidget(titleLabel, 0, 1, 1, 1);

    setTextFor(nextBtn, ">");
    setTextFor(prevBtn, "<");
    connect(nextBtn, &QPushButton::clicked, this, [=]() { setNext(); });
    connect(prevBtn, &QPushButton::clicked, this, [=]() { setPrev(); });

    setLayout(gridLayout);

    updateView();
}

void QResultWidget::setNext() {
    if (mState == nullptr) return;

    auto count = mState->getCount();
    if (mIndex >= count - 1) return;

    mIndex++;
    emit next();

    updateView();
}

void QResultWidget::setPrev() {
    if (mState == nullptr) return;

    if (mIndex == 0) return;

    mIndex--;
    emit prev();

    updateView();
}

void QResultWidget::setState(SessionState *state) {
    mState = state;
    mIndex = 0;
    emit stateUpdate(state);
    updateView();
}

void QResultWidget::updateView() {
    if (mState == nullptr) {
       setVisible(false);
       return;
    }
    setVisible(true);
    auto result = mState->at(mIndex);
    if (result == nullptr) {
        setTextFor(titleLabel, "");
        setTextFor(userAnswerLabel, "");
        setTextFor(questionLabel, "");
        setTextFor(correctLabel, "");
        return;
    }

    setTextFor(titleLabel, "Тест " + QString::number(mIndex + 1) + "/" + QString::number(mState->getCount()));
    setTextFor(questionLabel, result->mQuestion);

    auto userAnswer = result->mUserAnswers.back();
    if (userAnswer.length() == 0) userAnswer = mEmptyAnswer;

    auto userAnswerStyle = "color: " + QString::fromUtf8(result->mPointsForTest ? "green;" : "red;");
    setTextFor(userAnswerLabel, userAnswer);
    userAnswerLabel->setStyleSheet(userAnswerStyle);

    auto correctAnswer = result->mAnswer;
    if (correctAnswer.length() == 0) correctAnswer = mEmptyAnswer;
    setTextFor(correctLabel, correctAnswer);
}
