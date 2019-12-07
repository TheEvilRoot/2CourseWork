#pragma once

#include <QFile>
#include <QTextStream>

class Settings {
public:
    int versionMajor;
    int versionMinor;
    int versionBuild;
    char versionSign;
    QString *style;

    int attemptsCount;
    bool randomTestsCount;
    int totalTestsCount;
    int wordsTestsCount;
    int sentencesTestsCount;
    Settings():
        versionMajor(1),
        versionMinor(3),
        versionBuild(201019),
        versionSign('A'),
        style(new QString),
        attemptsCount(1),
        randomTestsCount(true),
        totalTestsCount(25),
        wordsTestsCount(15),
        sentencesTestsCount(5) {
        loadStyles(style);
    }

    bool loadStyles(QString *str) {
        auto *file = new QFile(":/styles/styles.css");
        if (!file->open(QIODevice::ReadOnly)) {
            return false;
        }
        auto *stream = new QTextStream(file);
        stream->setCodec("UTF-8");

        if (str) (*str) = stream->readAll();

        file->close();

        delete file;
        delete stream;
        return true;
    }
};
