#include <iostream>
#include <fstream>
#include <vector>

class HuffmanTree {
    private:
        int weight;
        std::vector<char>::iterator value;
        HuffmanTree* left;
        HuffmanTree* right;
    
    public:
        HuffmanTree(int, std::vector<char>::iterator);
        std::vector<int> count_weights();

};

class HuffmanSymbol {
    private:
        char symbol;
        std::vector<char> code;
        int weight;
};

class HuffmanCoDec {
    public:
        HuffmanCoDec(std::ifstream, std::ofstream);

    private:
        std::ifstream fin;
        std::ofstream fout;
        std::vector<char> symbol; 
        std::vector<int> weight
};

int main (int argc, char **argv) {

}