/*
 * Name: Dalton Neely
 * Class: CSCI 480 Theory of Algorithms
 * Professor: Gary Rader
 * Due: First Week of October
 * Description: Implement your own text file compression/decompression
 * programs. The compression program should use a Huffman Encoding and
 * take a single parameter which is the pathname for the file of text to
 * be compressed
 * Program Name: MCP
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>
#include <cmath>
#include <iomanip>
#include <cstdio>
#include "nodeClass.hpp"
#include "heapClass.hpp"
#include "huffmanClass.hpp"

using namespace std;

//protos
double compareFiles(string, string);

/*
 * swap()
 * Usage: swap(&node1, &node2)
 * Testing: works 10/2/17
 * This function swaps to nodes of nodeClass type
 * DN
 */

void swap(nodeClass * A, nodeClass * B){
	nodeClass * temp = new nodeClass;
	
	temp->set(A->freq, A->ascii, A->L, A->R);
	A->set(B->freq, B->ascii, B->L, B->R);
	B->set(temp->freq, temp->ascii, temp->L, temp->R);
}

/*
 * read()
 * Usage: read(&heapObject, fileName)
 * Testing: works 10/2/17
 * This function reads the character in the given file and adds nodes
 * to the min heap passed to the function
 * DN
 */

void read(heapClass * _heap, string _file){
	ifstream in(_file);
	char c;
	int frequency[128];
	
	//initialize frequency array
	for(int i = 0; i < 128; i++)
		frequency[i] = 0;
	
	//iterate through all the characters in the file
	while(in.get(c))
		frequency[(int) c]++;
	
	//iterate through all ascii values 
	for(int i = 0; i < 128; i++){
		//check if ascii even showed up in the file
		if(frequency != 0){
			//add a node to the min heap
			nodeClass * node = new nodeClass;
			node->set(frequency[i], i, nullptr, nullptr);
			_heap->insert(node);
		}
	}
	
	in.close();
}

/*
 * intToChar()
 * Usage: intToChar(intToConvert)
 * Testing: works 10/1/17
 * This function converts a integer to a character 
 * DN
 */

char intToChar(int _input){
	if(_input == 1)
		return '1';
	if(_input == 0);
		return '0';
}

/*
 * encodeText()
 * Usage: encodeText(fileName, &huffmanTree)
 * Testing: works 9/30/17
 * This funciton encodes the text stored in a file with the huffman
 * codes passed by the huffman object
 * DN
 */

string encodeText(string _file, huffmanClass * _tree){
	char c;
	string strCodes[128];
	string eText = "";
	ifstream in(_file);
	
	//initialize
	for(int i = 0; i < 128; i++)
		strCodes[i] = "";
	
	//iterate through each ascii value
	for(int i = 0; i < 128; i++)
		//iterate through the max length of a code
		for(int j = 0; j < 50; j++)
			//only store the codes that are valid
			if(_tree->codes[i][j] != 2)
				strCodes[i] += intToChar(_tree->codes[i][j]);
				
	//encode each character with its code
	while(in.get(c)){
		eText += strCodes[(int) c];
	}
	
	return eText;
}

/*
 * writeBinary()
 * Usage: writeBinary(binaryString, bitCount, ofsteam)
 * Testing: works 9/30/17
 * This function takes in a string of bits anlong with the number of bits
 * and writes this string to the output file
 * DN
 */

void writeBinary(string _binary, int _count, ofstream & _out){
	int pos = 0;
	int len = 8;
	char c[1];
	bitset<8> eof(26);
	
	//if bits are not a multiple of 8 then make them by padding with zeros
	if(_count % 8 != 0){
		for(int i = 0; i < _count % 8; i++){
			bitset<1> temp;
			temp.reset();
			_binary += temp.to_string();
			_count++;
		}
	}
	
	//iterate and write by the byte length
	for(int i = 0; i < _count / 8; i++){
		bitset<8> byte(_binary.substr(pos, len));
		c[0] = byte.to_ulong();
		_out.write(c, 1);
		pos += 8;
	}
	
	//Write the eof character
	_count +=8;
	c[0] = eof.to_ulong();
	_binary += eof.to_string();
	_out.write(c, 1);
}

