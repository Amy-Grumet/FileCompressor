# File Compression Utility Using Huffman Coding
A file compression tool that utilizes Huffman coding. Huffman coding is a lossless data compression algorithm that assigns shorter codes to frequently occuring characters and longer codes to less frequent ones, resulting in file size reduction. 

## Features
  - Huffman Tree: Builds a huffman tree based on character frequencies
  - Priority Queue: Manages nodes using a min-heap structure to create the Huffman Tree
  - File Handling: Reads from an input file (.txt) and writes the compressed binary data to an output file. This includes file opening, reading, and writing operations. 
  - Binary Encoding: Compresses the input file by using the generated codes
    
## Compilation and Usage
### Prequisites
  - C Compiler
### How to Compile
  1. Clone the repository:
       git clone https://github.com/Amy-Grumet/FileCompressor.git
  2. Use the following command to compile:
       gcc -o huffman main.c fileCompressor.c
  3. Use the following command to compress the file, where the name of your input file is input:
       ./huffman <input.txt>

