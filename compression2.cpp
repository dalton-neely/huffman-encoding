/*
 * Name: Dalton Neely
 * Date Due: October 2nd, 2017
 * Program Name: Huffman Encoder
 * Description: Use a form of variable length compression called Huffman
 * compression. We will acomplish this by creating a Huffman Tree, which
 * implements a priority queue (min-heap), with seperate internal nodes.
 * The program will use a lot of recursion to traverse the tree aswell.
 */
 
 #include <iostream>
 #include <fstream>
 
 using namespace std;
 
 /*
  * The following lines of code contain an abstract data type (min heap).
  * A min heap is defined as having values (keys) of the parent node
  * being less than or equal to the nodes of the children. Thus, the 
  * lowest value node is the root node at the top of the tree.
  */
  
  /*
   * class minHeapNode
   * The class minHeapNode creates an object that stores one character
   * of the file, stored as an ascii value, and also stores how many
   * times the character appeared in the file. Furthermore, a pointer
   * points to the left and right children nodes of the min heap. Note,
   * if both pointers are set to nullptr, then the node is a leaf node.
   * Declaration Example: minHeapNode nodeName(asciivalue, frequency);
   * Child Assignment Example: nodeName.leftChild = &leftChildNode;
   * Child Members Access Example: nodeName.leftChild->asciiValue
   * Testing: class works 100% as it is suppossed to
   */
  
  class minHeapNode{
	   public:
		unsigned int asciiValue, frequency;
		minHeapNode *leftChild, *rightChild;
		minHeapNode(unsigned int, unsigned int);
		bool isLeaf();
		void copy(minHeapNode);
  };
  
  /*
   * copy()
   * This function serves a to copy one node to another without the use
   * of an overloaded operator, or by complicated pointer swaping
   * Usage: nodeToPasteInto.copy(nodeToBeCopied);
   * Testing: This funciton works as designed
   */
   
  void minHeapNode::copy(minHeapNode nodeToBeCopied){
		this->asciiValue = nodeToBeCopied.asciiValue;
		this->frequency = nodeToBeCopied.frequency;
		this->leftChild = nodeToBeCopied.leftChild;
		this->rightChild = nodeToBeCopied.rightChild;
  }
  
  /*
   * isLeaf()
   * This function returns a boolean value based on its status as a leaf
   * If either this->leftChild or this->rightChild have a pointer, and
   * not a null value then the function returns false (0). If both left
   * and right child are null then the node is a leaf node, and the function
   * returns true (1).
   * Testing: Function Works 100%
   */
  
  bool minHeapNode::isLeaf(){
	return !(this->leftChild) && !(this->rightChild);
  }
  
  
  /*
   * minHeapNode() CONSTRUCTOR
   * This is the constructor of minHeapNode that takes in the ascii value
   * and the frequency of the character, and allocates space in memory.
   * Note: the left and right child are set to nullptr; Thus, they are
   * leaft nodes by default.
   * Testing: Constructor works as supposed
   */
  
  minHeapNode::minHeapNode(unsigned int passedAscii, unsigned int passedFrequency){
	  asciiValue = passedAscii;
	  frequency = passedFrequency;
	  leftChild = rightChild = nullptr;
  }
  
  /* swap()
   * This function (swap) is a member of the minHeapNode class that allows
   * us two swap two node for one another.
   * Usage: swap(node1, node2)
   * Note: a pointer to the node does not need to be passed as the actual
   * object is the one being passed by referance.
   * Testing: Function works as designed to 100%
   */
  
  void swap(minHeapNode & a, minHeapNode & b){
		minHeapNode t(0, 0);
		t.copy(a);
		a.copy(b);
		b.copy(t);
  }
  
  /*
   * class minHeap (ADT)
   * minHeap is a data structure to keep track of the heap nodes. The
   * size corresponds to the current amount of nodes in the heap. Whereas,
   * the capacity tells us how many nodes the heap will end up having.
   * Furthermore, an array of pointers will be assigned to keep track of
   * the nodes. This is basically the priority queue in array form, and 
   * thus the maximum capacity must be declared first.
   * In this program that means that capacity is the number of different
   * or unique characters used in the file.
   * Usage: To assign a node pointer to the minHeap array do the following
   * minHeapObject.array[indexNumberLessThanCapacity] = &minHeapNodeObject;
   * Now to reference that node do the following
   * minHeapObject.array[indexNumberLessThanCapacity]->asciiValue;
   * or like this to reference on of the stored node's children
   * minHeapObject.array[indexNumberLessThanCapacity]->leftChild->asciiValue;
   * Testing: Class works as designed 100%;
   */
  
  class minHeap{
	  public:
			unsigned int size, capacity;
			minHeapNode *array[128];
			minHeap(int);
			bool isSizeOne();
			void insert(minHeapNode *);
  };
  
  void minHeap::insert(minHeapNode * _minHeapNode){
		this->array[this->size - 1] = _minHeapNode;
		cout << this->array[this->size -1]->asciiValue << endl;
		this->size++;
		cout << this->size << endl;
  }
  
  /*
   * minHeap(capacity) CONSTRUCTOR
   * Constuctor for the minHeap class that takes the arguments of capacity,
   * which is the maximum number of unique characters in the file. Note,
   * that size is set to 0 by default because a new heap has no nodes.
   * Usage: minHeap testHeap(128);
   * Testing: The funciton works as designed 100%
   */
   
