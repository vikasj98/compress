#include <iostream>
#include <vector>
#include <sys/stat.h>
#include <fstream>
#include <algorithm>
#include <bitset>

using namespace std;

const int MAX_NUMBER_OF_CHARACTER_VALUES = 256;

struct FileInfo
{
    int size;
    char* data;
};

struct Node
{
	char character;
	Node* left;
	Node* right;
	int frequency;
};

struct CharacterAndFrequency
{
    char character;
    int frequency;
};

struct RootAndFrequency
{
    Node* root;
    vector<CharacterAndFrequency> histogram;
};

struct CharacterEncoding
{
	unsigned int encoding;	//Encoding of the character
	char depth;			//Number of bits (first n bits from LSB. First bit is the MSB among these bits)
};

FileInfo readFile(char* fileName)
{
    FileInfo inputfileInfo = {};
    struct stat stat_buff;
	int rc = stat(fileName, &stat_buff);
    if(rc == 0)
    {
        //successful 
        inputfileInfo.size = stat_buff.st_size;
    }
    else
    {
        //Failed
        inputfileInfo.size = 0;
    }
    inputfileInfo.data = new char[inputfileInfo.size];
    fstream infile;
	infile.open(fileName, ios::in | ios::binary);
	infile.read(inputfileInfo.data, inputfileInfo.size);
	infile.close();
    return inputfileInfo;

}

bool comparator(Node* struct1, Node* struct2)
{
	return (struct1->frequency < struct2->frequency);
}

