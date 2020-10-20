#ifndef __HUFFMAN__
#define __HUFFMAN__

#include <iostream>
#include <fstream>
#include <vector>
#include <deque>

#define ERROR 1
#define SUCCESS 0
enum mode { encode, decode };

using namespace std;

class HuffmanTree {
    private:
        int weight;
        vector<char>::iterator value;
        HuffmanTree* left;
        HuffmanTree* right;
    
    public:
        HuffmanTree(int, vector<char>::iterator);

};

class HuffmanSymbol {
    private:
        char symbol;
        vector<char> code;
        int weight;

    public:
        HuffmanSymbol(char);
        void inc_weight();
        void write_code(vector<char>);

        char get_symbol() {
            return symbol;
        };
        int get_weight() {
            return weight;
        };
        vector<char> get_code() {
            return code;
        };
};

class HuffmanCoDec {
    public:
        HuffmanCoDec(string, string, char);
        int read_bin_file(string&);
        void add_new_symbol_into_alphabet(char symbol) {
            alphabet.push_back(new HuffmanSymbol(symbol));
        };
        void count_symbol_weight(char);
        void start();
        void finish();

    private:
        char mode;
        vector<HuffmanSymbol*> alphabet;
        ifstream fin;
        ofstream fout;
        vector<char> symbol; 
        vector<int> weight;
};

#endif