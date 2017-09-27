#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char **argv){
	char c;
	ifstream in;
	in.open(argv[1], ifstream::in |ifstream::binary);
	while(in.get(c)){
		cout << c;
	}
	return 0;
}
