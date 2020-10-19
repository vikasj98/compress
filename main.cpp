#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>

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
		/*
		Pick the first 2 elements. Create 2 nodes out of them. If these two characters were non zero values 
		that means they will be leaves, else there value will be zero. Make a new node adding the values of the two nodes and 			assign its character as zero. Then remove the original 2 nodes from the vector and add the new node. Repeat the process. 			At the end when we have only 2 nodes left let them be the children of root node.
		*/
	}while(false);//  (frequencyVect.size() > 1);
	



	return 0;


}
