#pragma once

class Settings {
public:
    bool isAttemptMode;
    int versionMajor;
    int versionMinor;
    int versionBuild;
    char versionSign;
    Settings(): isAttemptMode(false), versionMajor(1), versionMinor(1), versionBuild(51019), versionSign('A')  {}
};
