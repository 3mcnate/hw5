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


#define DEBUG


// Add prototypes of helper functions here
void buildWords(
    std::string word, 
    size_t pos, 
    const std::string& in, 
    int fixedLettersRemaining,
    std::string& floating, 
    std::set<std::string>& solutions, 
    const std::set<std::string>& dict);

// Definition of primary wordle function
std::set<std::string> wordle(const std::string& in, const std::string& floating, const std::set<std::string>& dict)
{
    std::set<std::string> solutions;
    std::string emptyWord(in.size(), '-');
    
    int numFixedLetters = 0;
    for (size_t i = 0; i < in.size(); i++) {
        if (in[i] != '-') numFixedLetters++;
    }

    std::string floatingCopy(floating);

    buildWords(emptyWord, 0, in, numFixedLetters, floatingCopy, solutions, dict);
    return solutions;
}

// Define any helper functions here
void buildWords(
    std::string word, 
    size_t pos, 
    const std::string& in, 
    int fixedLettersRemaining,
    std::string& floating, 
    std::set<std::string>& solutions,
    const  std::set<std::string>& dict)
{
    // base case: we've built up the whole word.
    if (pos == word.size()) {
        // check if word exists
        if (dict.find(word) != dict.end()) {
            solutions.insert(word);
        }
        return;
    }

    // add letter to word
    if (in[pos] != '-') {
        word[pos] = in[pos];
        buildWords(word, pos+1, in, fixedLettersRemaining-1, floating, solutions, dict);
    }
    else {
        size_t availableLetters = word.size() - pos - fixedLettersRemaining;
        
        // the current word has no hope of containing all necessary floating letters
        if (availableLetters < floating.size()) {
            return;
        }

        // all remaining letters must be from the floating letters set
        if (availableLetters == floating.size()) {
            
            std::string floatingCopy(floating);
            size_t erasePos = 0;

            std::set<char> repeatLetters;

            for (size_t i = 0; i < floating.size(); i++) {
                // check if we've already tried this letter
                if (repeatLetters.find(floating[i]) != repeatLetters.end()) {
                    erasePos++;
                    continue;
                }

                word[pos] = floating[i];
                repeatLetters.insert(floating[i]);

                // erase the used letter and pass on to recurse, then add it back for the next iteration
                floatingCopy.erase(erasePos, 1);
                buildWords(word, pos+1, in, fixedLettersRemaining, floatingCopy, solutions, dict);
                floatingCopy.push_back(floating[i]);
            }
            return;
        }

        // else, just go through all possible letters
        for (char c = 'a'; c <= 'z'; c++) {
            word[pos] = c;

            // if we're guessing a letter in floating, we need to remove it.
            bool floatingLetter = false;
            size_t floatingPos = 0;
            for (size_t i = 0; i < floating.size(); i++) {
                if (floating[i] == c) {
                    floatingLetter = true; 
                    floatingPos = i;
                    break;
                }
            }

            if (floatingLetter) floating.erase(floatingPos, 1);
            buildWords(word, pos+1, in, fixedLettersRemaining, floating, solutions, dict);
            if (floatingLetter) floating.push_back(c);
        }
    }
}