////////////////////////////////////////////////////////////////////////////////////////////////
// File :			DTSLab6.cpp
// Author :			Rodney Stoeffler
// Last Modified :	10.12.10
// Purpose :		Driver Program for the BST class.
// Notes :			Property of Full Sail University, Data Structures Course.
//					DON'T CHANGE THIS FILE. change your code if something is not working
//					Program Output should match the Lab Document
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
// LEAK DETECTION
////////////////////////////////////////////////////////////////////////////////////////////////
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES
////////////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
using namespace std;
#include "BST.h"   // Include for provided BST header.

////////////////////////////////////////////////////////////////////////////////////////////////
// Function :	main
// Parameters : argc - the number of command line arguments
//				argv - the array of command line arguments
// Return :		int - 0 for success
////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char ** argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
// this function call will set a breakpoint at the location of a leaked block
// set the parameter to the identifier for a leaked block
//	_CrtSetBreakAlloc(187);

////////////////////////////////////////////////////////////////////////////////////////////////
// LAB 6
////////////////////////////////////////////////////////////////////////////////////////////////
	cout << "**********************************************************************\n";
	cout << "**                              LAB 6:                              **\n";
	cout << "**********************************************************************\n\n";

	////////////////////////////////////////////////////////////////////////////////////////////
	// Declarations
	////////////////////////////////////////////////////////////////////////////////////////////
	int vals[] =   {50, 70, 30, 20, 40, 80, 60, 55, 75, 35, 25, 45, 85, 65, 77};
	char cvals[] = {'g','k','c','a','e','n','i','h','l','d','b','f','o','j','m'};
	BST<int> tree;
	BST<int> tree2;
	BST<char> ctree;
	BST<char> ctree2;

	////////////////////////////////////////////////////////////////////////////////////////////
	// TEST 1 - add a few nodes, print, and clear the tree.
	////////////////////////////////////////////////////////////////////////////////////////////
	cout << "*** TEST 1 ***\n";
	for(int i = 0; i< 15; ++i)
		tree.insert(vals[i]);
	tree.printInOrder();
	tree.clear();

	////////////////////////////////////////////////////////////////////////////////////////////
	// TEST 2 - add a few nodes, do searching on the items, and clear the tree.
	////////////////////////////////////////////////////////////////////////////////////////////
	cout << "\n\n*** TEST 2 ***\n";
	tree.clear();
	for(int i = 0; i< 15; ++i)
	{
		if(vals[i] != 25 && vals[i] != 65 && vals[i] != 75)
			tree.insert(vals[i]);
	}
	
	tree.printInOrder();
	for(int i = 25; i <= 85; i += 5)
		if(tree.find(i))
			cout << "The value " << i << " was found!\n";
		else
			cout << "The value " << i << " was not found..\n";
	tree.clear();

	////////////////////////////////////////////////////////////////////////////////////////////
	// TEST 3 - add a few nodes, remove some, and clear the tree.
	//////////////////////////////////////////////////////////////////////////////////////////// 
	cout << "\n\n*** TEST 3 ***\n";

	for(int i = 0; i< 15; ++i)
		tree.insert(vals[i]);

	tree.printInOrder();
	tree.findAndRemove(5);
	tree.printInOrder();
	tree.findAndRemove(33);
	tree.printInOrder();
	tree.findAndRemove(71);
	tree.printInOrder();
	tree.findAndRemove(20);
	tree.printInOrder();
	tree.findAndRemove(70);
	tree.printInOrder();
	tree.findAndRemove(55);
	tree.printInOrder();
	tree.findAndRemove(77);
	tree.printInOrder();
	tree.findAndRemove(50);
	tree.printInOrder();
	tree.findAndRemove(30);
	tree.printInOrder();
	tree.findAndRemove(25);
	tree.printInOrder();
	tree.findAndRemove(35);
	tree.printInOrder();
	tree.findAndRemove(65);
	tree.printInOrder();
	tree.findAndRemove(85);
	tree.printInOrder();
	tree.findAndRemove(80);
	tree.printInOrder();
	tree.findAndRemove(40);
	tree.printInOrder();
	tree.findAndRemove(45);
	tree.printInOrder();
	tree.findAndRemove(60);
	tree.printInOrder();
	tree.findAndRemove(75);
	tree.printInOrder();
	tree.findAndRemove(0);
	tree.printInOrder();

	////////////////////////////////////////////////////////////////////////////////////////////
	// TEST 4
	////////////////////////////////////////////////////////////////////////////////////////////
	cout << "\n\n*** TEST 4 ***\n";
	for(int i = 0; i< 15; ++i)
		ctree.insert(cvals[i]);


	cout << "4a:\n";
	ctree.printInOrder();

	ctree2 = ctree;
	ctree.clear();

	cout << "4b:\n";
	ctree2.printInOrder();

	BST<char>ctree3(ctree2);
	ctree2.clear();

	cout << "4c:\n";
	ctree3.printInOrder();

	ctree = ctree2;
	cout << "4d:\n";
	ctree2.printInOrder();

	BST<char>ctree4(ctree);
	cout << "4e:\n";
	ctree4.printInOrder();

	////////////////////////////////////////////////////////////////////////////////////////////
	// Complete
	////////////////////////////////////////////////////////////////////////////////////////////
	system("pause");
	return 0;
}