#include "huffman.hpp"
#include <algorithm>
#include <limits>

void HuffmanTree::inc_weight() {
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
    auto symbol_it = find_if(alphabet.begin(), alphabet.end(), [&symbol](HuffmanTree* val) -> bool { 
        return val->get_symbol() == symbol; 
    });
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

int HuffmanCoDec::get_min_node() {
    int min = numeric_limits<int>::max();
    int i_min = 0;
    for (int i = 0; i < active_nodes.size(); i++) {
        if (min >= (*active_nodes.at(i))->get_weight()) {
            min = (*active_nodes.at(i))->get_weight();
            i_min = i;
        }
    }
    return i_min;
};

void HuffmanCoDec::make_tree() {
    fill_active_nodes();
    while (active_nodes.size() != 1) {
        int index_min_0 = get_min_node();
        auto tmp_it_0 = active_nodes.at(index_min_0);
        active_nodes.erase(active_nodes.begin() + index_min_0);

        int index_min_1 = get_min_node();
        auto tmp_it_1 = active_nodes.at(index_min_1);
        active_nodes.erase(active_nodes.begin() + index_min_1);
        
        additional_nodes.push_back(new HuffmanTree(*tmp_it_0, *tmp_it_1, '0')); // 0 - trash
        active_nodes.push_back(additional_nodes.end() - 1);
    }
}

void HuffmanCoDec::fill_active_nodes() {
    for (auto it = alphabet.begin(); it != alphabet.end(); it++)
        active_nodes.push_back(it);
};

void HuffmanCoDec::count_codes(HuffmanTree* node, string cur_code) {
    if (node->get_left() != nullptr)
        count_codes(node->get_left(), cur_code + "0");
    else if (node->get_right() != nullptr)
        count_codes(node->get_right(), cur_code + "1");
    else {}
    node->write_code(cur_code);
};

void HuffmanCoDec::start() {
    make_tree();
    count_codes(*active_nodes.at(0), "");
};