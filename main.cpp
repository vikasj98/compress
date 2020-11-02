#include <iostream>
#include <vector>
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
    inputfileInfo.size = 0;
    inputfileInfo.data = NULL;
    return inputfileInfo;

}

RootAndFrequency generateTree(FileInfo inputdata)
{
    RootAndFrequency huffmanInfo = {};
    huffmanInfo.root = 0;
    vector<CharacterAndFrequency> histogram;
    huffmanInfo.histogram = histogram;
    return huffmanInfo;
}

void generateEncodingDictionary(Node* root, CharacterEncoding* CharacterEncodingDictionary)
{
    //This will generate the encoding dictionary to be used.
}

bool printCompressionInfo(vector<CharacterAndFrequency> hitogram, 
        CharacterEncoding* CharacterEncodingDictionary)
{
    cout << "Dummy compression information" << endl;
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
    if(argc != 1)
    {
        cout << "Usage : compress input_file_name" << endl;
    }
    FileInfo inputData = {};

    //Read the input file and store it in a buffer. The pointer to this buffer is returned.
    inputData = readFile(argv[1]);
    RootAndFrequency huffmanInfo = {};
    //After this we will have generated a huffman tree whose root will be available. 
    //histogram of frequencies is also returned.
    huffmanInfo = generateTree(inputData);
    CharacterEncoding characterEncodingDictionary[MAX_NUMBER_OF_CHARACTER_VALUES] = {};
    //Traverse the tree and populate the dictionary for character encoding.
    generateEncodingDictionary(huffmanInfo.root, characterEncodingDictionary);
    bool proceed = printCompressionInfo(huffmanInfo.histogram, 
                                    characterEncodingDictionary);

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