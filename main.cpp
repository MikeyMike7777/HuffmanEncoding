#include "HuffmanTree.h"
#include <fstream>
#include <cstring>
#include <cmath>

using namespace std;

/* main
*  parameters:
*      argc -- the number of arguments from the command line
*      argv -- command <source> <destination>
*  return value: 0 (indicating a successful run)
*
* This function facilitates the ability to huff and unhuff a file using
* Huffman Coding and Huffman Trees.
*
* EOF character: '~'
* Magic Number: 125 or '}'
*/
int main(int argc, char* argv[]) {
    ifstream inputFile;
    ofstream outputFile;
    string text = "";
    
    argc = 3;
    argv[0] = "-unhuff";
    argv[1] = "huffed.txt";
    argv[2] = "output.txt";

    //check arguments
    if (argc == 3) {
        inputFile.open(argv[1]);
        if(!inputFile){
            cerr << "No input file found" << endl;
            return 1;
        }
        outputFile.open(argv[2]);
        if(!outputFile){
            cerr << "No output file found" << endl;
            return 1;
        }
    }
    else {
        cerr << "Invalid Arguments" << endl;
        return 1;
    }

    if(argv[0] == "-huff"){
        //get file as string
        char c;
        while(inputFile.get(c)){
            text += c;
        }

        //write magic number to file (125)
        outputFile << (char)125;

        char eof = '~';
        text += eof;
        //get frequencies of each
        map<char, int> frequencies = getFrequencies(text);

        //write number of pairs
        outputFile << frequencies.size();
        //write frequencies to file
        for(pair<char, int> p : frequencies){
            outputFile << p.first << " " << p.second;
        }

        //create huffman tree
        priority_queue<Node*, vector<Node*>, compareNodes> pq =
            buildHuffmanTree(frequencies);

        //read each character
        outputFile << getEncode(pq, text);
    }
    else if(argv[0] == "-unhuff"){
        char c;
        int size, freq, front, div;
        
        //check magic number
        if(inputFile.get() != (char)125){
            cerr << "File has not been compressed" << endl;
            return 1;
        }
        //get number of characters
        inputFile >> size;
        //get frequencies
        map<char, int> frequencies;
        for(int i = 0; i < size; i++){
            inputFile.get(c);
            inputFile >> freq;
            frequencies[c] = freq;
        }
        //rebuild huffman tree
        priority_queue<Node*, vector<Node*>, compareNodes> pq =
            buildHuffmanTree(frequencies);

        //get file as string
        while(inputFile.get(c)){
            for(int i = 6; i >= 0; i--){
                //get bits from character
                div = pow(2, i);
                front = c / div;
                if(front == 0){
                    text += '0';
                }
                else {
                    text += '1';
                    c -= div;
                }
            }
        }
        outputFile << getDecode(pq, text);
    }
    else {
        cerr << "Invalid Command" << endl;
    }
    
    return 0;
}
