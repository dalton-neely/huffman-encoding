#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>
#include <cmath>

using namespace std;

class nodeClass{
	public:
		int freq, ascii;
		nodeClass *L, *R;
		nodeClass();
		void set(int, int, nodeClass*, nodeClass*);
		void print();
};

nodeClass::nodeClass(){
	freq = 0;
	ascii = 0;
	L = nullptr;
	R = nullptr;
}

void nodeClass::set(int _F, int _A, nodeClass *_L, nodeClass *_R){
	if(_F < 0){
		cout << "Node Frequency Less Than 0" << endl;
	}else if(_A < 0 || _A > 128){
		cout << "Node is not vaild ascii" << endl;
	}else{
		freq = _F;
		ascii = _A;
		L = _L;
		R = _R;
	}
}

void nodeClass::print(){
	cout << "####" << endl;
	cout << "Location: " << this << endl;
	cout << "Frequency: " << freq << endl;
	cout << "Ascii: " << ascii << endl;
	cout << "Left ptr: " << L << endl;
	cout << "Right ptr: " << R << endl;
	cout << "####" << endl;
}

void swap(nodeClass * A, nodeClass * B){
	nodeClass * temp;
	temp = new nodeClass;
	temp->set(A->freq, A->ascii, A->L, A->R);
	A->set(B->freq, B->ascii, B->L, B->R);
	B->set(temp->freq, temp->ascii, temp->L, temp->R);
}

class heapClass{
	public:
		nodeClass * nodes[500];
		int size;
		heapClass();
		void insert(nodeClass *);
		void sort(int);
		void print();
		nodeClass * extract();
};

heapClass::heapClass(){
	for(int i = 0; i < 500; i++)
		nodes[i] = nullptr;
	size = 0;
}

void heapClass::insert(nodeClass * _node){
	nodes[size] = _node;
	size++;
	for(int i = (size - 2) / 2; i >= 0; i--)
		sort(i);
}

void heapClass::sort(int _index){
	if(_index >= 0){
		int small = _index;
		int left = (2 * _index) + 1;
		int right = (2 * _index) + 2;
		if(left < size && nodes[left]->freq < nodes[small]->freq)
			small = left;
		if(right < size && nodes[right]->freq < nodes[small]->freq)
			small = right;
		if(small != _index){
			swap(nodes[small], nodes[_index]);
			sort(small);
		}
	}
}

void heapClass::print(){
	for(int i = 0; i < size; i++)
		cout << "index: " << i << " [" << nodes[i]->freq << "," << nodes[i]->ascii << "]" << endl;
}

nodeClass * heapClass::extract(){
	nodeClass * ptr;
	ptr = new nodeClass;
	ptr->set(nodes[0]->freq, nodes[0]->ascii, nodes[0]->L, nodes[0]->R);
	swap(nodes[0], nodes[--size]);
	for(int i = (size - 2) / 2; i >= 0; i--)
		sort(i);
	return ptr;
}

class huffmanClass{
	public:
		nodeClass * root;
		int codes[128][50];
		int leaves;
		huffmanClass();
		void create(heapClass *);
		void printLeaves(nodeClass *);
		void createCodes(nodeClass *, int *, int);
		void printCodes();
};

huffmanClass::huffmanClass(){
	root = nullptr;
	leaves = 0;
	for(int j = 0; j < 128; j++)
		for(int i = 0; i < 50; i++)
			codes[j][i] = 2;
}

void huffmanClass::create(heapClass * _heap){
	if(_heap->size != 1){
		nodeClass * a = nullptr, * b = nullptr;
		a = _heap->extract();
		b = _heap->extract();
		nodeClass * node;
		node = new nodeClass;
		node->set(a->freq + b->freq, 128, a, b);
		if(a->ascii != 128){
			a->L = nullptr;
			a->R = nullptr;
			leaves++;
		}
		if(b->ascii != 128){
			b->L = nullptr;
			b->R = nullptr;
			leaves++;
		}
		root = node;
		_heap->insert(node);
		create(_heap);
	}
}