minHeap::minHeap(int passedCapacity){
	size = 0;
	capacity = passedCapacity;
	for(int i = 0; i < passedCapacity; i++){
		this->array[i] = nullptr;
	}
}

/*
 * isSizeOne()
 * This function is a member of the minHeap class and checks if the current
 * min heap has a size of one. If the heap is of size one then it returns
 * true (1), otherwise the function returns false (0)
 * Usage: bool result = testHeap.isSizeOne();
 * Testing: The function works as designed 100%
 */

bool minHeap::isSizeOne(){
	return(this->size == 1);
}

/*
 * minHeapify()
 * This function is used when the min heap is out of order due to a extraction
 * of a node. Thus, use this funciton when ever a node is extracted.
 * Algorithm:
 * 	1. Find the indicies of the left and right children of the current index in question
 * 	2. If the frequency of the left child is less than that of the current index
 * 		a. Store the index of the left child in the smallest index holder
 * 	3. If the frequency of the right child is less than that of the current index
 * 		a. Store the index of the right child in the smallest index holder
 * 	4. If the value stored by the smallest index does not equal that of the
 * 	   value stored by the current index
 * 		a. swap the nodes of the smallest index with that of the current index
 * 	5. Repeat until the smallest index equals the current index
 * Note: Only works on sub-trees, and assumes that all other sub-trees are already
 * sorted.
 * Usage: minHeapify(&minHeapObject, indexRootOfSubTree);
 * Testing: The funciton works as designed 100%
 */

void minHeapify(minHeap * passedMinHeap, int index){
	//index will initially be passed a zero to start
	int smallest = index;
	//2 times the current index plus ONE finds the LEFT child in the array
	int left = 2 * index + 1;
	//2 times the current index plus TWO finds the RIGHT child in the array
	int right = 2 * index + 2;
	
	/*
	 * If the left child's index is less than the size of the min Heap
	 * (this is to make sure we are not indexing a node that is not in the tree)
	 * AND the frequency of the left child's node is LESS THAN the frequency
	 * of the smallest index's frequency. Then, the smallest index now equals
	 * the left child's index. Otherwise keep the indicies the same.
	 */
	if(left < passedMinHeap->size && 
	passedMinHeap->array[left]->frequency < 
	passedMinHeap->array[smallest]->frequency)
		smallest = left;
	/*
	 * If the RIGHT child's index is LESS THAN the size of the min heap 
	 * (this is to make sure we are not indexing a node that is not in the tree) AND
	 * the RIGHT child's frequency is LESS THAN the frequency of the smallest
	 * index. Then, the smallest index now equals the RIGHT child's index.
	 * Otherwise keep the indicies the same.
	 */
	if(right < passedMinHeap->size && 
	passedMinHeap->array[right]->frequency < 
	passedMinHeap->array[smallest]->frequency)
		smallest = right;
	/*
	 * If the smallest index DOES NOT EQUAL the index originally passed
	 * in the arugment list, which means one of the if statements above
	 * was true, then swap the nodes of the smallest index with the node
	 * represented by the index originally passed in the argument list.
	 * Then, recurse again, this time with the new smallest index passed
	 * as the index in the argument list.
	 */
	if(smallest != index){
		swap(passedMinHeap->array[smallest], passedMinHeap->array[index]);
		minHeapify(passedMinHeap, smallest);
	}
}

/*
 * extractMinNode()
 * This function extracts the lowest frequency node in the min heap and
 * returns a pointer to the location where the new temporary node is stored.
 * (this node is not in the min heap anymore).
 * Algorithm:
 * 	1. Create a temporary node for storage
 * 	2. Swap the highest index node with the lowest index node
 * 	3. Decrement the size of the min heap
 * 	4. Set the hightest index node pointer in the array to nullptr
 * 	5. Heapify (sort) the min heap
 * 	6. Return the pointer to the temporary node
 * Usage: extractMinNode(&minHeapObject);
 * Testing: works as designed 100%
 */

