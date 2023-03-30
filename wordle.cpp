#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void buildWords(
    std::string word, 
    size_t pos, 
    const std::string& in, 
    const std::string& floating, 
    std::set<std::string>& possibleWords);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    std::set<std::string> possibleWords;
    std::string emptyWord(in.size(), '-');

    buildWords(emptyWord, 0, in, floating, possibleWords);

    std::set<std::string> solutions;
    for (std::set<std::string>::iterator it = possibleWords.begin(); it != possibleWords.end(); ++it) {
        if (dict.find(*it) != dict.end()) {
            solutions.insert(*it);
        }
    }
    return solutions;
}

// Define any helper functions here
void buildWords(
    std::string word, 
    size_t pos, 
    const std::string& in, 
    const std::string& floating, 
    std::set<std::string>& possibleWords)
{
    // base case: we've built up the whole word.
    if (pos == word.size()) {
        // check if the word meets floating point constraints.
        for (size_t i = 0; i < floating.size(); i++) {
            bool foundLetter = false;
            for (size_t j = 0; j < word.size(); j++) {
                if (floating[i] == word[j]) {
                    foundLetter = true;
                    break;
                }
            }
            if (!foundLetter) {
                return;
            }
        }
        possibleWords.insert(word);
        return;
    }

    // add letter to word
    if (in[pos] != '-') {
        word[pos] = in[pos];
        buildWords(word, pos+1, in, floating, possibleWords);
    }
    else {
        for (char c = 'a'; c < 'z'+1; c++) {
            word[pos] = c;
            buildWords(word, pos+1, in, floating, possibleWords);
        }
    }
}