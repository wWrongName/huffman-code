#include "huffman.hpp"
#include <algorithm>
#include <cstdio>
#include <limits>

void print_active_nodes(vector<HuffmanTree*> a_n) {
    cout << "\nACTIVE NODES:\n";
    for (auto it = a_n.begin(); it != a_n.end(); it++)
        cout << "symbol " << (int)(*it)->get_symbol() << ", code " << (*it)->get_code() << "\n";
};

void print_alphabet(vector<HuffmanTree*> alphabet) {
    cout << "\nALPHABET:\n";
    for (auto it = alphabet.begin(); it != alphabet.end(); it++)
        cout << "symbol " << (int)(*it)->get_symbol() << ", code " << (*it)->get_code() << ", w: " << (*it)->get_weight() << "\n";
};

HuffmanCoDec::HuffmanCoDec(char* input, char* output, char m) {
    fin = fopen(input, "rb");
    if (fin == NULL) {
        cout << "Can't open input file!\n";
    }
    fout = fopen(output, "wb");
    if (fout == NULL) {
        cout << "Can't open output file!\n";
        fclose(fin);
    }
    mode = m;
};

void HuffmanCoDec::read_bin_file() {
    char tmp_buff;
    while (!feof(fin)) {
        fread(&tmp_buff, 1, 1, fin);
        data.push_back(tmp_buff);
        if (mode == encode)
            count_symbol_weight(tmp_buff);
    }
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
        active_nodes.push_back(*(alphabet.end() - 1));
    }
};

int HuffmanCoDec::make_tree() {
    while (true) {
        int index_min_0 = get_min_node();
        auto tmp_it_0 = active_nodes.at(index_min_0);

        int index_min_1 = get_min_node();
        if (index_min_1 == -1) {
            return index_min_0;
        }
        auto tmp_it_1 = active_nodes.at(index_min_1);

        additional_nodes.push_back(new HuffmanTree(tmp_it_0, tmp_it_1, '0')); // 0 - trash

        active_nodes.push_back(*(additional_nodes.end() - 1));
    }
};

int HuffmanCoDec::get_min_node() {
    int min = numeric_limits<int>::max();
    int i_min = 0;
    for (int i = 0; i < active_nodes.size(); i++) {
        if (min > active_nodes.at(i)->get_weight() && active_nodes.at(i)->isOpen()) {
            min = active_nodes.at(i)->get_weight();
            i_min = i;
        }
    }
    if (min != numeric_limits<int>::max()) {
        active_nodes.at(i_min)->close(); 
        return i_min;
    }
    else
        return -1;
};

void HuffmanCoDec::count_codes(HuffmanTree* node, string cur_code) {
    if (node->get_left() != nullptr)
        count_codes(node->get_left(), cur_code + "0");
    if (node->get_right() != nullptr)
        count_codes(node->get_right(), cur_code + "1");
    node->write_code(cur_code);
};

void HuffmanCoDec::write_alphabet_into_file() {
    vector<char> table;
    for (auto it = alphabet.begin(); it != alphabet.end(); it++) {
        table.push_back((char)((*it)->get_code().length())); // code size

        auto tmp_vector = get_one_code((*it)->get_code());
        table.insert( table.end(), tmp_vector.begin(), tmp_vector.end() );

        table.push_back((*it)->get_symbol()); // original symbol
    }
    char size[2];
    size[0] = (short)table.size() & 0x00FF;
    size[1] = (short)table.size() >> 8;
    
    fwrite(size, 1, 2, fout);
    for (auto it = table.begin(); it != table.end(); it++) {
        // for (auto i = 7; i >= 0; i--)
        //     cout << (00000001 & (*it >> i));
        // cout << "\n";
        fwrite(&(*it), 1, 1, fout);
    };
};

vector<char> HuffmanCoDec::get_one_code(string code) {
    float code_size = code.length();
    int amount_of_chars = (int)(code_size / 8.0 + 0.9);
    vector<char> one_code;
    for (int i = 0; i < amount_of_chars; i++) {
        char tmp_size = ((int)code_size - i*8);
        if (tmp_size > 8)
            one_code.push_back(get_byte(code.substr(i*8, 8)));
        else
            one_code.push_back(get_byte(code.substr(i*8, tmp_size)));
    }
    return one_code;
}

char HuffmanCoDec::get_byte(string code) {
    char one_byte_code = 0;
    char mask = 1;
    for (int i = 0; i < code.length(); i++)
        if (code[i] == '1')
            one_byte_code |= (mask << (i));
    free_size = 8 - code.length();
    return one_byte_code;
};

