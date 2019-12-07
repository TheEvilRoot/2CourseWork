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

    Settings();

    bool loadStyles(QString *str);
};
