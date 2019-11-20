#pragma once

#include <QFile>
#include <QTextStream>
#include <iostream>

class Settings {
public:
    int versionMajor;
    int versionMinor;
    int versionBuild;
    char versionSign;
    QString *style;

    int attemptsCount;
    int wordsTestsCount;
    int sentencesTestsCount;
    Settings(): versionMajor(1), versionMinor(3), versionBuild(201019), versionSign('A'), style(new QString), attemptsCount(1), wordsTestsCount(15), sentencesTestsCount(5) {
        loadStyles(style);
    }

    bool loadStyles(QString *str) {
        QFile *file = new QFile(":/styles/styles.css");
        if (!file->open(QIODevice::ReadOnly)) {
            std::cerr << "Unable to open css file: " + file->errorString().toStdString() << "\n";
            return false;
        }
        QTextStream *stream = new QTextStream(file);
        stream->setCodec("UTF-8");

        (*str) = stream->readAll();

        file->close();
        if (file) delete file;
        if (stream) delete stream;

        return true;
    }
};
