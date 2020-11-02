#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <string>
#include <bitset>
#include <sys/stat.h>
#include <fstream>

using namespace std;

struct CharacterEncoding
{
	unsigned int encoding;	//Encoding of the character
	char depth;			//Number of bits (first n bits from LSB. First bit is the MSB among these bits)
};

CharacterEncoding CharacterEncodingDictionary[256] = {};

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

int getFileSize(char *fileName)
{
	struct stat stat_buff;
	int rc = stat(fileName, &stat_buff);
	return (rc == 0) ? stat_buff.st_size : -1;
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
The second argument has all the directions that were taken to reach upto this node. Directions start from MSB. 
0 means left, 1 means right
*/
void traverseTree(Node* current, unsigned int directions, int depth)
{
	//cout << "depth : " << depth << ", directions : " << int(directions) << endl;
	if(current->character != 0)
	{
		//We reached a leaf node, we have encoding for a character here
		/*
		bitset<16> x(directions);
		//cout << "directions " << int(directions) << endl;
		cout << current->character << " : " << current->frequency << " : " << depth << " : " << x << endl;;
		for(int i=depth-1; i>=0; i--)
		{
			cout<<x[i];
		}
		cout << endl;
		//<< x << endl;
		*/
		CharacterEncodingDictionary[current->character].encoding = directions;
		CharacterEncodingDictionary[current->character].depth = depth;
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
	//char document[] = "Hello, this is a sample string for which I will try to make a huffman tree. Okay now it seems that I have a basic program to test the compression. Lets add a bigger sentence and see how it peforms. Yes, this sentence that I am typing is to be used for checking the compression achieved";
	char* document;
	char sourceFileName[] = "test.txt";
	int inputFileSize = getFileSize(sourceFileName);
	if(inputFileSize > 0)
	{
		cout << "The size of file is " << inputFileSize << endl;
	}
	document = new char[inputFileSize];
	fstream infile;
	infile.open(sourceFileName, ios::in | ios::binary);
	infile.read(document, inputFileSize);
	infile.close();
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
	
	for(int i=0; i<256; i++)
	{
		if(CharacterEncodingDictionary[i].depth != 0)
		{
			unsigned char directions = CharacterEncodingDictionary[i].encoding;
			unsigned char depth = CharacterEncodingDictionary[i].depth; 
			bitset<8*sizeof(int)> x(directions);
			cout << char(i) << " : " << int(depth) << " : " ;//<< x << endl;;
			for(int i=depth-1; i>=0; i--)
			{
				cout<<x[i];
			}
			cout << endl;
		}
	}
	
	int sum = 0;
	for(int i=0; i<256; i++)
	{
		sum += frequencies[i] * CharacterEncodingDictionary[i].depth;
	}
	cout << "The predicted bit length is " << sum << endl;
	
	int index = 0;
	//char encoded[512] = "";
	char *encoded;
	encoded = new char[sum];
	int bitIndex = 0;
	int byteIndex = 0;
	int bitInByte = 0;
	
	//Here we calculate the number of bits to be used for encoded data
	
	while(index < strlen(document))
	{
		char tmp = document[index++];
		unsigned int encoding = CharacterEncodingDictionary[tmp].encoding;;
		unsigned char depth = CharacterEncodingDictionary[tmp].depth;
		int internalIndex = depth-1;
		while(internalIndex>=0)
		{
			//Set/Reset the bit at bitIndex
			if(((1 << internalIndex) & encoding))
			{	
				//Encoding of this bit is 1
				encoded[byteIndex] |= (1 << bitInByte);
			}
			else
			{
				encoded[byteIndex] &= (~(1 << bitInByte));
			}
			internalIndex--;
			bitIndex++;
			bitInByte++;
			if(bitInByte == 8)
			{
				byteIndex++;
				bitInByte = 0;
			}
		}
		
	}
	cout << "Total bytes and bits used : " << byteIndex << ", " << bitIndex << endl;
	cout << "Original bytes " << strlen(document) << endl;
	
	for(int i=0 ; i<= 10; i++)
	{
		cout << int(encoded[i]) << " - ";
	}
	cout << endl;
	//char decoded[512] = "";
	char *decoded = new char[inputFileSize];
	int decodingBit = 0;
	int byteInDecoded = 0;
	Node *currentNode = root;
	while(decodingBit <= bitIndex)
	{
		int decodingByte = decodingBit/8;
		int bitInByte = decodingBit % 8;
		decodingBit++;
		if(encoded[decodingByte] & (1 << (bitInByte)))
		{
			currentNode = currentNode->right;
		}
		else
		{
			currentNode = currentNode -> left;
		}
		if(currentNode->character != 0)
		{
			decoded[byteInDecoded++] = currentNode->character;
			currentNode = root;
		}
		
	}
	cout << "Original text " << document << endl;
	cout << "Decoded text " << endl;
	cout << decoded << endl;
	//traverseTree(root->left, string("0"));
	//traverseTree(root->right, string("1"));
	return 0;


}
