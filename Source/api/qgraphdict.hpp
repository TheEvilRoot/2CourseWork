#pragma once

#include "graphdict.hpp"

#include <QString>
#include <QStringList>
#include <vector>

class QGraphDict: public GraphDict<QString, QStringList, std::vector<size_t>, std::pair<size_t, size_t>> {
public:
    QGraphDict(): GraphDict([](QString& string, char del) {
        return string.split(del);
    }) { }
};
