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

void compressFile(string _file, int _entries, huffmanClass * _tree){
	string newFile = _file + ".mcp";
	ofstream out;
	out.open(newFile, ofstream::out | ofstream::binary);
	bitset<32> magicNum(0x1AF69384);
	bitset<32> silence(0x00000000);
	out.write(magicNum.to_string().c_str(), 32);
	out.write(silence.to_string().c_str(), 32);
	int nameSize = _file.length();
	int modSize = nameSize;
	if(nameSize % 4 != 0){
		int diff = 4 - (nameSize % 4);
		modSize += diff;
	}
	bitset<8> nameSizeByte(nameSize);
	out.write(nameSizeByte.to_string().c_str(), 8);
	for(int i = 0; i < modSize; i++){
		if(i < nameSize){
			bitset<8> current((int) _file[i]);
			out.write(current.to_string().c_str(), 8);
		}else{
			bitset<8> current(0x00);
			out.write(current.to_string().c_str(), 8);
		}
	}
	out.write(silence.to_string().c_str(), 32);
	bitset<8> numOfEntries(_entries);
	out.write(numOfEntries.to_string().c_str(), 8);
	int encodeLength = 0;
	for(int i = 0; i < 128; i++){
		if(_tree->codes[i][0] != 2){
			bitset<8> ascii(i);
			out.write(ascii.to_string().c_str(), 8);
			for(int j = 0; j < 50; j++){
				if(_tree->codes[i][j] != 2){
					encodeLength++;
				}
			}
			bitset<8> eLength(encodeLength);
			out.write(eLength.to_string().c_str(), 8);
			char array[1];
			for(int q = 0; q < encodeLength; q++){
				array[1] = (char) _tree->codes[i][q];
				out.write(array, 1);
			}
			encodeLength = 0;
		}
	}
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
	char magicNumber[32];
	in.read(magicNumber, 32);
	string mgcNum(magicNumber);
	cout << "Magic Number: " << binaryToInt(mgcNum, 32) << endl;
	char silence[32];
	in.read(silence, 32);
	char fileNameSize[8];
	in.read(fileNameSize, 8);
	string fileNameSizeStr(fileNameSize);
	int nameSize = binaryToInt(fileNameSizeStr, 8);
	int modSize = nameSize;
	if(nameSize % 4 != 0){
		int diff = 4 - (nameSize % 4);
		modSize += diff;
	}
	char name[nameSize];
	cout << "Filename: ";
	for(int i = 0; i < modSize; i++){
		in.read(name, 8);
		if(i < nameSize){
			string nameStr(name);
			cout << (char) binaryToInt(nameStr, 8);
		}
	}
	cout << endl;
	in.read(silence, 32);
	char entriesLen[8];
	in.read(entriesLen, 8);
	cout << "Entries: " << binaryToInt(entriesLen, 8) << endl;
	for(int i = 0; i < binaryToInt(entriesLen, 8); i++){
		char ascii[8];
		in.read(ascii, 8);
		cout << "Ascii Value: " << binaryToInt(ascii, 8);
		char len[8];
		in.read(len, 8);
		cout << " Number of Bits: " << binaryToInt(len, 8) << " Code: ";
		for(int j = 0; j < binaryToInt(len, 8); j++){
			char c[1];
			in.read(c, 1);
			cout << binaryToInt(c, 1);
		}
		cout << endl;
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