/*
 * compressFile();
 * Usage: compressFile(fileName, numOfCharacters, &huffmanTree)
 * Testing: works 9/30/17
 * This funciton takes an input file along with the huffman tree
 * already created to compress the contents and write it to a new file
 * DN
 */

void compressFile(string _file, int _entries, huffmanClass * _tree){
	string bitStream = "", newFile = _file + ".mcp";
	//encode the files contents and store it in the eText string
	string eText = encodeText(_file, _tree);
	int bitCount = 0, nameSize = _file.length(), modSize = nameSize;
	int diff, encodeLength = 0, bitBuffer[8], bitBufferCt = 0;
	bitset<32> magicNum(0x1AF69384);
	bitset<8> buffer;
	bitset<1> oneBit;
	
	ofstream out;
	out.open(newFile, ofstream::out | ofstream::binary);
	
	//Write Magic Number of 0x1AF69384 (452367236)
	bitStream += magicNum.to_string();
	bitCount += bitStream.size();

	//if the name size is not a multiple of 4 make it a multiple of 4
	if(nameSize % 4 != 0){
		diff = 4 - (nameSize % 4);
		modSize += diff;
	}
	
	//add the actual nameSize to the bit stream
	bitset<8> nameSizeByte(nameSize);
	bitStream += nameSizeByte.to_string();
	bitCount += nameSizeByte.size();
	
	//Write the name of the file in ascii
	for(int i = 0; i < modSize; i++){
		//Check if nameSize is less than modSize otherwize pad with zeros
		if(i < nameSize){
			bitset<8> current((int) _file[i]);
			bitStream += current.to_string();
			bitCount += current.size();
		}else{
			bitset<8> current(0x00);
			bitStream += current.to_string();
			bitCount += current.size();
		}
	}
	
	//Write the Number of Entries
	bitset<8> bit8(_entries);
	bitStream += bit8.to_string();
	bitCount += bit8.size();

	
	//Write the codes table
	//iterate through the whole standard ascii
	for(int i = 0; i < 128; i++){
		//Check to see if the ascii value even has a code to write
		if(_tree->codes[i][0] != 2){
			
			//Write the ascii value as a byte
			bitset<8> ascii(i);
			bitStream += ascii.to_string();
			bitCount += ascii.size();
			
			//find the code length of the current ascii
			for(int j = 0; j < 50; j++)
				if(_tree->codes[i][j] != 2)
					encodeLength++;
			
			//Write the length of the code as a byte
			bitset<8> eLength(encodeLength);
			bitStream += eLength.to_string();
			bitCount += eLength.size();
			
			//iterate the length of the code for current ascii
			for(int q = 0; q < encodeLength; q++){
				//set the bit
				oneBit.set(0, _tree->codes[i][q]);
				//Write the bit to the stream
				bitStream += oneBit.to_string();
				bitCount += oneBit.size();
			}
			//reset the code length value
			encodeLength = 0;
		}
	}
	
	//Write the length in bits of the encoded text using a 4 byte number
	bitset<32> length(eText.length());
	bitStream += length.to_string();
	bitCount += 32;
	
	//Write the encode text to the bitStream
	bitStream += eText;
	bitCount += eText.length();
	
	//Lastly write the binary kept in the bitStream to the output file
	writeBinary(bitStream, bitCount, out);
	
	cout.setf(ios::fixed);
	cout.precision(2);
	
	cout << "Compression saved " << compareFiles(_file, _file+".mcp") << "% storage space!" << endl;
	
	remove(_file.c_str());
}

/*
 * binaryToInt()
 * Usage: binaryToInt(binaryString, sizeOfString)
 * Testing: works 9/30/17
 * This function takes in a string version of binary bits with the length
 * of bits and converts that into an int
 */

