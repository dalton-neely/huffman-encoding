#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class nodeClass{
	private:
		unsigned int frequency, ascii;
		nodeClass * left, * right;
	public:
		nodeClass();
		nodeClass(unsigned int, unsigned int);
		nodeClass(nodeClass &);
		unsigned int getAscii();
		char getChar();
		unsigned int getFrequency();
		void setLeft(nodeClass *);
		void setRight(nodeClass *);
		nodeClass * getLeft();
		nodeClass * getRight();
		bool leaf();
		void swap(nodeClass &);
		void reset();
		void increase();
		void print();
		void setAscii(unsigned int);
		void setFrequency(unsigned int);
};

nodeClass::nodeClass(){
	frequency = ascii = 0;
	left = right = nullptr;
}

nodeClass::nodeClass(unsigned int _ascii, unsigned int _frequency){
	ascii = _ascii;
	frequency = _frequency;
	left = right = nullptr;
}

nodeClass::nodeClass(nodeClass & _node){
	ascii = _node.getAscii();
	frequency = _node.getFrequency();
	left = _node.getLeft();
	right = _node.getRight();
}

unsigned int nodeClass::getAscii(){
	return ascii;
}

char nodeClass::getChar(){
	return (char) ascii;
}

unsigned int nodeClass::getFrequency(){
	return frequency;
}

void nodeClass::setLeft(nodeClass * _node){
	left = _node;
}

void nodeClass::setRight(nodeClass * _node){
	right = _node;
}

nodeClass * nodeClass::getLeft(){
	return left;
}

nodeClass * nodeClass::getRight(){
	return right;
}

bool nodeClass::leaf(){
	return (left == nullptr && right == nullptr);
}

void nodeClass::swap(nodeClass & _node){
	nodeClass temp(*this);
	ascii = _node.ascii;
	frequency = _node.frequency;
	left = _node.left;
	right = _node.right;
	_node.ascii = temp.ascii;
	_node.frequency = temp.frequency;
	_node.left = temp.left;
	_node.right = temp.right;
}

void nodeClass::reset(){
	ascii = frequency = 0;
	left = right = nullptr;
}

void nodeClass::increase(){
	frequency++;
}

void nodeClass::setAscii(unsigned int _ascii){
	ascii = _ascii;
}

void nodeClass::setFrequency(unsigned int _frequency){
	frequency = _frequency;
}

class heapClass{
	private:
		unsigned int capacity, size;
		nodeClass nodes[128];
	public:
		heapClass(unsigned int);
		void insert(nodeClass &);
		nodeClass getNode(unsigned int);
		bool isOne();
		void sortSubTree(unsigned int);
		void printArray();
		nodeClass extract();
		void sort();
		void read(string);
		void setPtr(int);
};

heapClass::heapClass(unsigned int _capacity){
	capacity = _capacity;
	size = 0;
	for(int index = 0; index < _capacity; index++){
		nodeClass node(0,0);
		nodes[index] = node;
	}
}

void heapClass::insert(nodeClass &_node){
	if(_node.getAscii() <= 128 && _node.getAscii() >= 0 && _node.getFrequency() > 0){
		nodes[size].setAscii(_node.getAscii());
		nodes[size].setFrequency(_node.getFrequency());
		nodes[size].setLeft(_node.getLeft());
		nodes[size].setRight(_node.getRight());
		size++;
		sort();
	}else{
		cout << "Error: heapClass::insert()" << endl;
	}
}

nodeClass heapClass::getNode(unsigned int _index){
	return nodes[_index];
}

bool heapClass::isOne(){
	if(size == 1)
		return true;
	else
		return false;
}

void heapClass::sortSubTree(unsigned int index){
	if(index >= 0){
		unsigned int smallestFrequency = index;
		unsigned int leftChildIndex = 2 * index + 1;
		unsigned int rightChildIndex = 2 * index + 2;
		
		if(leftChildIndex < size && nodes[leftChildIndex].getFrequency() < nodes[smallestFrequency].getFrequency())
			smallestFrequency = leftChildIndex;
		if(rightChildIndex < size && nodes[rightChildIndex].getFrequency() < nodes[smallestFrequency].getFrequency())
			smallestFrequency = rightChildIndex;
		if(smallestFrequency != index){
			nodes[index].swap(nodes[smallestFrequency]);
			sortSubTree(smallestFrequency);
		}
	}
}


void heapClass::printArray(){
	cout << "#######" << endl;
	for(int index = 0; index < size; index++)
		cout << index << " -> " << nodes[index].getFrequency() << endl;
	cout << "#######" << endl;
}

