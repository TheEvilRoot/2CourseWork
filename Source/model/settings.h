#pragma once

#include <QFile>
#include <QTextStream>
#include <iostream>

class Settings {
public:
    bool isAttemptMode;
    int versionMajor;
    int versionMinor;
    int versionBuild;
    char versionSign;
    QString *style;
    Settings(): isAttemptMode(false), versionMajor(1), versionMinor(1), versionBuild(51019), versionSign('A')  {
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