vector<char>::iterator HuffmanCoDec::read_alphabet() {
    short size = ((short)data[0] | ((short)data[1] << 8));
    // for (auto iter = data.begin() + 2; iter != data.end(); data++) {
    //     for (auto i = 7; i >= 0; i--)
    //         cout << (00000001 & (*iter >> i));
    //     cout << "\n";
    // }
    auto iter = data.begin() + 2;
    for ( ; iter != data.end() && size; ) {
        char code_length = *iter++;
        size--;
        if (iter == data.end() || size == 0)
            return iter;
        char chars_num = (char)((float)code_length / 8.0 + 0.9);
        string code = "";
        for (int i = 0; i < 1; i++) {
            char symbol_part = *iter++;
            size--;
            if (iter == data.end() || size == 0)
                return iter;
            for (auto j = 0; j < 8; j++)
                code += (00000001 & (symbol_part >> j)) + 48;
            code.erase(code.begin() + code_length, code.end());
        }
        code_length = *iter++;
        size--;
        add_new_symbol_into_alphabet(code_length);
        (*(alphabet.end() - 1))->write_code(code);
    }
    return iter;
};

HuffmanTree* HuffmanCoDec::find_by_symbol(char f_symbol) {
    auto symbol_it = find_if(alphabet.begin(), alphabet.end(), [&f_symbol](HuffmanTree* val) -> bool { 
        return val->get_symbol() == f_symbol;
    });
    return *symbol_it;
};

template <typename T0> void HuffmanCoDec::write_vector_into_file(vector<T0> data_vector) {
    for (auto it = data_vector.begin(); it != data_vector.end(); it++)
        fwrite(&(*it), 1, 1, fout);
};

// void HuffmanCoDec::pack_into_one_dyte(vector<char>::iterator last_symbol, char new_symbol, char size) {
//     *last_symbol |= (new_symbol >> (8 - size));
// };

// void HuffmanCoDec::vector_shift_left(vector<char>& vc, char size) {
//     for (auto it = vc.begin(); it != vc.end(); it++) {
//         *it = *it << size;
//         if ((it + 1) != vc.end())
//             *it |= (*(it + 1) >> size);
//     }
//     if (size >= free_size)
//         vc.pop_back();
// };

void HuffmanCoDec::encode_and_write_into_file() {
    auto it = data.begin();
    auto letter_obj = find_by_symbol(*it++); // get letter(object HuffmanTree) from alphabet
    auto output = get_one_code(letter_obj->get_code())[0];
    char sub_output;
    char shift_place = 0;
    vector<char> output_vec;

    while (it != data.end()) {
        if (free_size > 0) {
            char tmp_size;
            while (free_size > 0) {
                tmp_size = free_size;
                letter_obj = find_by_symbol(*it++);
                if (it == data.end())
                    break;
                sub_output = get_one_code(letter_obj->get_code())[0];
                output |= sub_output << (8 - tmp_size);
                shift_place = tmp_size;
                free_size = tmp_size - (8 - free_size);
            }
        }
        else if (free_size < 0) {
            for (auto j = 7; j >= 0; j--)
                cout << (00000001 & (output >> j));
            cout << "\n";
            output_vec.push_back(output);
            output = sub_output >> shift_place;
            free_size = 8 + free_size;
        }
        else {
            for (auto j = 7; j >= 0; j--)
                cout << (00000001 & (output >> j));
            cout << "\n";
            output_vec.push_back(output);
            letter_obj = find_by_symbol(*it++);
            output = get_one_code(letter_obj->get_code())[0];
            shift_place = 0;
        }
    }
    write_vector_into_file(output_vec);
};

HuffmanTree* HuffmanCoDec::check_alphabet(string code_buff) {
    auto code_it = find_if(alphabet.begin(), alphabet.end(), [&code_buff](HuffmanTree* val) -> bool { 
        return val->get_code() == code_buff;
    });
    if (code_it != alphabet.end()) {
        return *code_it;
    }
    return nullptr;
};

void HuffmanCoDec::decode_and_write_into_file(vector<char>::iterator it) {
        // for (auto i = 0; i < 8; i++)
        //     cout << (00000001 & (*it >> i));
        // cout << "\n";
    string code_buff = "";
    for ( ; it != data.end(); it++) {
        for (int sub_it = 0; sub_it < 8; sub_it++) { 
            code_buff += ((*it >> sub_it) & 00000001) + 48;
            auto out_struct = check_alphabet(code_buff);
            if (out_struct != nullptr) {
                char out_char = out_struct->get_symbol();
                fwrite(&out_char, 1, 1, fout);
                code_buff = "";
            }
        }
    }
};

void HuffmanCoDec::start() {
    if (mode == encode) {
        read_bin_file();
        count_codes(active_nodes.at(make_tree()), "");
        print_alphabet(alphabet);
        write_alphabet_into_file();
        encode_and_write_into_file();
    }
    else {
        read_bin_file();
        decode_and_write_into_file(read_alphabet());
        print_alphabet(alphabet);
    }
};

void HuffmanCoDec::finish() {
    if (fin != NULL)
        fclose(fin);
    if (fout != NULL)
        fclose(fout);
};