nodeClass heapClass::extract(){
	nodeClass temp(nodes[0]);
	nodes[0].swap(nodes[--size]);
	nodes[size].reset();
	sortSubTree(0);
	return temp;
}

void heapClass::sort(){
	if(size <= 3)
		sortSubTree(0);
	else{
		for(int index = (size -2) /2; index >= 0; index--)
				sortSubTree(index);
	}
}

void heapClass::read(string _file){
	ifstream in;
	char currentCharacter;
	unsigned int frequency = 0;
	for(int i = 0; i < capacity; i++){
		in.open(_file);
		while(in.get(currentCharacter)){
			if(currentCharacter == (char) i)
				frequency++;
		}
		if(frequency != 0){
			nodeClass node(i, frequency);
			insert(node);
			frequency = 0;
		}
		in.close();
	}
}

void heapClass::setPtr(int _index){
	int left = (_index * 2) + 1;
	int right = (_index * 2) + 2;
	if(left < size)
		nodes[_index].setLeft(&nodes[left]);
	else
		nodes[_index].setLeft(nullptr);
	if(right < size)
		nodes[_index].setRight(&nodes[right]);
	else
		nodes[_index].setRight(nullptr);
	if(_index < size)
		setPtr(_index + 1);
}

class subTreeClass{
	private:
		nodeClass * root, *left, *right;
		unsigned int frequency, ascii;
	public:
		subTreeClass(unsigned int, unsigned int, unsigned int, unsigned int);
		subTreeClass(nodeClass *, nodeClass *);
		nodeClass * getRoot();
};

subTreeClass::subTreeClass(unsigned int _ascii1, unsigned int _frequency1, unsigned int _ascii2, unsigned int _frequency2){
	nodeClass newLeft(_ascii1, _frequency1);
	nodeClass newRight(_ascii2, _frequency2);
	nodeClass newRoot(128, _frequency1 + _frequency2);
	newRoot.setLeft(&newLeft);
	newRoot.setRight(&newRight);
	left = &newLeft;
	newLeft.setLeft(nullptr);
	newLeft.setRight(nullptr);
	newRight.setLeft(nullptr);
	newRight.setRight(nullptr);
	right = &newRight;
	root = &newRoot;
}

subTreeClass::subTreeClass(nodeClass * _a, nodeClass * _b){
	nodeClass newRoot(128, _a->getFrequency() + _b->getFrequency());
	newRoot.setLeft(_a);
	newRoot.setRight(_b);
	if(_a->getAscii() != 128){
		_a->setLeft(nullptr);
		_a->setRight(nullptr);
	}
	if(_b->getAscii() != 128){
		_b->setLeft(nullptr);
		_b->setRight(nullptr);
	}
	left = _a;
	right = _b;
	root = &newRoot;
}

nodeClass * subTreeClass::getRoot(){
	return root;
}

class huffmanClass{
	private:
		unsigned int size, capacity;
		nodeClass nodes[255];
		nodeClass * root;
		int codes[128][100];
	public:
		huffmanClass();
		void insert(heapClass &);
		void insert2(heapClass);
		nodeClass * getRoot();
		nodeClass * getLeftMost(nodeClass *);
		void inOrderTrav(nodeClass *);
		void print1DArray(int *, int);
		void createCodes(nodeClass *, int *, int );
};

huffmanClass::huffmanClass(){
	size = 0;
	capacity = 255;
	for(int index = 0; index < capacity; index++){
		nodeClass newNode(0,0);
		nodes[index] = newNode;
	}
	for(int index = 0; index < 128; index++)
		for(int i = 0; i < 100; i++)
			codes[index][i] = 2;
}

void huffmanClass::insert(heapClass & _heap){
	if(!_heap.isOne()){
		nodeClass smallest(0,0);
		nodeClass smallest2(0,0);
		smallest = _heap.extract();
		cout << (char) smallest.getAscii() << " with frequency " << smallest.getFrequency() << " is being combined with ";
		smallest2 = _heap.extract();
		cout << (char) smallest2.getAscii() << " with frequency " << smallest2.getFrequency() << endl;
		//subTreeClass newSubTree(smallest.getAscii(), smallest.getFrequency(), smallest2.getAscii(), smallest2.getFrequency());
		subTreeClass newSubTree(&smallest, &smallest2);
		nodeClass * nodePtr = newSubTree.getRoot();
		root = nodePtr;
		_heap.insert(*nodePtr);
		insert(_heap);
	}
}

