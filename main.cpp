#include<iostream>
#include <unordered_map>

using namespace std;

template<typename T>
class Node {
public:
	T data;
	Node<T>* left, * right;
	friend ostream& operator<<(ostream& os, shared_ptr<Node<int>> node);
};
ostream& operator<<(ostream& os, shared_ptr<Node<int>> node)
{
	if (!node)
		return os;
	os << (int)(node->data) << ' ' << (shared_ptr<Node<int>>)(node->left) << (shared_ptr<Node<int>>)(node->right);
	return os;
}
namespace Tree {
	Node<int>* newNode(int data);
	Node<int>* buildTree(int centerArr[], int reverseArr[], int len);
	Node<int>* buildStep(int centerArr[], int reverseArr[], 
		int inStartIndex, int inEndIndex, 
		int* pCurrentIndex, 
		unordered_map<int, int>& hashTableCenter);
}

Node<int>* Tree::buildStep(int centerArr[], int reverseArr[], 
	int inStartIndex, int inEndIndex, 
	int* pCurrentIndex, 
	unordered_map<int, int>& hashTableCenter)
{
	// recursion stop condition
	if (inStartIndex > inEndIndex)
		return NULL;
	// create node
	int curr = reverseArr[*pCurrentIndex];
	Node<int>* node = Tree::newNode(curr);
	// single threaded - so safety
	(*pCurrentIndex)--;
	
	// if node has no children - return
	if (inStartIndex == inEndIndex)
		return node;
	// get local root index from hashtable
	int iIndex = hashTableCenter[curr];

	node->right = Tree::buildStep(centerArr, reverseArr, 
		iIndex + 1, inEndIndex, 
		pCurrentIndex, 
		hashTableCenter);
	node->left = Tree::buildStep(centerArr, reverseArr, 
		inStartIndex, iIndex - 1, 
		pCurrentIndex, 
		hashTableCenter);

	return node;
}


Node<int>* Tree::buildTree(int centerArr[], int reverseArr[], int len)
{
	//hash table for fast search by key
	unordered_map<int, int> hashTableCenter;
	for (int i = 0; i < len; i++)
		hashTableCenter[centerArr[i]] = i;

	int index = len - 1;
	//The last item in reverseArr is root, which also divides the subtrees in the centerArr
	//We move the pointer from the end of reverseArr and divide recursively into subtrees at each step
	return Tree::buildStep(centerArr, reverseArr, 0, len - 1,&index, hashTableCenter);
}

Node<int>* Tree::newNode(int data)
{
	Node<int>* node = new Node<int>();
	node->data = data;
	node->left = node->right = NULL;
	return node;
}

int main()
{
	int centerArr[] = { 4, 8, 2, 5, 1, 6, 3, 7 };
	int reverseArr[] = { 8, 4, 5, 2, 6, 7, 3, 1 };
	int forwardArr[] = { 1, 2, 4, 8, 5, 3, 6, 7 };
		
	int n = sizeof(centerArr) / sizeof(centerArr[0]);

	shared_ptr<Node<int>> root = (shared_ptr<Node<int>>)Tree::buildTree(centerArr, reverseArr, n);

	cout << "Preorder of the constructed tree : \n";
	cout << root;

	return 0;
}
