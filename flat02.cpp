#include <fst/fstlib.h>
#include <iostream>
#include <string>
#include <unordered_set>

using namespace fst;

class PhoneticClassifier {
private:
    StdVectorFst classifier;
    std::unordered_set<char> vowelSet;
    std::unordered_set<char> consonantSet;
    
    void initializeCharacterSets() {
        // Define vowel characters
        const std::string vowelChars = "aeiouAEIOU";
        for (char v : vowelChars) {
            vowelSet.insert(v);
        }
        
        // Define consonant characters  
        const std::string consonantChars = "bcdfghjklmnpqrstvwxyzBCDFGHJKLMNPQRSTVWXYZ";
