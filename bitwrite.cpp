#include <iostream>
#include <fstream>
#include <bitset>
#include <string>

using namespace std;

int main(int argc, char **argv){
	char insert;
	ofstream out;
	out.open("testbin.txt", ofstream::out | ofstream::binary);
	bitset<8> bits;
	bits.set();
	bits.set(4, 0);
	insert = (char) bits.to_ulong();
	char array[1];
	array[0] = insert;
	bitset<8> newbit((int) insert);
	out.write(array, 1);
	cout << newbit << endl;
	return 0;
}
