#ifndef __HUFFMAN__
#define __HUFFMAN__

#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#define ERROR 1
#define SUCCESS 0

enum modes    { encode, decode };
enum statuses { opened, closed };

using namespace std;

class HuffmanTree {
    private:
        int weight;
        char symbol;
        bool status;
        string code;
        HuffmanTree* left;
        HuffmanTree* right;
    
    public:
        HuffmanTree(HuffmanTree* left_node, HuffmanTree* right_node, char smb) {
            left = left_node;
            right = right_node;
            if (left != nullptr && right != nullptr)
                weight = left->weight + right->weight;
            else 
                weight = 1;
            symbol = smb;
            status = opened;
        };
        void inc_weight() {
            weight++;
        };
        bool isOpen() {
            return status == opened;
        };
        void close() {
            status = closed;
        };
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
        HuffmanTree* get_left() {
            return left;
        };
        HuffmanTree* get_right() {
            return right;
        };
};

class HuffmanCoDec {
    public:
        HuffmanCoDec(char*, char*, char);
        void start();
        void finish();

    private:
        void add_new_symbol_into_alphabet(char symbol) {
            alphabet.push_back(new HuffmanTree(nullptr, nullptr, symbol));
        };
        HuffmanTree* check_alphabet(string);
        void pack_into_one_dyte(vector<char>::iterator, char, char);
        template <typename T0> void write_vector_into_file(vector<T0>);
        void vector_shift_left(vector<char>&, char);
        vector<char> get_one_code(string);
        HuffmanTree* find_by_symbol(char);
        void count_codes(HuffmanTree*, string);
        void encode_and_write_into_file();
        void decode_and_write_into_file(vector<char>::iterator);
        void write_alphabet_into_file();
        void count_symbol_weight(char);
        char get_byte(string);
        vector<char>::iterator read_alphabet();
        void read_bin_file();
        int get_min_node();
        int make_tree();
        
        vector<HuffmanTree*> alphabet;
        vector<HuffmanTree*> additional_nodes;
        vector<HuffmanTree*> active_nodes;
        vector<char> data;
        char free_size;
        char mode;
        FILE *fin;
        FILE *fout;
};

#endif