#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <string>
#include <bitset>

using namespace std;


struct Node
{
	char character;
	Node* left;
	Node* right;
	int frequency;
};

struct charAndFrequency
{
	char character;
	int frequency;
};

bool comparator(Node* struct1, Node* struct2)
{
	return (struct1->frequency < struct2->frequency);
}

/*
void traverseTree(Node* current, string directions)
{
	if(current->character != 0)
	{
		//We are at the leaf
		cout << current->character <<  " : " << current->frequency << " : " << directions << endl;
		return;
	}
	if(0 != current->left)
	{
		traverseTree(current->left, directions+'0');	//0 to the left
	}
	if(0 != current->right)
	{
		traverseTree(current->right, directions+'1');
	}
}
*/
/*
The second argument has all the directions that were taken to reach upto this node. Directions start from MSB. 0 means left, 1 means right
*/
void traverseTree(Node* current, unsigned char directions, int depth)
{
	//cout << "depth : " << depth << ", directions : " << int(directions) << endl;
	if(current->character != 0)
	{
		//We reached a leaf node, we have encoding for a character here
		bitset<8> x(directions);
		//cout << "directions " << int(directions) << endl;
		cout << current->character << " : " << current->frequency << " : " << depth << " : " << x << endl;;
		for(int i=depth-1; i>=0; i--)
		{
			cout<<x[i];
		}
		cout << endl;
		//<< x << endl;
		return;
	}
	if(current->left != 0)
	{
		traverseTree(current->left, directions << 1, depth+1);
	}
	if(current->right != 0)
	{
		traverseTree(current->right, ((directions << 1) | 1), depth+1);
	}
}

int main()
{
	char document[] = "Hello, this is a sample string for which I will try to make a huffman tree";
	int frequencies[256] = {};
	
	for(int i=0; i<=strlen(document); i++)
	{
		frequencies[document[i]]++;
	}
	
	vector<charAndFrequency> frequencyVect;

	const int MAX_CHARACTERS = 256;
	for(int i=0; i<MAX_CHARACTERS; i++)
	{
		if(frequencies[i] != 0)
		{
			charAndFrequency tmp = {
					static_cast<char>(i), //Character
					frequencies[i] //Frequency of the character
				};
			frequencyVect.push_back(tmp);
			//cout << char(i) << " : " << frequencies[i] << endl;
		}

	}
	vector<Node*> nodeVect;
	for(int i=0; i<frequencyVect.size(); i++)
	{
		Node *tmp = new Node();
		tmp->character = frequencyVect[i].character;
		tmp->frequency = frequencyVect[i].frequency;
		nodeVect.push_back(tmp);
	} 
	
	cout << "Before Sorting " << endl;
	for(int i=0; i<nodeVect.size(); i++)
	{
		cout << nodeVect[i]->character << " : " << nodeVect[i]->frequency << endl;
	} 
	cout<< " About to sort " << endl;
	sort(nodeVect.begin(), nodeVect.end(), comparator);
	
	cout << "After sorting "<< endl;
	
	for(int i=0; i<nodeVect.size(); i++)
	{
		cout << nodeVect[i]->character << " : " << nodeVect[i]->frequency << endl;
	} 
	
	do
	{
		Node *tmp1;
		Node *tmp2;
		tmp1 = nodeVect[0];
		tmp2 = nodeVect[1];
		Node *newNode = new Node();
		newNode->left = tmp1;
		newNode->right = tmp2;
		newNode->character = 0;	//0 represents a non leaf node
		newNode->frequency = tmp1->frequency + tmp2->frequency;
		nodeVect.erase(nodeVect.begin());	//Removes the 0th element. After this the 1st element will become 0th element
		nodeVect.erase(nodeVect.begin());	//Removed the element 1
		nodeVect.push_back(newNode);	//Added the new node to the list. Now the process will be repeated
		sort(nodeVect.begin(), nodeVect.end(), comparator);
	}while(nodeVect.size() > 1);
	Node* root = nodeVect[0];
	traverseTree(root->left, 0, 1);
	traverseTree(root->right, 1, 1);
	//traverseTree(root->left, string("0"));
	//traverseTree(root->right, string("1"));
	return 0;


}