void huffmanClass::printLeaves(nodeClass * _ptr){
	if(_ptr->L != nullptr && _ptr->R != nullptr){
		if(_ptr->L != nullptr)
			printLeaves(_ptr->L);
		if(_ptr->R != nullptr)
			printLeaves(_ptr->R);
	}else{
		cout << _ptr->ascii << endl;
	}	
}

void huffmanClass::createCodes(nodeClass * _root, int * _code, int _index){
	if(_root->L != nullptr){
		_code[_index] = 0;
		createCodes(_root->L, _code, _index + 1);
	}
	if(_root->R != nullptr){
		_code[_index] = 1;
		createCodes(_root->R, _code, _index + 1);
	}
	if(_root->L == nullptr && _root->R == nullptr){
		for(int i = 0; i < _index; i++){
			codes[_root->ascii][i] = _code[i];
		}
	}
}

void huffmanClass::printCodes(){
	for(int j = 0; j < 128; j++){
		if(codes[j][0] != 2){
			cout << "Ascii: " << j << " codes: ";
			for(int i = 0; i < 50; i++){
				if(codes[j][i] != 2)
					cout << codes[j][i];
			}
			cout << endl;
		}
	}
}

void read(heapClass * _heap, string _file){
	ifstream in;
	char c;
	int f = 0;
	in.open(_file);
	for(int i = 0; i < 128; i++){
		in.open(_file);
		while(in.get(c)){
			if(c == (char) i)
				f++;
		}
		if(f != 0){
			nodeClass * node;
			node = new nodeClass;
			node->set(f, i, nullptr, nullptr);
			_heap->insert(node);
			f = 0;
		}
		in.close();
	}
}

char intToChar(int _input){
	if(_input == 1)
		return '1';
	if(_input == 0);
		return '0';
}

int charToInt(char _input){
	if(_input == '1')
		return 1;
	if(_input == '0')
		return 0;
}

string encodeText(string _file, huffmanClass * _tree){
	char c;
	string strCodes[128];
	string eText = "";
	for(int i = 0; i < 128; i++)
		strCodes[i] = "";
	for(int i = 0; i < 128; i++){
		for(int j = 0; j < 50; j++){
			if(_tree->codes[i][j] != 2)
				strCodes[i] += intToChar(_tree->codes[i][j]);
		}
	}
	ifstream in;
	in.open(_file);
	while(in.get(c)){
		eText += strCodes[(int) c];
	}
	return eText;
}

void writeBit(ofstream & _out, int * _bits, int & _ct, int _value){
	char c[1];
	bitset<8> bits;
	if(_ct == 8){
		for(int i = 0; i < 8; i++){
			if(_bits[i] == 1){
				bits.set(i);
			}else{
				bits.reset(i);
			}
		}
		c[0] = (char) bits.to_ulong();
		_out.write(c, 1);
		_ct = 0;
	}else{
		if(_value == 1){
			_bits[_ct] = 1;
		}else if(_value == 0){
			_bits[_ct] = 0;
		}else{
			cout << "Error: Something other than a 1 or a 0 passed to be written" << endl;
		}
		_ct++;
	}
}

