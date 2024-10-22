#ifndef FILECOMPRESSOR_H
#define FILECOMPRESSOR_H

typedef struct Node {
    char ch;
    int freq;
    struct Node *left, *right;
} Node;

Node *createNode(char ch, int freq);
Node *buildHuffmanTree(int freq[]);
void generateCodes(Node *root, char *code, int top, char codes[256][256]);
void compressFile(const char *inputFile, const char *outputFile, char codes[256][256]);
void decompressFile(const char *compressedFile, const char *outputFile, Node *root);

#endif
