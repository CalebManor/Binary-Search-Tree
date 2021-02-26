/*****************************************************************************
Name:  WordFrequency.cpp
Des:
This program will take an input text file and count word frequencies.
After reading and counting the words,  the program will prompt to see if
the user wishes to trim the list.  Finally, after trimming the list,
the words and their frequencies will be written to a textfile called
<filename>.frq

Author: Coach Rags
******************************************************************************/

//Include headers needed to write app
#include <fstream>
#include <cstring>
#include <iostream>
#include <vld.h>
#include "BST.h"

//Using the standard namespace
using namespace std;


//Global variable used to store the name of the input text file
const int SIZEOFSOURCEFILESTRING = 80;


/******************************************************************************
Function Name:  AllCaps
Des:  The function will take a string (called data) and will convert it to
all uppercase letters.
******************************************************************************/
void AllCaps(char* data)
{
	for (size_t i = 0; i < strlen(data); i++)
		data[i] = toupper(data[i]);
}

/******************************************************************************
Function Name: GetWords
Des: This function will take an input line, parse out each word in the line
and then update the word list accordingly;  either adding the word to the
list or simple updating the frequency count of that word.
******************************************************************************/
void GetWords(char* line, BST & WordList)
{
	//Constant value for a null character at the end of a C-style string
	const char NULLCHARACTER = '\0';

	int i = 0;
	BSTtype word;
	int j;

	//Loop until you get to the end of the line, a null character is found at
	//the end of the line
	while (line[i] != NULLCHARACTER)
	{
		//skip over non-alphas, terminate on alpha or null
		while ((!isalpha(line[i])) && (line[i] != NULLCHARACTER))
			i++;

		//store word, make sure not at end of line
		if (isalpha(line[i]))
		{
			j = 0;
			while (isalpha(line[i]))
				word[j++] = line[i++];
			word[j] = NULLCHARACTER;

			//capitalize each word, case makes no difference with words		
			AllCaps(word);


			//update word list using a BST object
			if (!WordList.BSTexists(word))
				WordList.BSTadd(word);
			else
				WordList.BSTinc(word);
		}
	}
}

/*****************************************************************************
Function Name: CountWords
Des:  This function will read the file and count word frequencies and build
a wordlist using the bst(binary search tree) class.
******************************************************************************/
void CountWords(BST & WordList, ifstream & fin)
{
	//Max character size of standard line of text
	const int MAXLENGTHOFLINE = 600;
	//Allocate storage for max length plus a slot for the null character
	char line[MAXLENGTHOFLINE];

	int linecount = 0;

	//Read the file one line at a time and parse the words out of the line
	while (fin.peek() != EOF)
	{
		linecount++;
		//Getline reads Max-1 chars at most so adjust accordingly
		fin.getline(line, MAXLENGTHOFLINE);
		//This function will also store the words in the binary search tree
		GetWords(line, WordList);
	}
}

/*****************************************************************************
Function Name: TrimTree
Des:  This function will trim the tree according to the users wishes
*****************************************************************************/
void TrimTree(BST & WordList)
{
	const char SIZEOFOPTIONSTRING = 2;
	const char MAXWORDSIZE = 19;

	char option[SIZEOFOPTIONSTRING];
	BSTtype word;

	cout << "Enter letter to return list of words which start with that letter"
		<< endl;
	cout << "Enter <cr> to quit trimming tree" << endl << endl;
	cin.getline(option, SIZEOFOPTIONSTRING);

	//If user hit only the enter key the string length will be zero
	while (strlen(option) > 0)
	{
		option[0] = toupper(option[0]);
		if (isalpha(option[0]))
		{
			cout << "All words found beginning with letter " << option[0]
				<< endl << endl;

			//Print out words which begin with a certain letter
			WordList.BSTprint(option[0], cout);
			cout << endl << endl;

			//Get word to delete from user
			cout << "Enter word to delete (<cr> to quit): ";
			cin.getline(word, MAXWORDSIZE + 1);
			AllCaps(word);

			//Try to delete word if user entered a word to delete
			while (strlen(word) > 0)
			{
				if (WordList.BSTdelete(word))
					cout << word << " DELETED" << endl;
				else
					cout << word << " NOT FOUND" << endl;

				cout << "Enter word to delete (<cr> to quit): ";
				cin.getline(word, MAXWORDSIZE + 1);
				AllCaps(word);
			}

			//Output new list without the deleted word
			cout << endl << "List with words beginning with " << option[0]
				<< endl << endl;
			WordList.BSTprint(option[0], cout);
			cout << endl << endl;
		}

		cout << "Enter letter to return list of words which start with that letter"
			<< endl;
		cout << "Enter <cr> to quit trimming tree" << endl << endl;
		cin.getline(option, SIZEOFOPTIONSTRING);
	}
}
/*****************************************************************************
Function Name: OpenOutfile
Des:  This will create a filename for the output file of frequencies and
then open this file.
*****************************************************************************/
void OpenOutfile(ofstream & fout, char * source)
{
	const char MAXFILENAMELENGTH = 80;
	//Allocate extra slot for null character
	char dest[MAXFILENAMELENGTH + 1];
	int i = 0;

	while ((source[i] != '.') && (source[i] != '\0'))
	{
		dest[i] = source[i];
		i++;
	}
	dest[i] = '\0';
	strcat_s(dest, ".frq");
	fout.open(dest);

	cout << "Output found in " << dest << endl;
}

/*****************************************************************************
Function Name: WriteToFile
Des:  This function will open the output file and then print the word list
to this file.
*****************************************************************************/
void WritetoFile(BST & WordList, ofstream & fout, char * source) {

	OpenOutfile(fout, source);
	WordList.BSTprint(fout);
	fout.close();
}

/*****************************************************************************
Function Name: main
Des:  Main will obtain the name of the file from which words and their
frequencies in the file will be counted.  If the source exists main will
execute three main functions to 1)Get the words and their frequencies
2) Trim the wordlist if the user desires and 3) Write the list to file.

If the source file does not exist the program will terminate.
*****************************************************************************/
void main()
{
	BST WordList;
	ifstream fin;
	ofstream fout;
	//Allocate storage for max length plus a slot for the null character
	char source[SIZEOFSOURCEFILESTRING + 1];

	cout << "Name of text file to count word freq: ";
	cin.getline(source, SIZEOFSOURCEFILESTRING + 1);
	fin.open(source); //make sure file exists
					  //int treeHeight;
	if (fin)
	{
		CountWords(WordList, fin);
		cout << "Node Count: " << WordList.getBSTNodeCount() << endl;
		cout << "Tree height after build: " << WordList.getBSTHeight() << endl;
		TrimTree(WordList);
		WritetoFile(WordList, fout, source);
	}
	else
		cout << "File does not exist..program terminated" << endl;
	fin.close();
	fout.close();

	//cin.get();
}