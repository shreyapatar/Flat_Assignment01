#include <fst/fstlib.h>
#include <iostream>
#include <string>
#include <vector>

using namespace fst;

class TextProcessor {
private:
    StdVectorFst transformer;
    
public:
    TextProcessor() {
        buildTransducer();
    }
    
private:
    void buildTransducer() {
        // Create states - using a loop-based approach
        std::vector<int> states;
        for(int i = 0; i < 3; i++) {
            states.push_back(transformer.AddState());
        }
        
        transformer.SetStart(states[0]);
        transformer.SetFinal(states[2], TropicalWeight::One());
        
        // Build character mappings
        setupCharacterTransformations(states);
    }
    
    void setupCharacterTransformations(const std::vector<int>& states) {
        // Process lowercase letters with different logic
        for(int i = 0; i < 26; i++) {
            char lower = 'a' + i;
            char upper = 'A' + i;
            
            // Two-step path for lowercase conversion
            transformer.AddArc(states[0], StdArc(lower, upper, TropicalWeight::One(), states[1]));
            transformer.AddArc(states[1], StdArc(0, 0, TropicalWeight::One(), states[2]));
        }
        
        // Direct path for uppercase (different approach)
        for(char c = 'A'; c <= 'Z'; c++) {
            transformer.AddArc(states[0], StdArc(c, c, TropicalWeight::One(), states[2]));
        }
        
        // Numeric characters with alternative path
        for(char d = '0'; d <= '9'; d++) {
            transformer.AddArc(states[0], StdArc(d, d, TropicalWeight::One(), states[2]));
        }
        
        // Special characters
        char specials[] = {' ', ',', '.', '!', '?'};
        for(char sc : specials) {
            transformer.AddArc(states[0], StdArc(sc, sc, TropicalWeight::One(), states[2]));
        }
    }
    
public:
    std::string processText(const std::string& input) {
        if(input.empty()) return "";
        
        std::string result;
        result.reserve(input.length());
        
        // Process each character with state tracking
        int currentState = transformer.Start();
        
        for(char ch : input) {
            bool transformed = false;
            
            // Check possible transitions from current state
            for(ArcIterator<StdVectorFst> arc_iter(transformer, currentState); 
                !arc_iter.Done(); arc_iter.Next()) {
                
                const StdArc& arc = arc_iter.Value();
                if(arc.ilabel == ch) {
                    if(arc.olabel != 0) {
                        result += static_cast<char>(arc.olabel);
                    }
                    currentState = arc.nextstate;
                    transformed = true;
                    break;
                }
            }
            
            if(!transformed) {
                // Keep character as-is if no transformation rule
                result += ch;
                currentState = transformer.Start();
            }
        }
        
        return result;
    }
};

int main() {
    TextProcessor processor;
    
    std::cout << "Text Conversion Tool\n";
    std::cout << "====================\n";
    
    while(true) {
        std::string userInput;
        std::cout << "\nEnter text (or 'quit' to exit): ";
        std::getline(std::cin, userInput);
        
        if(userInput == "quit") break;
        
        std::string converted = processor.processText(userInput);
        std::cout << "Original: " << userInput << "\n";
        std::cout << "Converted: " << converted << "\n";
    }
    
    std::cout << "Processing complete.\n";
    return 0;
}
