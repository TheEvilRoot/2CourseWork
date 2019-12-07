#include "model/settings.hpp"

Settings::Settings():
    versionMajor(1),
    versionMinor(3),
    versionBuild(201019),
    versionSign('A'),
    style(new QString),
    attemptsCount(1),
    randomTestsCount(true),
    totalTestsCount(25),
    wordsTestsCount(15),
    sentencesTestsCount(5) { loadStyles(style); }

bool Settings::loadStyles(QString *str) {
    auto *file = new QFile(":/styles/styles.css");
    if (!file->open(QIODevice::ReadOnly)) {
        return false;
    }

    auto *stream = new QTextStream(file);
    stream->setCodec("UTF-8");

    if (str)
        (*str) = stream->readAll();

    file->close();
s
    delete file;
    delete stream;

    return true;
}
