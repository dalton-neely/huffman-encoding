#ifndef HUFFMANCLASS_HPP
#define HUFFMANCLASS_HPP

#include <iostream>
#include "nodeClass.hpp"
#include "heapClass.hpp"

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

#endif
