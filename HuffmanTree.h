/*
Author: Michael Mathews
Assignment Title: Assignment 12.1
Assignment Description: Use Huffman coding to "huff" and "unhuff" a file
Due Date: 11/28/22
Date Created: 11/21/22
Date Last Modified: 11/28/22
*/

#ifndef HuffmanTree_h
#define HuffmanTree_h

#include <iostream>
#include <string>
#include <queue>
#include <map>


using namespace std;

//A Tree node
struct Node{
    char c;
    int freq;
    Node *left, *right;
};

//Comparison object to be used to order the heap
struct compareNodes {
    bool operator()(Node* left, Node* right){
        return left->freq > right->freq;
    }
};

/*
precondition: no node created
postcondition: node created
return type: Node* that holds the address of the new node created
description: used to allocate a new node for the tree
*/
Node* newNode(char c, int freq, Node* left, Node* right){
    Node* node = new Node();
    node->c = c;
    node->freq = freq;
    node->left = left;
    node->right = right;
    return node;
}

/*
precondition: no character encoded
postcondition: every character encoded
return type: none
description: traverse the Huffman Tree and encode each character in map
*/
void encode(Node* root, string str, map<char, string> &huffmanCode){
    if (root != nullptr){
        //if leaf node, store code
        if (root->left == nullptr && root->right == nullptr) {
            huffmanCode[root->c] = str;
        }
        //add to code and continue down tree
        encode(root->left, str + "0", huffmanCode);
        encode(root->right, str + "1", huffmanCode);
    }
}

/*
precondition: no character decoded
postcondition: every character decoded
return type: none
description: traverse the Huffman Tree and decode the encoded string
*/
void decode(Node* root, int &index, string str, string &result){
    if (root != nullptr) {
        //if leaf node, get character
        if (root->left == nullptr && root->right == nullptr){
            result += root->c;
        }
        else {
            index++;
            if (str[index] == '0'){
                decode(root->left, index, str, result);
            }
            else {
                decode(root->right, index, str, result);
            }
        }
    }
}

/*
precondition: no frequences found for characters
postcondition: every character's frequency is found
return type: map<char, int> a map of each character to its frequency
description: finds the frequency that each character appears in a string
*/
map<char, int> getFrequencies(string input){
    //make map of characters to their frequencies in a text
    map<char, int> frequencies;
    for (char c : input) {
        frequencies[c]++;
    }
    return frequencies;
}

/*
precondition: no tree created
postcondition: prioity queue huffman tree created
return type: priority_queue holds the huffman tree used for encoding/decoding
description: creates a huffman tree for use with encoding and decoding
*/
// Builds Huffman Tree and decode given input text
priority_queue<Node*, vector<Node*>, compareNodes> buildHuffmanTree(
                                        map<char, int> frequencies){

    // Add characters as leaf nodes to Huffman priority queue
    priority_queue<Node*, vector<Node*>, compareNodes> pq;
    for (pair<char, int> p : frequencies) {
        pq.push(newNode(p.first, p.second, nullptr, nullptr));
    }

    //create internal nodes with no character
    while (pq.size() != 1){
        //get two nodes of highest priority and store in left/right
        Node *left = pq.top(); pq.pop();
        Node *right = pq.top(); pq.pop();

        //create new node with freq equal to sum of freq of nodes above
        int sum = left->freq + right->freq;
        pq.push(newNode('\0', sum, left, right));
    }
    return pq;
}

/*
precondition: no encoded string
postcondition: string is completely encoded
return type: string - encoded version of the string
description: uses encoded version of characters and places them as bits into
 a final encoded string in order
*/
string getEncode(priority_queue<Node*, vector<Node*>, compareNodes> pq,
                                                                string input){
    //change character to huffman ones and zeros
    map<char, string> huffmanCode;
    //read reach one and zero and push into character if possible
    //add character to string
    //get all encodings
    encode(pq.top(), "", huffmanCode);
    // print encoded string
    string str = "";
    string code;
    char character = '\0';
    int counter = 0;
    for (char c: input) {
        code = huffmanCode[c];
        //change code to bits
        for(int i = 0; i < code.length(); i++){
            //read 1 or 0
            if(code[i] == '0'){
                if(counter == 7){
                    //add 8 bits
                    str += character;
                    character = '\0';
                    counter = 0;
                }
                //add bit
                character *= 2;
                //update current bit
                counter++;
            }
            else if (code[i] == '1'){
                if(counter == 7){
                    //add 8 bits
                    str += character;
                    character = '\0';
                    counter = 0;
                }
                //add bit
                character *= 2;
                character += 1;
                //update current bit
                counter++;
            }
        }
    }
    //add trailing 0's
    //make sure not creating a new character
    counter = 7 - counter;
    if(counter != 7){
        for(int i = 0; i < counter; i++){
            character *= 2;
        }
        str += character;
    }
    return str;
}

/*
precondition: no decoded string
postcondition: string is completely decoded
return type: string - decoded version of the string
description: uses encoded version of characters decodes them in order until
 a certain character is hit as the 'eof' character
*/
string getDecode(priority_queue<Node*, vector<Node*>, compareNodes> pq,
                                                                string input){
    // traverse the Huffman Tree again and this time
    // decode the encoded string
    string result = "";
    int index = -1;
    bool done = false;
    //if file has been compressed, decode
    while (!done) {
        decode(pq.top(), index, input, result);
        if(result[result.size()-1] == '~'){
            result.pop_back();
            done = true;
        }
    }
    return result;
}

#endif /* HuffmanTree_h */
