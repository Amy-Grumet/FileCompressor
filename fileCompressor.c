#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileCompressor.h"

Node *createNode(char ch, int freq) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->ch = ch;
    node->freq = freq;
    node->left = node->right = NULL;
    return node;
}

typedef struct PriorityQueue {
    int size;
    int capacity;
    Node **array;
} PriorityQueue;

PriorityQueue *createPriorityQueue(int capacity) {
    PriorityQueue *pq = (PriorityQueue *)malloc(sizeof(PriorityQueue));
    pq->size = 0;
    pq->capacity = capacity;
    pq->array = (Node **)malloc(capacity * sizeof(Node *));
    return pq;
}

void swapNodes(Node **a, Node **b) {
    Node *temp = *a;
    *a = *b;
    *b = temp;
}

void minHeapify(PriorityQueue *pq, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < pq->size && pq->array[left]->freq < pq->array[smallest]->freq)
        smallest = left;

    if (right < pq->size && pq->array[right]->freq < pq->array[smallest]->freq)
        smallest = right;

    if (smallest != index) {
        swapNodes(&pq->array[smallest], &pq->array[index]);
        minHeapify(pq, smallest);
    }
}

Node *extractMin(PriorityQueue *pq) {
    Node *temp = pq->array[0];
    pq->array[0] = pq->array[pq->size - 1];
    pq->size--;
    minHeapify(pq, 0);
    return temp;
}

void insertPriorityQueue(PriorityQueue *pq, Node *node) {
    pq->size++;
    int i = pq->size - 1;

    while (i && node->freq < pq->array[(i - 1) / 2]->freq) {
        pq->array[i] = pq->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    pq->array[i] = node;
}

int isSingleNode(PriorityQueue *pq) {
    return pq->size == 1;
}

void generateCodes(Node *root, char *code, int top, char codes[256][256]) {
    if (root->left) {
        code[top] = '0';
        generateCodes(root->left, code, top + 1, codes);
    }
    if (root->right) {
        code[top] = '1';
        generateCodes(root->right, code, top + 1, codes);
    }
    if (!root->left && !root->right) {
        code[top] = '\0';
        strcpy(codes[(unsigned char)root->ch], code);
    }
}

void writeBit(FILE *out, int bit, unsigned char *Buffer, int *bitCount) {
    if (bit) *Buffer |= (1 << (7 - *bitCount));
    (*bitCount)++;
    if (*bitCount == 8) {
        fputc(*Buffer, out);
        *bitCount = 0;
        *Buffer = 0;
    }
}

void flushBitBuffer(FILE *out, unsigned char *Buffer, int *bitCount) {
    if (*bitCount > 0) {
        fputc(*Buffer, out);
    }
}

void compressFile(const char *inputFile, const char *outputFile, char codes[256][256]) {
    FILE *in = fopen(inputFile, "r");
    FILE *out = fopen(outputFile, "wb");

    if (!in || !out) {
        printf("Error opening files.\n");
        return;
    }

    unsigned char Buffer = 0;
    int bitCount = 0;

    char ch;
    while ((ch = fgetc(in)) != EOF) {
        char *code = codes[(unsigned char)ch];
        for (int i = 0; code[i] != '\0'; i++) {
            writeBit(out, code[i] - '0', &Buffer, &bitCount);
        }
    }
    flushBitBuffer(out, &Buffer, &bitCount);
    fclose(in);
    fclose(out);
}

PriorityQueue *buildPriorityQueue(int freq[]) {
    PriorityQueue *pq = createPriorityQueue(256);

    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            insertPriorityQueue(pq, createNode((char)i, freq[i]));
        }
    }
    return pq;
}

Node *buildHuffmanTree(int freq[]) {
    PriorityQueue *pq = buildPriorityQueue(freq);

    while (!isSingleNode(pq)) {
        Node *left = extractMin(pq);
        Node *right = extractMin(pq);
        Node *internal = createNode('\0', left->freq + right->freq);
        internal->left = left;
        internal->right = right;
        insertPriorityQueue(pq, internal);
    }
    return extractMin(pq);
}

void freeHuffmanTree(Node *root) {
    if (root == NULL) return;
    freeHuffmanTree(root->left);
    freeHuffmanTree(root->right);
    free(root);
}
