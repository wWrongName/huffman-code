#include "huffman.hpp"
#include <algorithm>

HuffmanSymbol::HuffmanSymbol(char smb) {
    symbol = smb;
    weight = 1;
};

void HuffmanSymbol::inc_weight() {
    weight++;
};

HuffmanCoDec::HuffmanCoDec(string input, string output, char m) {
    fin.open(input, ios_base::binary); 
    if (!fin.is_open()) {
        cout << "Can't open input file!\n";
    }
    fout.open(output, ios_base::binary);
    if (!fout.is_open()) {
        cout << "Can't open output file!\n";
    }
    mode = m;
};

void HuffmanCoDec::count_symbol_weight(char symbol) {
    auto symbol_it = find_if(alphabet.begin(), alphabet.end(), [&symbol](HuffmanSymbol* val) -> bool { return val->get_symbol() == symbol; });
    if (symbol_it != alphabet.end()) {
        (*symbol_it)->inc_weight();
    }
    else {
        add_new_symbol_into_alphabet(symbol);
    }
};

int HuffmanCoDec::read_bin_file(string& input_name) {
    char tmp_buff;
    deque<char> buff;

    while (!fin.eof()) {
        fin >> tmp_buff;
        buff.push_back(tmp_buff);
        count_symbol_weight(tmp_buff);
    }
    return SUCCESS;
};