int binaryToInt(string _binary, int _size){
	int returnVal = 0, power = 0;
	
	//iterate backwards because binary is right to left
	for(int i = _size - 1; i >= 0; i--){
		//only increase the return value if the bit is turned on
		if(_binary[i] == '1')
			returnVal += (int)pow(2, power);
		power++;
	}
	
	return returnVal;
}

/*
 * writeToFile()
 * Usage: writeToFile(fileName, textToWrite)
 * Testing: works 9/30/17
 * This function writes a string to the given file
 * DN
 */

void writeToFile(string _fileName, string _text){
	
	ofstream out;
	out.open(_fileName, ofstream::out);
	
	char c[1];
	
	//iterate the length of the readable string, and write one character
	//at a time.
	for(int i = 0; i < _text.length(); i++){
		c[0] = _text[i];
		out.write(c, 1);
	}
	
	out.close();
}

/*
 * readCompressed()
 * Usage: readCompressed(fileName)
 * Testing: works 9/30/17
 * This funciton reads a compressed file and combines the bits gathered
 * from the file and converts them to a string of bits. Then, the funciton
 * deconstructs the bit string and converts it into a readable string
 * using a predefined protocol, and sends that data to the writeToFile
 * funciton.
 * DN
 */

void readCompressed(string _file){
	ifstream in;
	in.open(_file, ifstream::in | ifstream::binary);
	
	string binary = "", fileName = "", text = "", codes[128];;
	char c, pattern[50];
	int count = 0, index = 0, size = 0, magicNumber, fileNameLength;
	int modSize, entriesLength, codeLength = 0, ascii;
	int textEncodedLength = 0, patCount = 0, numOfMatch = 0;
	bool mismatch = true, wrong = false; 
	
	for(int i = 0; i < 50; i++)
		pattern[i] = '3';

	//Read all the bits from the file and put them in a 
	//string representation
	while(in.get(c)){
			bitset<8> current((int) c);
			binary += current.to_string();
			count += 8;
	}
	//Store number of bits in size
	size = binary.length();
	
	//Read Magic Number increment index 4 bytes
	magicNumber = binaryToInt(binary.substr(0, 32), 32);
	index += 32;
	
	//check if file can be uncompressed
	if(magicNumber == 452367236){
		//Read file name length, if it is not a multiple of 4, then increase
		//the file name length to make it a multiple of 4, then increase by
		//1 byte
		fileNameLength = binaryToInt(binary.substr(index, 8), 8);
		modSize = fileNameLength;
		if(fileNameLength % 4 != 0){
			int diff = 4 - (fileNameLength % 4);
			modSize += diff;
		}
		fileNameLength = modSize;
		index += 8;
		
		//Read file name by reading each ascii value byte
		for(int i = 0; i < fileNameLength; i++){
			fileName += (char) binaryToInt(binary.substr(index, 8), 8);
			index += 8;
		}
		
		
		//Read Entries Length (amount of leaves/character)
		entriesLength = binaryToInt(binary.substr(index, 8), 8);
		index += 8;
		
		
		//Read Codes, iterate through all entries and within each entry
		//iterate through each bit to store in the codes array
		for(int i = 0; i < entriesLength; i++){
			ascii = binaryToInt(binary.substr(index, 8), 8);
			index += 8;
			codeLength = binaryToInt(binary.substr(index, 8), 8);
			index += 8;
			for(int j = 0; j < codeLength; j++){
				codes[ascii] += binary[index];
				index++;
			}
		}
		
		//Read the text encoded length
		textEncodedLength = binaryToInt(binary.substr(index, 32), 32);
		index += 32;
						
		//iterate through the bits in the stirng starting with index and
		//ending with the index plus the length of the encoded texts (bits)
		for(int i = index; i < index + textEncodedLength; i++){
			//Store the current character of the binary string in a pattern
			//array
			pattern[patCount] = binary[i];
			patCount++;
			//Iterate through all 128 standard ascii characters
			for(int j = 0; j < 128; j++){
				//Checks if the ascii character has a code, if the first int
				//int the codes array is initalized to 2, then no code exists
				//this improves speed so we don't look up characters that
				//don't have a code
				if(codes[j][0] != 2){
					//Start wrong set to false because we don't know yet
					wrong = false;
					//iterate through the length of the current pattern length
					//as long as the the pattern is not wrong (!wrong)
					for(int q = 0; q < patCount && !wrong; q++){
						if(pattern[q] == codes[j][q]){
							mismatch = false;
							numOfMatch++;
						}else{
							//set wrong to true to break the loop
							mismatch = true;
							wrong = true;
						}
					}
					//if the number of matching pattern characters equals
					//the length of characters in the code AND there was no
					//mismatch of characters AND the length of the code
					//stored in the codes array is not zero, then add that
					//ascii value (j) to the text string
					if(numOfMatch == codes[j].length() &&
					!mismatch &&
					codes[j].length() != 0){
						text += (char)j;
						//reset the pattern count becaue we have a match
						patCount = 0;
						//set j to 128 to break the loop
						j = 128;
						//reset mismatch to its default
						mismatch = false;
					}
					//Reset the number of matches for this ascii value search
					numOfMatch = 0;
				}
			}
		}
		//All the character should now be written to the text string
		//So write the string with the fileName captured in the read operation
		writeToFile(fileName, text);
		
		in.close();
		
		remove(_file.c_str());
	}else{
		//output error message to user
		cout << "Not a valid compressed file. Exiting..." << endl;
	}				
}


