#include <iostream>
#include <map>
#include "bst.h"
#include "avlbst.h"

using namespace std;


int main(int argc, char *argv[])
{
  /*
    // Binary Search Tree tests
    BinarySearchTree<char,int> bt;
    bt.insert(std::make_pair('d', 4));
    bt.insert(std::make_pair('b', 2));
    bt.insert(std::make_pair('a', 3));
    bt.insert(std::make_pair('c',1));
    bt.insert(std::make_pair('e',5));
    bt.print();
	  bt.remove('b');
    bt.print();
    cout << "Binary Search Tree contents:" << endl;
    for(BinarySearchTree<char,int>::iterator it = bt.begin(); it != bt.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    if(bt.find('b') != bt.end()) {
        cout << "Found b" << endl;
    }
    else {
        cout << "Did not find b" << endl;
    }
    cout << "Erasing b" << endl;
    // bt.remove('b');

    // AVL Tree Tests
    AVLTree<int,int> at;
    at.insert(std::make_pair(2,9));
    at.print();
    at.insert(std::make_pair(1,8));
    at.print();
    at.insert(std::make_pair(0,159));
    at.print();

    cout << "\nAVLTree contents:" << endl;
    for(AVLTree<int,int>::iterator it = at.begin(); it != at.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    at.print();
    if(at.find('b') != at.end()) {
        cout << "Found b" << endl;
    }
    else {
        cout << "Did not find b" << endl;
    }
    cout << "Erasing b" << endl;
    //at.remove('b');*/

    AVLTree<uint16_t, uint16_t> testTree;

	  testTree.insert(std::make_pair(2, 9));
    testTree.print();
	  testTree.insert(std::make_pair(1, 8));
    testTree.print();
	  testTree.insert(std::make_pair(0, 159));
    testTree.print();

    return 0;
}
