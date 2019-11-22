#pragma once

#include "orderedset.hpp"
#include "mutablematrix.hpp"
#include "orderedsetlinkinterface.hpp"

#include <functional>

template<class String, class List, class IndexList, class IndexPair>
class GraphDict {
public:
    static int TYPE_UNRELATED;
    static int TYPE_TRANSLATION;
    static int TYPE_SYNONYM;

    explicit GraphDict(std::function<List(String&, char)> splitter, String &source): items(OrderedSetLinkInterface<String>(&keys, &values)) {
        this->textLines = splitter(source, '\n');
        init(splitter);
    }

    explicit GraphDict(std::function<List(String&, char)> splitter, List &lines): textLines(lines), items(OrderedSetLinkInterface<String>(&keys, &values)){
        init(splitter);
    }

    explicit GraphDict(std::function<List(String&, char)> splitter): items(OrderedSetLinkInterface<String>(&keys, &values)) {
        init(splitter);
    }

    void setSource(String &source) {
        this->textLines = splitter(source, '\n');
    }

    void setSource(List &lines) {
        this->textLines = lines;
    }

    size_t getItemsCount() { return items.getSize(); }

    SQIntMatrix& getGraph() { return graph; }

    size_t operator[](String &str) { return items.indexOf(str); }

    String operator[](size_t index) { return items[index]; }

    IndexList translationsFor(size_t index) {
        IndexList result;
        if (index == getItemsCount()) return result;

        for(size_t i = 0; i < getItemsCount(); i++) {
            if (i != index && graph.at(index, i) == GraphDict::TYPE_TRANSLATION)
                result.push_back(i);
        }
        return result;
    }

    IndexList translationsFor(String word) {
        size_t index = operator[](word);
        return translationsFor(index);
    }

    /**
     * Generate random pair of words (language independent)
     * @return pair of indeces
     */
    IndexPair randomPair() {
        size_t randomWordIndex = random() % getItemsCount();
        IndexList wordTranslations = translationsFor(randomWordIndex);

        size_t randomTranslationIndex = wordTranslations[random() % wordTranslations.size()];
        return { randomWordIndex, randomTranslationIndex };
    }

    /**
     * Generate random (language independent) words
     * @param count - count of random words. Should be less than getItemsCount()
     * @return vector of indeces
     * @throws std::logic_error if count > getItemsCount()
     */
    IndexList randomWords(size_t count) {
        if (count > getItemsCount()) throw std::logic_error("Count greater then items count in graph");
        IndexList all(getItemsCount());
        IndexList result(count);

        for (size_t i = 0; i < getItemsCount(); i++) all[i] = i;

        for (size_t i = 0; i < count; i++) {
            size_t randomIndex = random() % all.size();
            result[i] = all[randomIndex];
            std::iter_swap(all.begin() + randomIndex, all.end() - 1);
            all.pop_back();
        }

        return result;
    }

    bool isRelated(String word, String to) {
        if (word == to) return true;

        size_t wordIndex = operator[](word);
        size_t toIndex = operator[](to);
        if (wordIndex == getItemsCount() || toIndex == getItemsCount()) return false;

        return graph.get(wordIndex, toIndex) == TYPE_SYNONYM;
    }

    bool isTranslation(String word, String translation) {
        size_t wordIndex = operator[](word);
        size_t translationIndex = operator[](translation);
        if (wordIndex == getItemsCount() || translationIndex == getItemsCount()) return false;

        auto wordTranslations = translationsFor(wordIndex);
        for (const auto &tr : wordTranslations) {
            if (graph.get(tr, translationIndex) == TYPE_SYNONYM) return true;
        }

        return false;
    }

  bool parse() {
        parseLines(textLines);

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

private:
    List textLines;
    OrderedSet<String> keys;
    OrderedSet<String> values;
    OrderedSetLinkInterface<String> items;
    MutableSQIntMatrix graph;
    std::function<List(String&, char)> splitter;

    void init(std::function<List(String&, char)>& spl) {
        this->splitter = spl;
        this->graph = MutableSQIntMatrix(GraphDict::TYPE_UNRELATED);
    }

    void parseLines(List& lines, size_t index = 0) {
        if (index >= lines.size()) return;
        auto line = lines[index];

        List args = splitter(line, '\t');
        if (args.size() < 2) throw std::logic_error("Invalid line on index " + std::to_string(index) + ": " + line + ". It has " + std::to_string(args.size()) + " elements insted of 2");

        String key = args[0];
        String value = args[1];

        size_t keyIndex = keys.push(key);
        size_t valueIndex = values.push(value); // relative index!

        parseLines(lines, index + 1); // Yes, RECURSIVELY READ THE FILE

        // Here we already pushed all keys to ordered set and we have static length, so we can get absolute index of value in OSLI
        // Key index already absolute because key set is first in link interface
        size_t absValueIndex = valueIndex + keys.size();

        if (graph.getSize() < items.getSize())
            graph.extendForIndex(items.getSize()); // Will proceed only one time

        graph.at(keyIndex, absValueIndex) = TYPE_TRANSLATION;
        graph.at(absValueIndex, keyIndex) = TYPE_TRANSLATION;
    }
};

template<class String, class List, class IndexList, class IndexPair>
int GraphDict<String, List, IndexList, IndexPair>::TYPE_UNRELATED = 0;

template<class String, class List, class IndexList, class IndexPair>
int GraphDict<String, List, IndexList, IndexPair>::TYPE_SYNONYM = 1;

template<class String, class List, class IndexList, class IndexPair>
int GraphDict<String, List, IndexList, IndexPair>::TYPE_TRANSLATION = 2;
