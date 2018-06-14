#ifndef NODECLASS_HPP
#define NODECLASS_HPP

#include <iostream>

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

#endif
