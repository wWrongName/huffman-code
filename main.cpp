#include "huffman.hpp"
#include <cstring>
#define NUM_OF_ELEMENTS 7

void help() {
    cout << "In order to encode file you need write: ./huffman -i file0.bin -o file1.bin -m enc\n";
    cout << "In order to decode file you need write: ./huffman -i file1.bin -o file2.bin -m dec\n";
};

int main (int argc, char **argv) {
    char* input_name;
    char* output_name;
    char mode;
    if (argc != NUM_OF_ELEMENTS) {
        help();
        return 1;
    }
    for (int i = 1; i < NUM_OF_ELEMENTS; i++) {
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
            else {
                cout << "Wrong mode!\n";
                return 1;
            }
        }
        else {
            cout << "Wrong option!\n";
            return 1;
        }
    }
    HuffmanCoDec huffmanCoDec(input_name, output_name, mode);
    huffmanCoDec.start();
    huffmanCoDec.finish();
    return 0;
}