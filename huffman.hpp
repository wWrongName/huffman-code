#ifndef __HUFFMAN__
#define __HUFFMAN__

#include <iostream>
#include <fstream>
#include <vector>
#include <deque>

#define ERROR 1
#define SUCCESS 0
enum mode  { encode, decode };

using namespace std;

class HuffmanTree {
    private:
        int weight;
        char symbol;
        string code;
        HuffmanTree* left;
        HuffmanTree* right;
    
    public:
        HuffmanTree(HuffmanTree* left_node, HuffmanTree* right_node, char smb) {
            left = left_node;
            right = right_node;
            if (left != nullptr)
                weight = left->weight + right->weight;
            else 
                weight = 1;
            symbol = smb;
        };
        void inc_weight();
        void write_code(vector<char>);

        char get_symbol() {
            return symbol;
        };
        int get_weight() {
            return weight;
        };
        string get_code() {
            return code;
        };
        void write_code(string c) {
            code = c;
        };
        int get_weight() {
            return weight;
        };
        HuffmanTree* get_left() {
            return left;
        };
        HuffmanTree* get_right() {
            return right;
        };
};

class HuffmanCoDec {
    public:
        HuffmanCoDec(string, string, char);
        int read_bin_file(string&);
        void add_new_symbol_into_alphabet(char symbol) {
            alphabet.push_back(new HuffmanTree(nullptr, nullptr, symbol));
        };
        void count_symbol_weight(char);
        void start();
        void finish();

    private:
        int get_min_node();
        void make_tree();
        void count_codes(HuffmanTree*, string);
        void fill_active_nodes();
        char mode;
        vector<HuffmanTree*> alphabet;
        vector<HuffmanTree*> additional_nodes;
        vector<vector<HuffmanTree*>::iterator> active_nodes;
        ifstream fin;
        ofstream fout;
};

#endif