/*
 * compareFiles()
 * Usage: compareFiles(originalFileName, compressedFileName);
 * This funciton compares the original file with the compressed file
 * for size difference
 * Return Value: a double containing the compression percentage
 * Testing: Works 9/30/17
 * DN
 */
double compareFiles(string _original, string _compressed){
	int originalSize;
	int compressedSize;
	
	//Get the size of the original file
	ifstream in(_original, ifstream::ate | ifstream::binary);
	originalSize =  in.tellg();
	in.close();
	
	//Get the size of the compressed file
	in.open(_compressed, ifstream::ate | ifstream::binary);
	compressedSize = in.tellg();
	in.close();
	
	//return the ratio times 100 to convert it to a percent
	return 100.00 * ((double) compressedSize / (double) originalSize);
}


/*
 * diff()
 * This function finds the difference between the original file and its
 * compressed version.
 * Usage: diff(nameOfFile)
 * Return Value: returns true if files differ, else it returns false
 * Testing: Works 9/30/17
 * DN 
 */
bool diff(string _file){
	bool difference = false;
	char c;
	string startStr = "";
	string endStr = "";
	
	//Get contents of original file
	ifstream in(_file);
	while(in.get(c))
		startStr += c;
	in.close();
	
	//Get contents of uncompressed file
	in.open("uzip."+_file);
	while(in.get(c))
		endStr += c;
	
	//Compare its contents
	if(startStr != endStr)
		difference = true;
		
	return difference;
}
	
int main(int argc, char *argv[]){
	string arg2(argv[2]);
	string filename(argv[1]);
	
	if(arg2 == "-compress"){
		//Array to store intermidate code values in recursive function call
		//createCodes()
		int array[100];
		
		//Creating the min heap (priority queue) and the huffman tree
		heapClass * heap = new heapClass;
		huffmanClass * tree = new huffmanClass;
		
		//Creates the initial state of the min heap by reading the input file
		read(heap, filename);
		//creates the huffman tree from the min heap
		tree->create(heap);
		//traverses the huffman tree at root (0) and creates the encoding scheme
		tree->createCodes(tree->root, array, 0);
		
		//Compress file by passing the filename, number of leaves,
		//and the tree itself
		compressFile(filename, tree->leaves, tree);
	}else if(arg2 == "-decompress"){
		//Read the compressed file, then write the new file
		readCompressed(filename);
	}else{
		cout << "Usage: MPC.exe <fileName> (-compress | -decompress)" << endl; 
	}

	//operating system return value
	return 0;
}