void writeBinary(string _binary, int _count, ofstream & _out){
	//cout << "coutFirst: " << _count << endl;
	if(_count % 8 != 0){
		for(int i = 0; i < _count % 8; i++){
			bitset<1> temp;
			temp.reset();
			_binary += temp.to_string();
			_count++;
		}
		//cout << "(" << _count << ")bitStream: " << _binary << endl;
	}
	//cout << "count: " << _count << endl;
	int pos = 0;
	int len = 8;
	char c[1];
	for(int i = 0; i < _count / 8; i++){
		bitset<8> byte(_binary.substr(pos, len));
		c[0] = byte.to_ulong();
		_out.write(c, 1);
		pos += 8;
	}
	bitset<8> em(25);
	cout << "em : " << em.to_string() << endl;
	//bitset<64> eof(-1);
	//cout << "eof: " << eof.to_string() << endl;
	_count +=8;
	//_count += 64;
	//char b[8];
	//bitset<8> ones;
	//ones.set();
	//for(int i = 0; i < 8; i++){
		//b[i] = (char) ones.to_ulong();
		//_binary += ones.to_string();
	//}
	c[0] = em.to_ulong();
	_binary += em.to_string();
	_out.write(c, 1);
	//_out.write(b, 8);
	//cout << "(" << _count << ")bitStream: " << _binary << endl;
}

