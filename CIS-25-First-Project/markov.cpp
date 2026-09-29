#include "markov.h"
#include <fstream>
#include <cstdlib>

std::string joinWords(const std::string words[], int startIndex, int count) {
    std::string result = "";
    for (int i = 0; i < count; i++) {
        result += words[startIndex + i];
        if (i < count - 1) {
            result += " ";
        }
    }
    return result;
}

int readWordsFromFile(std::string filename, std::string words[], int maxWords) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        return -1;
    }

    int counter = 0;
    while (counter < maxWords && inputFile >> words[counter]) {
        counter++;
    }

    inputFile.close();
    return counter;
}

int buildMarkovChain(const std::string words[], int numWords, int order,
    std::string prefixes[], std::string suffixes[],
    int maxChainSize) {
    if (order < 1 || order > 3 || numWords <= order || maxChainSize <= 0) {
        return 0;
    }

    int count = 0;

    for (int i = 0; i < numWords - order && count < maxChainSize; i++) {
        prefixes[count] = joinWords(words, i, order);
        suffixes[count] = words[i + order];
        count++;
    }

    return count;
}

std::string getRandomSuffix(const std::string prefixes[], const std::string suffixes[], int chainSize, std::string currentPrefix) {
    int matchCount = 0;

    for (int i = 0; i < chainSize; i++) {
        if (prefixes[i] == currentPrefix) {
            matchCount++;
        }
    }

    if (matchCount == 0) {
        return "";
    }

    int targetMatch = rand() % matchCount;
    int currentMatch = 0;

    for (int i = 0; i < chainSize; i++) {
        if (prefixes[i] == currentPrefix) {
            if (currentMatch == targetMatch) {
                return suffixes[i];
            }
            currentMatch++;
        }
    }

    return "";
}

std::string getRandomPrefix(const std::string prefixes[], int chainSize) {
    if (chainSize <= 0) {
        return "";
    }
    int randomIndex = rand() % chainSize;
    return prefixes[randomIndex];
}

std::string generateText(const std::string prefixes[], const std::string suffixes[], int chainSize, int order, int numWords) {
    if (chainSize <= 0 || numWords <= 0 || order < 1) {
        return "";
    }

    std::string currentPrefix = getRandomPrefix(prefixes, chainSize);
    std::string result = currentPrefix;
    int wordCount = order;

    std::string window[10];
    int wIndex = 0;
    std::string tempWord = "";
    for (size_t i = 0; i < currentPrefix.length(); i++) {
        if (currentPrefix[i] == ' ') {
            if (wIndex < order) {
                window[wIndex++] = tempWord;
            }
            tempWord = "";
        }
        else {
            tempWord += currentPrefix[i];
        }
    }
    if (wIndex < order) {
        window[wIndex++] = tempWord;
    }

    while (wordCount < numWords) {
        std::string suffix = getRandomSuffix(prefixes, suffixes, chainSize, currentPrefix);
        if (suffix == "") {
            break;
        }

        result += " " + suffix;
        wordCount++;

        for (int i = 0; i < order - 1; i++) {
            window[i] = window[i + 1];
        }
        window[order - 1] = suffix;

        currentPrefix = joinWords(window, 0, order);
    }

    return result;
}