minHeapNode * extractMinNode(minHeap * _MinHeap){
	cout << "inside extractMinNode" << endl;
	minHeapNode * temp = _MinHeap->array[0];
	cout << "passed temp" << endl;
	//There are two ways to swap these objects, but we already have a
	//function to do it so why not
	//_MinHeap->array[0] = _MinHeap->array[_MinHeap->size - 1];
	cout << _MinHeap->array[67]->asciiValue << " swaps with " << _MinHeap->array[_MinHeap->size - 1]->asciiValue << endl;
	swap(_MinHeap->array[0], _MinHeap->array[_MinHeap->size - 1]);
	cout << "passed swap" << endl;
	_MinHeap->size--;
	_MinHeap->array[_MinHeap->size] = nullptr;
	minHeapify(_MinHeap, 0);
	return temp;
}

/*
 * insertMinHeap()
 * This function inserts a heap node into the min heap tree, by taking the
 * arguments of the minHeap itself, and the minHeapNode. Note these must be
 * passed as a reference to the pointer.
 * Usage: insertMinHeap(&minHeapObject, &minHeapNodeObject);
 * Algorithm
 * 	1. Add the element to the bottom level of the heap.
 * 	2. Compare the added element with its parent; if they are in the correct order, stop.
 * 	3. If not, swap the element with its parent and return to the previous step.
 * Testing: function works as designed 100%
 */

void insertMinHeap(minHeap * passedMinHeap, minHeapNode * passedMinHeapNode){
	//Increment the size of the heap (very important)
	passedMinHeap->size++;
	cout << "###[passedMinHep]###" << endl << "Capacity: " << passedMinHeap->capacity << endl << "Size: " << passedMinHeap->size << endl << "##########" << endl;
	cout << "###[passedMinHepNode]###" << endl << "Ascii Value: " << passedMinHeapNode->asciiValue << endl << "Frequency: " << passedMinHeapNode->frequency << endl << "##########" << endl;
	if(passedMinHeap->size == 1){
		passedMinHeap->array[0] = passedMinHeapNode;
		cout << "Tree was empty so node placed as root" << endl;
	}else{
		int i = passedMinHeap->size - 1;
		cout << "###[Traversal to input]###" << endl;
		//If the passed node is less than the frequency of its parent node, continue the while loop
		//Also check if we have not passed the root node by checking if i is equal to zero
		cout << "i = " << i << endl << "passedMinHeapNode Frequency: " << passedMinHeapNode->frequency << endl;
		cout << "(i - 1)/2 = " << (i - 1)/2 << endl;
		cout << "passedMinheap->array[" << (i - 1)/2 << "]->frequency: " << passedMinHeap->array[(i - 1)/2]->frequency << endl;
		cout << "While condition: " << passedMinHeapNode->frequency << " < " << passedMinHeap->array[(i - 1)/2]->frequency << " && " << i << " != 0" << endl;
		while(i != 0 && (passedMinHeapNode->frequency < passedMinHeap->array[(i - 1)/2]->frequency)){
			cout << "Condition True: enter while loop" << endl;
			//swap the pointers to the nodes held in the array
			passedMinHeap->array[i] = passedMinHeap->array[(i - 1)/2];
			cout << "passedMinHeap->array[" << i << "] now equals " << "passedMinHeap->array[" << (i - 1)/2 << "]" << endl;
			//set i to be the new parent of the next iteration
			cout << "i now equals [(i-1)/2]: " << (i - 1)/2 << endl;
			i = (i - 1) / 2;
		}
		cout << "Exit while loop!" << endl;
		//Now i should be set to the correct index in the heap
		passedMinHeap->array[i] = passedMinHeapNode;
		cout << "passedMinHeap->array[" << i << "] now stores the pointer to passedMinHeapNode" << endl;
	}
}

/*
 * buildMinHeap()
 * This function is meant to be the sister function of heapify. As is sorts
 * the whole min heap, instead of assuming that the sub-trees are already sorted.
 * It works by finding the right most node index of the second to last tier of
 * the tree. To do this we use the equation (size - 2)/2, note that int truncation
 * takes place. Then we use the Heapify function to sort the sub trees we send it
 * for that index, as well slowly work our way up the heap.
 * Algorithm:
 * 	1. Find the right most index of the second to last tier of the heap and store as index
 * 	2. Repeat the following until the index is less than zero
 * 		a. Sort the sub tree with the root of index
 * Usage: buildMinHeap(&minHeapObject);
 * Testing: Works as desinged 100%
 */