void huffmanClass::insert2(heapClass _heap){
	if(!_heap.isOne()){
		nodeClass a(0,0);
		nodeClass b(0,0);
		a = _heap.extract();
		cout << "Extracting " << a.getAscii() << " with frequecny " << a.getFrequency() << " Loc: " << &a << endl;
		b = _heap.extract();
		cout << "Extracting " << b.getAscii() << " with frequecny " << b.getFrequency() << " Loc: " << &b << endl;
		
		nodeClass newRoot(128, a.getFrequency() + b.getFrequency());		
		if(a.getAscii() != 128){
			a.setLeft(nullptr);
			a.setRight(nullptr);
		}
		if(b.getAscii() != 128){
			b.setLeft(nullptr);
			b.setRight(nullptr);
		}
		newRoot.setLeft(&a);
		newRoot.setRight(&b);
		cout << "NewNode " << newRoot.getAscii() << " with frequecny " << newRoot.getFrequency() << " Loc: " << &newRoot << " Left Loc: " << newRoot.getLeft() << " Right Loc: " << newRoot.getRight() << endl;
		root = &newRoot;
		_heap.insert(newRoot);
		//cout << "[" << (char) a.getAscii() << "," << a.getFrequency() << "," << &a << "," << a.getLeft() << "," << a.getRight() << "] + ";
		//cout << "[" << (char) b.getAscii() << "," << b.getFrequency() << "," << &b << "," << b.getLeft() << "," << b.getRight() << "] = ";
		//cout << "[" << (char) newRoot.getAscii() << "," << newRoot.getFrequency() << "," << &newRoot << "," << newRoot.getLeft() << "," << newRoot.getRight() << "]" << endl;
		insert2(_heap);
	}
}

nodeClass * huffmanClass::getRoot(){
	return root;
}

nodeClass * huffmanClass::getLeftMost(nodeClass * _ptr){
	if(_ptr->getAscii() == 128){
		getLeftMost(_ptr->getLeft());
	}else{
		return _ptr;
	}
}

void huffmanClass::inOrderTrav(nodeClass * _ptr){
	if(!_ptr->leaf()){
		cout << "here" << endl;
		if(_ptr->getLeft() != nullptr)
			inOrderTrav(_ptr->getLeft());
		cout << "[" << _ptr->getFrequency() << "," << _ptr->getAscii() << "]" << endl;
		inOrderTrav(_ptr->getRight());
	}
}

void huffmanClass::print1DArray(int * _array, int _index){
	for(int index = 0; index < _index; index++)
		cout << _array[index];
}

void huffmanClass::createCodes(nodeClass * root, int * _code, int index){
	//cout << "createCodes -> " << root << " -> index: " << index << endl;
	if(root->getLeft() != nullptr){
		//cout << "in left" << endl;
		//cout << root->getLeft()->getAscii() << "frequency: " << root->getLeft()->getFrequency() << endl;
		_code[index] = 0;
		createCodes(root->getLeft(), _code, index + 1);
	}
	if(root->getRight() != nullptr){
		//cout << "in Right" << endl;
		_code[index] = 1;
		createCodes(root->getRight(), _code, index + 1);
	}
	if(root->leaf()){
		if(root){
			cout << (char) root->getAscii() << "(" << root->getAscii() << ")" << " = ";
			print1DArray(_code, index);
			cout << endl;
			for(int i = 0; i < index; i++){
				//cout << "index: " << index << "||| ascii: " << root->getAscii() << endl;
				codes[root->getAscii()][i] = _code[i];
			}
		}
	}
}

int main(int argc, char *argv[]){
	heapClass heap(128);
	heap.read(argv[1]);
	huffmanClass huffman;
	huffman.insert2(heap);
	//cout << huffman.getRoot()->getFrequency() << endl;
	//cout << huffman.getRoot()->getAscii() << endl;
	//cout << huffman.getRoot()->getLeft()->getFrequency() << " + ";
	//cout << huffman.getRoot()->getRight()->getFrequency() << endl;
	unsigned int test = 0;
	for(int i = 0; i < 71; i++){
		test += heap.getNode(i).getFrequency();
	}
	//cout << test << endl;
	nodeClass * root;
	root = huffman.getRoot();
	int arr[100];
	huffman.createCodes(root, arr ,0);
	//huffman.inOrderTrav(root);
	//cout << root->getFrequency() << endl;
	//huffman.inOrderTrav(root);
	//heap.setPtr(0);
	return 0;
}
