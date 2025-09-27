
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
        for (char c : consonantChars) {
            consonantSet.insert(c);
        }
    }
    
public:
    PhoneticClassifier() {
        initializeCharacterSets();
        buildClassifier();
    }
    
    void buildClassifier() {
        // Create states
        classifier.AddState();
        classifier.SetStart(0);
        
        classifier.AddState(); // Vowel state (1)
        classifier.AddState(); // Consonant state (2)
        classifier.AddState(); // Other state (3)
        
        // Set final states with weights
        classifier.SetFinal(1, 0.0); // Vowel
        classifier.SetFinal(2, 0.0); // Consonant  
        classifier.SetFinal(3, 0.0); // Other
        
        // Add transitions for vowels
        for (char v : vowelSet) {
            classifier.AddArc(0, StdArc(v, 'V', 0.0, 1)); // Input char, output 'V', weight, dest
        }
        
        // Add transitions for consonants
        for (char c : consonantSet) {
            classifier.AddArc(0, StdArc(c, 'C', 0.0, 2));
        }
        
        // Default transition for other characters
        classifier.AddArc(0, StdArc(0, 'O', 0.0, 3)); // 0 = any character
    }
    
    char classifyCharacter(char input) {
        // Create input FST
        StdVectorFst inputFst;
        inputFst.AddState();
        inputFst.SetStart(0);
        inputFst.AddState();
        inputFst.SetFinal(1, 0.0);
        inputFst.AddArc(0, StdArc(input, input, 0.0, 1));
        
        // Compose with classifier
        StdVectorFst result;
        Compose(inputFst, classifier, &result);
        
        // Extract classification
        if (result.NumStates() > 0) {
            for (ArcIterator<StdVectorFst> aiter(result, 0); !aiter.Done(); aiter.Next()) {
                const StdArc &arc = aiter.Value();
                if (arc.olabel != 0) { // Output label
                    return static_cast<char>(arc.olabel);
                }
            }
        }
        return 'O'; // Other by default
    }
    
    void printClassification(const std::string& word) {
        std::cout << "Word: " << word << std::endl;
        std::cout << "Classification: ";
        for (char c : word) {
            std::cout << classifyCharacter(c) << " ";
        }
        std::cout << std::endl;
    }
};

// Example usage
int main() {
    PhoneticClassifier classifier;
    
    classifier.printClassification("Hello");
    classifier.printClassification("World");
    classifier.printClassification("AEIOU");
    
    return 0;
}