void compressFile(string _file, int _entries, huffmanClass * _tree){
	
	string bitStream = "";
	int bitCount = 0;
	
	//Write Magic Number of 0x1AF69384
	//char bit32[4];
	string newFile = _file + ".mcp";
	ofstream out;
	out.open(newFile, ofstream::out | ofstream::binary);
	
	
	bitset<32> magicNum(0x1AF69384);
	bitStream += magicNum.to_string();
	bitCount += bitStream.size();
	//cout << "(" << bitCount << ")bitStream: " << bitStream << endl;
	
	/*
	bitset<8> mn1(0x1A);
	bitset<8> mn2(0xF6);
	bitset<8> mn3(0x93);
	bitset<8> mn4(0x84);
	bit32[0] = (char) mn1.to_ulong();
	bit32[1] = (char) mn2.to_ulong();
	bit32[2] = (char) mn3.to_ulong();
	bit32[3] = (char) mn4.to_ulong();
	//int test = 452367236;
	//bitset<32> silence(0x00000000);
	out.write(bit32, 4);
	*/
	
	//Write the length of the file name in 8 bits to proceeds
	//out.write((char*)&test, sizeof(test));
	//out.write(silence.to_string().c_str(), 32);
	int nameSize = _file.length();
	int modSize = nameSize;
	if(nameSize % 4 != 0){
		int diff = 4 - (nameSize % 4);
		modSize += diff;
	}
	bitset<8> nameSizeByte(nameSize);
	bitStream += nameSizeByte.to_string();
	bitCount += nameSizeByte.size();
	//cout << "(" << bitCount << ")bitStream: " << bitStream << endl;
	/*char nsb[1];
	nsb[0] = (char) nameSizeByte.to_ulong();
	out.write(nsb, 1);
	
	//Write the file name
	char c[1];
	*/
	for(int i = 0; i < modSize; i++){
		if(i < nameSize){
			bitset<8> current((int) _file[i]);
			bitStream += current.to_string();
			bitCount += current.size();
			//c[0] = (char) current.to_ulong();
			//out.write(c, 1);
		}else{
			bitset<8> current(0x00);
			bitStream += current.to_string();
			bitCount += current.size();
			//c[0] = current.to_ulong();
			//out.write(c, 1);
		}
		
		//cout << "(" << bitCount << ")bitStream: " << bitStream << endl;
	}
	//out.write(silence.to_string().c_str(), 32);
	
	//Write the Number of Entries
	bitset<8> bit8(_entries);
	bitStream += bit8.to_string();
	bitCount += bit8.size();
	//cout << "(" << bitCount << ")bitStream: " << bitStream << endl;
	//c[0] = (char) bit8.to_ulong();
	//out.write(c, 1);
	
	//Write the codes table
	int encodeLength = 0;
	int bitBuffer[8];
	int bitBufferCt = 0;
	bitset<8> buffer;
	for(int i = 0; i < 128; i++){
		if(_tree->codes[i][0] != 2){
			bitset<8> ascii(i);
			bitStream += ascii.to_string();
			bitCount += ascii.size();
			//cout << "For " << i << " writing: ";
			//for(int t = 0; t < 8; t++){
				//writeBit(out, bitBuffer, bitBufferCt, ascii[t]);
				//cout << ascii[t];
			//}
			//cout << endl;
			//bitset<8> ascii(i);
			//c[0] = (char) ascii.to_ulong();
			//out.write(c, 1);
			for(int j = 0; j < 50; j++){
				if(_tree->codes[i][j] != 2){
					encodeLength++;
				}
			}
			bitset<8> eLength(encodeLength);
			bitStream += eLength.to_string();
			bitCount += eLength.size();
			//for(int z = 0; z < 8; z++){
				//writeBit(out, bitBuffer, bitBufferCt, eLength[z]);
			//}
			//c[0] = eLength.to_ulong();
			//out.write(c, 1);
			//char array[1];
			
			bitset<1> oneBit;
			for(int q = 0; q < encodeLength; q++){
				oneBit.set(0, _tree->codes[i][q]);
				bitStream += oneBit.to_string();
				bitCount += oneBit.size();
					//writeBit(out, bitBuffer, bitBufferCt, _tree->codes[i][q]);
			}
			//cout << "(" << bitCount << ")bitStream: " << bitStream << endl;
				//array[0] =  intToChar(_tree->codes[i][q]);
				//out.write(array, 1);
			encodeLength = 0;
		}
	}
	//writeBinary(bitStream, bitCount, out);
	string eText = encodeText(_file, _tree);
	//bitset<32> eLength(eText.length());
	
	bitset<32> length(eText.length());
	bitStream += length.to_string();
	bitCount += 32;
	bitStream += eText;
	bitCount += eText.length();
	//cout << "bitCount passed: " << bitCount << endl;
	//cout << "eText: " << eText << endl;
	//cout << "(" << bitStream.length() << ")bitStream: " << bitStream << endl;
	writeBinary(bitStream, bitCount, out);
	//for(int r = 0; r < 32; r++){
		//if(eLength[r] == 1){
			//bitset<1> one;
			//one.set();
			//if(bitBufferCt == 8){
				//for(int r = 0; r < 8; r++){
					//buffer[r] = bitBuffer[r];
				//}
				//c[0] = (char) buffer.to_ulong();
				//out.write(c, 1);
				//bitBufferCt = 0;
			//}
			//bitBuffer[bitBufferCt] = one[0];
			//bitBufferCt++;
		//}else if(eLength[r] == 0){
			//bitset<1> zero;
			//zero.reset();
			//if(bitBufferCt == 8){
			//	for(int r = 0; r < 8; r++){
				//	buffer[r] = bitBuffer[r];
			//	}
			//	c[0] = (char) buffer.to_ulong();
			//	out.write(c, 1);
			//	bitBufferCt = 0;
			//}
			//bitBuffer[bitBufferCt] = zero[0];
			//bitBufferCt++;
		//}
	//}
	/*for(int r = 0; r < eText.length(); r++){
		if(eText[r] == '1'){
			bitset<1> one;
			one.set();
			if(bitBufferCt == 8){
				for(int r = 0; r < 8; r++){
					buffer[r] = bitBuffer[r];
				}
				c[0] = (char) buffer.to_ulong();
				out.write(c, 1);
				bitBufferCt = 0;
			}
			bitBuffer[bitBufferCt] = one[0];
			bitBufferCt++;
		}else if(eText[r] == '0'){
			bitset<1> zero;
			zero.reset();
			if(bitBufferCt == 8){
				for(int r = 0; r < 8; r++){
					buffer[r] = bitBuffer[r];
				}
				c[0] = (char) buffer.to_ulong();
				out.write(c, 1);
				bitBufferCt = 0;
			}
			bitBuffer[bitBufferCt] = zero[0];
			bitBufferCt++;
		}
	}
	//bitset<32> eTextLen(eText.length());
	//out.write(eTextLen.to_string().c_str(), 32);
	//out.write(eText.c_str(), eText.length());*/
}

