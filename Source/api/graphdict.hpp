#pragma once

#include "api/orderedset.hpp"
#include "api/mutablematrix.hpp"
#include "api/randomgenerator.hpp"

#include <vector>
#include <algorithm>
#include <QString>
#include <QStringList>

class GraphDict {
public:
    static int TYPE_UNRELATED;
    static int TYPE_TRANSLATION;
    static int TYPE_SYNONYM;

    explicit GraphDict(QString &source) {
        this->textLines = source.split("\n"); // splitString(source, '\n');
        this->graph = MutableSQIntMatrix(GraphDict::TYPE_UNRELATED);
    }

    explicit GraphDict(QStringList &lines): textLines(lines) {
        this->graph = MutableSQIntMatrix(GraphDict::TYPE_UNRELATED);
    }

    GraphDict() {
        this->graph = MutableSQIntMatrix(GraphDict::TYPE_UNRELATED);
    }

    void setSource(QString &source) {
        this->textLines = source.split("\n");
    }

    void setSource(QStringList &lines) {
        this->textLines = lines;
    }

    bool parse() {
        for (const auto& line : textLines) { parseLine(line); }

        for (size_t i = 0; i < getItemsCount(); i++) {
            auto tr = translationsFor(operator[](i));
            for (const auto &trIdx : tr) {
                auto trOfTr = translationsFor(trIdx);
                for (const auto &synIdx : trOfTr) {
                    graph.at(synIdx, i) = TYPE_SYNONYM;
                    graph.at(i, synIdx) = TYPE_SYNONYM;
                }
            }
        }

        return true;
    }

    size_t getItemsCount() { return items.size(); }

    SQIntMatrix& getGraph() { return graph; }

    size_t operator[](QString &str) { return items.indexOf(str); }

    QString operator[](size_t index) { return items[index]; }

    std::vector<size_t> translationsFor(size_t index) {
        std::vector<size_t> result;
        if (index == getItemsCount()) return result;

        for(size_t i = 0; i < getItemsCount(); i++) {
            if (i != index && graph.at(index, i) == GraphDict::TYPE_TRANSLATION)
                result.push_back(i);
        }
        return result;
    }

    std::vector<size_t> translationsFor(QString word) {
        size_t index = operator[](word);
        return translationsFor(index);
    }

    std::pair<size_t, size_t> randomPair(RandomGenerator *rand) {
        size_t randomWordIndex = rand->intInRange(getItemsCount());
        std::vector<size_t> wordTranslations = translationsFor(randomWordIndex);

        size_t randomTranslationIndex = wordTranslations[rand->intInRange(wordTranslations.size())];
        return { randomWordIndex, randomTranslationIndex };
    }

    std::vector<size_t> randomWords(RandomGenerator *rand, size_t count) {
        if (count > getItemsCount()) throw std::logic_error("Count greater then items count in graph");
        std::vector<size_t> all(getItemsCount());
        std::vector<size_t> result(count);

        for (size_t i = 0; i < getItemsCount(); i++) all[i] = i;

        for (size_t i = 0; i < count; i++) {
            size_t randomIndex = rand->intInRange(all.size());
            result[i] = all[randomIndex];
            std::iter_swap(all.begin() + randomIndex, all.end() - 1);
            all.pop_back();
        }

        return result;
    }

    // Check if word === to
    bool isRelated(QString word, QString to) {
        if (word == to) return true;

        size_t wordIndex = operator[](word);
        size_t toIndex = operator[](to);
        if (wordIndex == getItemsCount() || toIndex == getItemsCount()) return false;

        return graph.get(wordIndex, toIndex) == TYPE_SYNONYM;
    }

    // translation - is synonym to any word's translation
    bool isTranslation(QString word, QString translation) {
        size_t wordIndex = operator[](word);
        size_t translationIndex = operator[](translation);
        if (wordIndex == getItemsCount() || translationIndex == getItemsCount()) return false;

        auto wordTranslations = translationsFor(wordIndex);
        for (const auto &tr : wordTranslations) {
            if (graph.get(tr, translationIndex) == TYPE_SYNONYM) return true;
        }

        return false;
    }

private:
    QStringList textLines;
    OrderedSet<QString> items;
    MutableSQIntMatrix graph;

    bool parseLine(const QString& line) {
        QStringList args = line.split("\t"); // splitString(line, '\t');
        if (args.size() < 2) return false;

        QString key = args[0];
        QString value = args[1];

        size_t keyIndex = items.push(key);
        size_t valueIndex = items.push(value);

        graph.extendForIndex(std::max(keyIndex, valueIndex));

        graph.at(keyIndex, valueIndex) = TYPE_TRANSLATION;
        graph.at(valueIndex, keyIndex) = TYPE_TRANSLATION;

        return true;
    }
};

int GraphDict::TYPE_UNRELATED = 0;
int GraphDict::TYPE_SYNONYM = 1;
int GraphDict::TYPE_TRANSLATION = 2;