RootAndFrequency generateTree(FileInfo inputdata)
{
    RootAndFrequency huffmanInfo = {};
    huffmanInfo.root = 0;
    //vector<CharacterAndFrequency> histogram;
    huffmanInfo.histogram.resize(MAX_NUMBER_OF_CHARACTER_VALUES); //= new vector<CharacterAndFrequency>(MAX_NUMBER_OF_CHARACTER_VALUES, 0);

    for(int i=0; i<MAX_NUMBER_OF_CHARACTER_VALUES; i++)
    {
        huffmanInfo.histogram[i].frequency = 0;
        huffmanInfo.histogram[i].character = i;
    }
    for(int i=0; i<inputdata.size; i++)
	{
		huffmanInfo.histogram[static_cast<unsigned char>(inputdata.data[i])].frequency++;
	}

    vector<Node*> nodeVect;
	for(int i=0; i<huffmanInfo.histogram.size(); i++)
	{
		Node *tmp = new Node();
		tmp->character = huffmanInfo.histogram[i].character;
		tmp->frequency = huffmanInfo.histogram[i].frequency;
		nodeVect.push_back(tmp);
	} 

    //cout << "Before Sorting " << endl;
	for(int i=0; i<nodeVect.size(); i++)
	{
		//cout << nodeVect[i]->character << " : " << nodeVect[i]->frequency << endl;
	} 
	sort(nodeVect.begin(), nodeVect.end(), comparator);
	//cout << "After sorting "<< endl;
	for(int i=0; i<nodeVect.size(); i++)
	{
		//cout << nodeVect[i]->character << " : " << nodeVect[i]->frequency << endl;
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
    huffmanInfo.root = root;
    return huffmanInfo;
}


/*
The second argument has all the directions that were taken to reach upto this node. Directions start from MSB. 
0 means left, 1 means right
*/
void traverseTree(Node* current, unsigned int directions, int depth, 
                    CharacterEncoding* CharacterEncodingDictionary)
{
    //cout << "Inside traverseTree on " << current << " depth : " << depth << endl;
	if(current->character != 0)
	{
        //cout << "Reached " << (int)(unsigned char)(current->character) << endl;
		CharacterEncodingDictionary[(unsigned char)current->character].encoding = directions;
		CharacterEncodingDictionary[(unsigned char)current->character].depth = depth;
		return;
	}
	if(current->left != 0)
	{
		traverseTree(current->left, directions << 1, depth+1, CharacterEncodingDictionary);
	}
	if(current->right != 0)
	{
		traverseTree(current->right, ((directions << 1) | 1), depth+1, CharacterEncodingDictionary);
	}
}

void generateEncodingDictionary(Node* root, CharacterEncoding* CharacterEncodingDictionary)
{
    traverseTree(root->left, 0, 1, CharacterEncodingDictionary);
    traverseTree(root->right, 1, 1, CharacterEncodingDictionary);
    //After this the encoding dictionary will have been generated
    //cout << "Done traversing everything " << endl;
    for(int i=0; i<256; i++)
	{
        //cout << "Traversing " << i << endl;
		if(CharacterEncodingDictionary[i].depth != 0)
		{
			unsigned char directions = CharacterEncodingDictionary[i].encoding;
			unsigned char depth = CharacterEncodingDictionary[i].depth; 
			bitset<8*sizeof(int)> x(directions);
			//cout << char(i) << " : " << int(depth) << " : " ;//<< x << endl;;
			for(int i=depth-1; i>=0; i--)
			{
				//cout<<x[i];
			}
			//cout << endl;
		}
	}
}

bool printCompressionInfo(vector<CharacterAndFrequency> histogram, 
        CharacterEncoding* characterEncodingDictionary, int inputSize)
{
    cout << "Dummy compression information" << endl;
    int totalBits = 0;
    for(int i=0; i<histogram.size(); i++)
    {
        totalBits += histogram[i].frequency * characterEncodingDictionary[i].depth;
    }
    int totalBytes = (totalBits / 8) + 1;
    cout << "Bytes needed after compression " << totalBytes << endl;
    cout << "That's " << (float(totalBytes) / inputSize) * 100 << " % of original size " << endl;
    cout << "Do you want to proceed " << endl;
    char response;
    cin >> response;
    if (response == 'y' || response == 'Y')
    {
        return true;
    }
    else
    {
        return false;
    }
}

char* compress(FileInfo inputData, CharacterEncoding* CharacterEncodingDictionary,
                int &outputDataLength)
{
    cout << "Compressing the data " << endl;
    outputDataLength = 0;
    char* outputData = NULL;
    return outputData;
}

void writeOutFile(Node* root, char* outputData, int outputDataLength, char* outputFileName)
{
    cout << "Writing output to a file" << endl;
}

void clearTree(Node *root)
{
    cout << "Clearing the tree" << endl;
}



int main(int argc, char** argv)
{
    if(argc != 2)
    {
        cout << "Usage : compress input_file_name" << endl;
        return -1;
    }
    FileInfo inputData = {};

    //Read the input file and store it in a buffer. The pointer to this buffer is returned 
    //along with the number of bytes read.
    inputData = readFile(argv[1]);
    cout << "Read " << inputData.size << " bytes of data" << endl;
    RootAndFrequency huffmanInfo = {};
    //After this we will have generated a huffman tree whose root will be available. 
    //histogram of frequencies is also returned.
    huffmanInfo = generateTree(inputData);
    int sum = 0;
    for(int i=0; i<MAX_NUMBER_OF_CHARACTER_VALUES; i++)
    {
        //cout << i << " : " << huffmanInfo.histogram[i].frequency << endl;
        sum += huffmanInfo.histogram[i].frequency;
    }
    cout << "Total of frequencies is " << sum << endl;
    CharacterEncoding characterEncodingDictionary[MAX_NUMBER_OF_CHARACTER_VALUES] = {};
    //Traverse the tree and populate the dictionary for character encoding.
    generateEncodingDictionary(huffmanInfo.root, characterEncodingDictionary);
    bool proceed = printCompressionInfo(huffmanInfo.histogram, 
                                    characterEncodingDictionary, inputData.size);

    if(proceed) 
    {
        int outputDataLength = 0;
        char *outputData = compress(inputData, characterEncodingDictionary, outputDataLength);
        //Write the compressed file to output
        writeOutFile(huffmanInfo.root, outputData, outputDataLength, argv[1]);
        delete [] outputData;
    }
    else
    {
        cout << "Not proceeding with compression" << endl;
    }
    delete [] inputData.data;
    clearTree(huffmanInfo.root);
    return 0;
}