int binaryToInt(string _binary, int _size){
	int rVal = 0;
	int c = 0;
	for(int i = _size - 1; i >= 0; i--){
		if(_binary[i] == '1'){
			rVal += (int)pow(2, c);
		}
		c++;
	}
	return rVal;
}

void readCompressed(string _file){
	ifstream in;
	in.open(_file, ifstream::in | ifstream::binary);
	//char magicNumber[32];
	
	char ch;
	//bitset<8> current;
	string binary = "";
	int em = 25;
	int count = 0;
	int index = 0;
	int size = 0;
	while(in.get(ch)){
		if(ch != (char)em){
			bitset<8> current((int) ch);
			binary += current.to_string();
			count += 8;
		}
	}
	size = binary.length();
	
	cout << endl << endl << "(" << count << ")Read String: " << binary << endl << endl;
	
	//Read Magic Number 
			//~ char mn[4];
			//~ in.read(mn, 4);
			//~ string mnS(mn);
			//~ cout << "Magic Number: ";
			//~ string bStr = "";
			//~ for(int i = 0; i < 4; i++){
				//~ bitset<8> bits((int) mn[i]);
				//~ bStr += bits.to_string();
			//~ }
			//~ cout << binaryToInt(bStr, 32) << endl;
			
	cout << "Read Magic Number: " << binaryToInt(binary.substr(0, 32), 32) << endl;
	index += 32;
	
	
	//Read file name length
	//char silence[32];
	//in.read(silence, 32);
				//~ char fns[1];
				//~ in.read(fns, 1);
				//~ bitset<8> fnsB((int) fns[0]);
				//~ //string fileNameSizeStr(fileNameSize);
				//~ int nameSize = binaryToInt(fnsB.to_string(), 8);
				//~ int modSize = nameSize;
				//~ cout << "File Name Length: " << nameSize << endl;
				//~ if(nameSize % 4 != 0){
					//~ int diff = 4 - (nameSize % 4);
					//~ modSize += diff;
				//~ }
				//~ cout << "File Name Mod Length: " << modSize << endl;
	int fnl = binaryToInt(binary.substr(index, 8), 8);
	cout << "Read File Name Length: " << fnl << endl;
	index += 8;
	
	//Read file name
					//~ char c[0];
					//~ cout << "Filename: ";
					//~ string fileName = "";
					//~ for(int i = 0; i < modSize; i++){
						//~ in.read(c, 1);
						//~ if(i < nameSize){
							//~ bitset<8> fn((int) c[0]);
							//~ fileName += (char) binaryToInt(fn.to_string(), 8);
						//~ }
					//~ }
					//~ cout << fileName << endl;
					//~ //in.read(silence, 32);
	
	cout << "Read File Name: ";
	for(int i = 0; i < fnl; i++){
		cout << (char) binaryToInt(binary.substr(index, 8), 8);
		index += 8;
	}
	cout << endl;
	
	
	//Read Entries Length
					//~ //char entriesLen[8];
					//~ in.read(c, 8);
					//~ bitset<8> entLen((int) c[0]);
					//~ int entriesLength = binaryToInt(entLen.to_string(), 8);
					//~ cout << "Entries: " << entriesLength << endl;
	int el = binaryToInt(binary.substr(index, 8), 8);
	index += 8;
	cout << "Read Entries Length: " << el << endl;
	
	
	//Read Codes
	int cl = 0;
	string codes[128];
	cout << "Read Codes:" << endl;
	for(int i = 0; i < el; i++){
		int ascii = binaryToInt(binary.substr(index, 8), 8);
		cout << "ASCII Value: " << ascii;
		index += 8;
		cl = binaryToInt(binary.substr(index, 8), 8);
		index += 8;
		cout << " Bits In Code: " << cl;
		cout << " Actual Binary Code: ";
		for(int j = 0; j < cl; j++){
			cout << binary[index];
			codes[ascii] += binary[index];
			index++;
		}
		cout << endl;
	}
	
	//Read Encode Length
	int tel = binaryToInt(binary.substr(index, 32), 32);
	index += 32;
	cout << "Read Encoded Length In Bits: " << tel << endl;
	
	//~ string pattern = "";
	//~ int pl = 0;
	//~ while(index < size){
		//~ pattern += binary[index];
		//~ pl++;
		//~ for(int i = 0; i < 128; i++){
			//~ for(int j = 0; j < pl; j++){
			//~ }
		//~ }
	//~ }
	
					//~ int codes[128][50];
					//~ int codeLen[128];
					//~ for(int i = 0; i < 128; i++)
						//~ codeLen[i] = 0;
					//~ for(int i = 0; i < 128; i++)
						//~ for(int j = 0; j < 50; j++)
							//~ codes[i][j] = 2;
					//~ for(int i = 0; i < binaryToInt(entLen.to_string(), 8); i++){
						//~ char ascii[8];
						//~ in.read(ascii, 8);
						//~ cout << "Ascii Value: " << binaryToInt(ascii, 8);
						//~ char len[8];
						//~ in.read(len, 8);
						//~ cout << " Number of Bits: " << binaryToInt(len, 8) << " Code: ";
						//~ for(int j = 0; j < binaryToInt(len, 8); j++){
							//~ char c[1];
							//~ in.read(c, 1);
							//~ codes[binaryToInt(ascii, 8)][j] = charToInt(c[0]);
							//~ codeLen[binaryToInt(ascii, 8)]++;
							//~ cout << binaryToInt(c, 1);
						//~ }
						//~ cout << endl;
					//~ }
					//~ cout << "Bits in Encoded Text: ";
					//~ char bitLen[32];
					//~ in.read(bitLen, 32);
					//~ int bitLenInt = binaryToInt(bitLen, 32);
					//~ cout << bitLenInt << endl;
					cout << "Text Decoded: ";
					char current[1];
					int patCount = 0;
					char pattern[50];
					for(int i = 0; i < 50; i++)
						pattern[i] = '3';
					bool mismatch = true;
					bool wrong = false;
					int numOfMatch = 0;
					for(int i = index; i < index + tel; i++){
						current[0] = binary[i];
						pattern[patCount] = current[0];
						patCount++;
						for(int j = 0; j < 128; j++){
							if(codes[j][0] != 2){
								//cout << "####" << (char) j << "####" << endl;
								//cout << "Pattern: ";
								wrong = false;
								for(int q = 0; q < patCount && !wrong; q++){
									//cout << pattern[q];
									if(pattern[q] == codes[j][q]){
										//cout << "match" << endl;
										mismatch = false;
										numOfMatch++;
									}
									else{
										//cout << "mismatch" << endl;
										mismatch = true;
										wrong = true;
									}
								}
								//cout << " Number of match: " << numOfMatch << endl;
								if(numOfMatch == codes[j].length() && !mismatch && codes[j].length() != 0){
									//cout << "numOfMatch: " << numOfMatch << " = " << " codeLen[" << j << "]: " << codes[j].length() << " && mismatch: " << mismatch << " && codeLen[" << j << "]: " << codes[j].length() << " !=  0" << endl; 
									cout << (char)j;
									patCount = 0;
									j = 128;
									mismatch = false;
									cout << "(" << i << ")";
								}
								numOfMatch = 0;
							}
						}
					}
					
}
	
int main(int argc, char *argv[]){
	int array[100];
	heapClass * heap;
	heap = new heapClass;
	huffmanClass * tree;
	tree = new huffmanClass;
	read(heap, argv[1]);
	tree->create(heap);
	tree->createCodes(tree->root, array, 0);
	//cout << tree->leaves << endl;
	tree->printCodes();
	compressFile(argv[1], tree->leaves, tree);
	string filename(argv[1]);
	filename += ".mcp";
	readCompressed(filename);
	return 0;
}
