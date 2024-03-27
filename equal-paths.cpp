#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <algorithm>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int findDepth(Node* root)
{
    if(!root) return 0;
    int leftDepth = findDepth(root -> left);
    int rightDepth = findDepth(root -> right);
    return 1 + max(leftDepth, rightDepth);
}

bool equalPathsHelper(Node* root, int depth, int target)
{
    if(!root) return true;
    if(!root -> left && !root -> right) return depth == target;
    return equalPathsHelper(root -> left, depth + 1, target) && equalPathsHelper(root -> right, depth + 1, target);
}

bool equalPaths(Node * root)
{
    if (root == NULL) return true;
    int target = findDepth(root);
    return equalPathsHelper(root, 1, target);
}