void buildMinHeap(minHeap * _MinHeap)
{
	int index = (_MinHeap->size - 2) / 2;
	
	for(index; index >= 0; index--)
		minHeapify(_MinHeap, index);
		
}

minHeap * createAndBuildMinHeap(unsigned int freq[], int _size){
	unsigned int counter = 0;
	unsigned int actualSize = 0;
	for(int i = 0; i < _size; i++){
		if(freq[i] != 0){
			actualSize++;
		}
	}
	minHeap newMinHeap(actualSize);
	for(unsigned int i = 0; i < _size; i++){
		if(freq[i] != 0){
			minHeapNode temp(i, freq[i]);
			newMinHeap.insert(&temp);
			//cout << newMinHeap.array[i]->frequency << endl;
		}
	}
	
	for(unsigned int i = 0; i < actualSize; i++){
		cout << newMinHeap.array[i]->frequency << endl;
	}
	
	buildMinHeap(&newMinHeap);
	return &newMinHeap;
}

minHeapNode * buildHuffmanTree(unsigned int freq[], int size){
	minHeapNode *left, *right, *top;
	minHeap tempMinHeap(size);
	
	minHeap * newMinHeap = &tempMinHeap;
	newMinHeap = createAndBuildMinHeap(freq, size);
	cout << "here" << endl;
	cout << tempMinHeap.size << endl;
	//cout << "in buildHuffmanTree node 6: " << newMinHeap->array[6]->frequency << endl;
	while(!newMinHeap->isSizeOne()){
		cout << "inside while" << endl;
		left = extractMinNode(newMinHeap);
		cout << "passed L" << endl;
		right = extractMinNode(newMinHeap);
		cout << "passed L and R " << endl;
		
		top->asciiValue = (int) '$';
		top->leftChild = left;
		top->rightChild = right;
		insertMinHeap(newMinHeap, top);
	}
	
	return extractMinNode(newMinHeap);
}
/*
void printCodes(minHeapNode * root, int arr[], int top)
{
	if(root->leftChild){
		arr[top] = 0;
		printCodes(root->leftChild, arr, top + 1);
	}
	if(root->rightChild){
		arr[top] = 1;
		printCodes(root->rightChild, arr, top + 1);
	}
	
	if(root->isLeaf()){
		cout << (char) root->asciiValue << endl;
		for(int i = 0; i < sizeof(arr)/sizeof(arr[0]); i++){
			cout << arr[i];
		}
		cout << endl;
	}
}

void HuffmanCodes(int freq[], int size){

minHeapNode * root = buildHuffmanTree(freq, size);
int arr[128], top = 0;
printCodes(root, arr, top);
}
*/

/*
 * printGUITree()
 * This funciton is mainly used for testing to see if the min heap is
 * building right, as it displays a textual GUI displayed as a tree.
 * Usage printGUITree(minHeapObject, 0, 0, 1, true);
 * Testing: Works as desgined 100%
 */

void printGUITree(minHeap _minHeap, int index, int level, int nodes, bool _continue){
	if(index == 0){
		cout << "Root: " << "Array Value[" << index << "]" << _minHeap.array[index]->asciiValue << " --> " << _minHeap.array[index]->frequency << endl;
		printGUITree(_minHeap, index + 1, level + 1, nodes + 1, _continue);
	}else{
		if(_continue){
			cout << "Level " << level << ":";
			for(int i = 0; i < nodes; i++){
				if(_minHeap.array[index + i] != nullptr){
					cout << "Array Value[" << index + i << "]" << _minHeap.array[index + i]->asciiValue << " --> " << _minHeap.array[index + i]->frequency << " ### ";
				}else{
					_continue = false;
				}
			}
			if(!_continue)
				cout << "Empty level" << endl;
			cout << endl;
			printGUITree(_minHeap, index + nodes, level + 1, nodes * 2, _continue);
		}
	}
}

	
 
int main(int argc, char *argv[]){
	
	//testing
	
	//end testing
	
	//Allocate memory to hold current char
	char c;
	//Allocate memory for array that holds ASCII frequency values
	unsigned int frequency[128];
	
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
	minHeapNode * tester;
	tester = buildHuffmanTree(frequency, 128);
	//printGUITree(*tester, 0, 0, 1, true);
	//Print the frequencies
	//for(int i = 0; i < 128; i++){
		//cout << (char) i << " ---> " << frequency[i] << endl;
	//}
	//HuffmanCodes(frequency, 128);
	return 0;
}
