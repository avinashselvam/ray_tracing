#ifndef TREEUTILSH
#define TREEUTILSH

#define COUNT 5

void print2DUtil(KDNode *root, int space) 
{ 
    // Base case 
    if (root == NULL) 
        return; 
  
    // Increase distance between levels 
    space += COUNT; 
  
    // Process right child first 
    print2DUtil(root->right, space); 
  
    // Print current node after space 
    // count 
    printf("\n"); 
    for (int i = COUNT; i < space; i++) 
        printf(" "); 
    printf("%lu\n", root->triangles.size()); 
  
    // Process left child 
    print2DUtil(root->left, space); 
} 

void print2D(KDNode *root) 
{ 
   // Pass initial space count as 0 
   print2DUtil(root, 0); 
} 

#endif