#include "markov.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>

int main() {
    srand(time(0));

    const int MAX_WORDS = 5000;
    std::string words[MAX_WORDS];
    std::string prefixes[MAX_WORDS];
    std::string suffixes[MAX_WORDS];

    std::string filename;
    int order = 0;
    int maxGenerateWords = 0;

    std::cout << "Enter input filename: ";
    std::cin >> filename;

    std::cout << "Enter order (1, 2, or 3): ";
    while (!(std::cin >> order) || order < 1 || order > 3) {
        std::cout << "Invalid input. Order must be 1, 2, or 3: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cout << "Enter maximum number of words to generate: ";
    while (!(std::cin >> maxGenerateWords) || maxGenerateWords < order) {
        std::cout << "Invalid input. Requested count must be at least the order (" << order << "): ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    int wordCount = readWordsFromFile(filename, words, MAX_WORDS);
    if (wordCount == -1) {
        std::cout << "Error: Could not open file '" << filename << "'." << std::endl;
        return 1;
    }

    if (wordCount <= order) {
        std::cout << "Error: Not enough words in the file. At least " << (order + 1) << " training words are needed." << std::endl;
        return 1;
    }

    if (wordCount == MAX_WORDS) {
        std::cout << "Notice: Reached maximum capacity of " << MAX_WORDS << " words. Additional words were ignored." << std::endl;
    }

    int chainSize = buildMarkovChain(words, wordCount, order, prefixes, suffixes, MAX_WORDS);
    if (chainSize <= 0) {
        std::cout << "Error: Failed to build Markov chain from input." << std::endl;
        return 1;
    }

	
    std::string generatedTextOutput = generateText(prefixes, suffixes, chainSize, order, maxGenerateWords);

    int actualWordCount = 0;
    if (!generatedTextOutput.empty()) {
        actualWordCount = order;
        for (size_t i = 0; i < generatedTextOutput.length(); i++) {
            if (generatedTextOutput[i] == ' ') {
                actualWordCount++;
            }
        }
    }

    std::cout << "\n--- Generated Text ---" << std::endl;
    std::cout << generatedTextOutput << std::endl;
    std::cout << "\nGenerated " << actualWordCount << " of at most " << maxGenerateWords << " words.";

    if (actualWordCount < maxGenerateWords) {
        std::cout << " Stopped early: no successor found." << std::endl;
    }
    else {
        std::cout << std::endl;
    }

    return 0;
}