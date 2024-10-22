#include <stdio.h>
#include "fileCompressor.h"

long getFileSize(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Error opening file: %s\n", filename);
        return -1;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);
    return size;
}

int main(int argc, char *argv[]) {
    if(argc < 2){
        printf("Not enough input.\n");
        return 1;
    }
    int freq[256] = {0};
    const char *inputFileName = argv[1];
    FILE *file = fopen(inputFileName, "r");
    if (!file) {
        printf("Error opening input file.\n");
        return 1;
    }
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        freq[(unsigned char)ch]++;
    }
    fclose(file);
    Node *root = buildHuffmanTree(freq);
    char codes[256][256] = {0};
    char code[256];
    generateCodes(root, code, 0, codes);
    compressFile(inputFileName, "compressed.bin", codes);
    long inputSize = getFileSize(inputFileName);
    long compressedSize = getFileSize("compressed.bin");
    if (inputSize != -1 && compressedSize != -1) {
        printf("Input file size: %ld bytes\n", inputSize);
        printf("Compressed file size: %ld bytes\n", compressedSize);
    }
    return 0;
}
