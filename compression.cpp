#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class frequencyClass{
	private:
		int asciiValue, count;
		frequencyClass *leftChild, *rightChild, *parent;
	public:
		frequencyClass();
		void setLeftChild(frequencyClass);
		frequencyClass * getLeftChild();
		void setRightChild(frequencyClass);
		frequencyClass * getRightChild();
		void setParent(frequencyClass);
		frequencyClass * getParent();
		void setAscii(int value);
		int getAscii();
};

void frequencyClass::setAscii(int value){
	asciiValue = value;
}

int frequencyClass::getAscii(){
	return asciiValue;
}

frequencyClass::frequencyClass(){
	asciiValue = 0;
	count = 0;
	leftChild = nullptr;
	rightChild = nullptr;
	parent = nullptr;
}

void frequencyClass::setLeftChild(frequencyClass node){
	leftChild = &node;
}

frequencyClass * frequencyClass::getLeftChild(){
	return leftChild;
}
void frequencyClass::setRightChild(frequencyClass node){
	rightChild = &node;
}

frequencyClass * frequencyClass::getRightChild(){
	return rightChild;
}
void frequencyClass::setParent(frequencyClass node){
	parent = &node;
}

frequencyClass * frequencyClass::getParent(){
	return this->parent;
}

int main(int argc, char *argv[]){
	
	//Allocate memory to hold current char
	char c;
	//Allocate memory for array that holds ASCII frequency values
	int frequency[128];
	
	frequencyClass node;
	frequencyClass node1;
	frequencyClass node2;
	node1.setAscii(444);
	node2.setAscii(777);
	node.setAscii(2);
	node.setParent(node1);
	node.setRightChild(node2);
	//cout << node.getLeftChild()->getAscii() << endl;
	cout << node.getParent()->getAscii() << endl;
	cout << node.getRightChild()->getAscii() << endl;
	
	//Initialize the array
	for(int i = 0; i < 128; i++){
		frequency[i] = 0;
	}
	
	//Open the file passed to the program
	ifstream inStream;
	inStream.open(argv[1]);
	//Loop through the file
	while(inStream.get(c)){
		frequency[(int) c]++;
	}
	
	//Print the frequencies
	//for(int i = 0; i < 128; i++){
		//cout << (char) i << " ---> " << frequency[i] << endl;
	//}
	return 0;
}
