#include "huffman.hpp"
#include <cstring>

void help() {
    cout << "In order to encode file you need write: ./huffman -i file0.bin -o file1.bin -m enc\n";
    cout << "In order to encode file you need write: ./huffman -i file1.bin -o file2.bin -m dec\n";
};

int main (int argc, char **argv) {
    string input_name;
    string output_name;
    char mode;
    if (argc != 5) {
        help();
        return 1;
    }
    for (int i = 1; i < 5; i++) {
        if (!strcmp(argv[i], "-i")) {
            input_name = argv[++i];
        }
        else if (!strcmp(argv[i], "-o")) {
            output_name = argv[++i];
        }
        else if (!strcmp(argv[i], "-m")) {
            if (!strcmp(argv[++i], "enc"))
                mode = encode;
            else if (!strcmp(argv[i], "dec"))
                mode = decode;
            else 
                cout << "Wrong mode!\n";
        }
        else {
            cout << "Wrong option!\n";
        }
    }
    HuffmanCoDec huffmanCoDec(input_name, output_name, mode);
    huffmanCoDec.start();
    huffmanCoDec.finish();
}