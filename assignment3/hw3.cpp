/**
 *
 * Alp Eren Gençoğlu - 150170019
 * BLG 335E - Analysis Of Algorithms
 * Homework 3 - 17.12.2019
 *
 */

#include <iostream>
#include <vector>
#include "hw3.h"

using namespace std;

enum Color {RED, BLACK}; 

// Order Statistic Tree node
class node {
  public:
	node* left;
	node* right;
	node* parent;
	
	int ord;	// node's place in the tree
	int size;	// package data
	bool color;	// red or black
	
};

// Order Statistic Tree
class OST {	
	node* root;
  public:
  	OST() : root(NULL) {}	// constructor
  	
  	node* new_node(int data, int ord);	// returns a node struct according to the given data
	
	void insert(int size, int ord);		// creates new node and calls insert_node
	void insert_node(node* node_new);	// inserts node as in BST. then calls insert_fix
	void insert_fix(node* np);			// fix insertion to not violate RBT rules.
	
	void rotate_right(node* np);
	void rotate_left(node* np);
	
	void del(node* to_be_deleted);					// delete node as in BST
	void del_fix(node* np);				// fix deletion to not violate RBT rules
	
	node* min_node(node* np);
	void transplant(node* u, node* v);
	
	node* get_nth_smallest(node* root, int n);	//
	int tree_size(node* root);
	
	node* get_root(){ return this->root; }
	bool is_empty(){ return this->root == NULL; }

	int count_red(node* root);
	int count_black(node* root);
	
	// Traverse functions
	void inorder_traverser(node* root){
		if(root == NULL) return;
		
		inorder_traverser(root->left);
		string color = root->color ? "Black" : "Red";
		cout << root->size <<"|" << root->ord << "|" << color << ", ";
		inorder_traverser(root->right);
	}
	void inorder_print(){
		inorder_traverser(get_root());
		cout << endl;
	}
	void preorder_traverser(node* root){
		if(root == NULL) return;
		
		string color = root->color ? "Black" : "Red";
		cout << root->size <<"|" << root->ord << "|" << color << ", ";
		preorder_traverser(root->left);
		preorder_traverser(root->right);
	}
	void preorder_print(){
		preorder_traverser(get_root());
		cout << endl;
	}
	
	
};

int OST::count_red(node* root){
	int number_of_reds = 0;
	if (root == NULL) return 0;
	number_of_reds += count_red(root->left);
	number_of_reds += count_red(root->right);
	if (root->color == RED) number_of_reds++;
	return number_of_reds;
}
int OST::count_black(node* root){
	int number_of_blacks = 0;
	if (root == NULL) return 0;
	number_of_blacks += count_black(root->left);
	number_of_blacks += count_black(root->right);
	if (root->color == BLACK) number_of_blacks++;
	return number_of_blacks;
}

node* OST::new_node(int size, int ord) {
	node* new_node = new node;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->parent = NULL;
	new_node->color = RED;
	new_node->size = size;
	new_node->ord = ord;			// !!!!!
	return new_node;
}

void OST::insert(int size, int ord){
	node* new_node = this->new_node(size, ord);
	insert_node(new_node);
}

// applies normal Binary Search Tree insertion first. Then calls insertfix method.
void OST::insert_node(node* new_node){
    node* curr_node = this->root; 
    node* prev_node = NULL; 
  
    while (curr_node != NULL) { 
        prev_node = curr_node; 
        if (new_node->size < curr_node->size) 
            curr_node = curr_node->left; 
        else
            curr_node = curr_node->right; 
    } 

    new_node->parent = prev_node;
    
    if (prev_node == NULL) 	// if root is null (empty tree)
        root = new_node; 
    else if (new_node->size < prev_node->size) 
        prev_node->left = new_node; 
    else
        prev_node->right = new_node; 

    insert_fix(new_node);
}

void OST::insert_fix(node* np){
	// if new node is the root, change color to black and return.
	if(np->parent == NULL){
		np->color = BLACK;
		return;
	}
	
	//if(np->parent->parent == NULL) return;
	
	node* uncle;
	node* parent;
	node* grandparent;
	
	while(np->parent!=NULL && np->parent->color == RED && np->color != BLACK){
		parent = np->parent;
		grandparent = np->parent->parent;
		if(parent == grandparent->right){
			uncle = grandparent->left;
			if(uncle != NULL && uncle->color == RED){	// case R: color of the uncle is red, recolor G, P, U.	
				uncle->color = BLACK;
				parent->color = BLACK;
				grandparent->color = RED;
				np = grandparent;		// make new node = grandparent to continue violation checking
			}
			else{
				if(np == parent->left) {	// case T: G, P and N form a triangle, rotate to obtain case L.
					np = parent;
					rotate_right(np);
				}
				// case L: G, P, and N form a line, rotate grandparent and recolor P and G
				parent = np->parent;	// THIS ONE GUY MADE ME WASTE SO MUCH TIME
				parent->color = BLACK;
				grandparent->color = RED;
				rotate_left(grandparent);
			}
		}
		
		else{	// [if(parent == grandparent->left)] -> same operations as above, only for other way around.
			uncle = grandparent->right;
			if(uncle != NULL && uncle->color == RED){	// case R: color of the uncle is red, recolor G, P, U.
				uncle->color = BLACK;
				parent->color = BLACK;
				grandparent->color = RED;
				np = grandparent;		// make new node = grandparent to continue violation checking
			}
			else{
				if(np == parent->right) {	// case T: G, P and N form a triangle, rotate to obtain case L.
					np = parent;
					rotate_left(np);
				}
				// case L: G, P, and N form a line, rotate grandparent and recolor P and G
				parent = np->parent;		// THIS ONE GUY MADE ME WASTE SO MUCH TIME
				parent->color = BLACK;
				grandparent->color = RED;
				rotate_right(grandparent);
			}
		}
		
		if(np == this->root)	// loop termination condition
			break;
	}
	
	this->root->color = BLACK;
}

void OST::del(node* to_be_deleted){
	/*
	cout << key << endl;
	
	node* curr_node = this->root;
	node* to_be_deleted = NULL;
	*/
	node* y;
	node* x;
	
	/*
	while(curr_node != NULL){
		if(curr_node->size == key){
			to_be_deleted = curr_node;
		}
		
		if(curr_node->size <= key)
			curr_node = curr_node->right;
		else
			curr_node = curr_node->left;
	}
	
	if (to_be_deleted == NULL)	// node to be deleted could not be found.
		return;
	*/	
	
	y = to_be_deleted;
	bool y_original_color = y->color;
	
	if (to_be_deleted->left == NULL){
		x = to_be_deleted->right;
		transplant(to_be_deleted, to_be_deleted->right);
	}
	else if (to_be_deleted->right == NULL){
		x = to_be_deleted->left;
		transplant(to_be_deleted, to_be_deleted->left);
	}
	else {
		y = min_node(to_be_deleted->right);
		//cout << "y: " << y->size << endl;
		y_original_color = y->color;
		x = y->right;
		
		if (x != NULL && y->parent == to_be_deleted) {
			x->parent = y;
		}
		else {
			transplant(y, y->right);
			y->right = to_be_deleted->right;
			if (y->right != NULL)
				y->right->parent = y;
		}
			
		transplant(to_be_deleted, y);
		y->left = to_be_deleted->left;
		y->left->parent = y;
		y->color = to_be_deleted->color;		
	}
	
	delete to_be_deleted;
	if (x != NULL && y_original_color == BLACK){
		del_fix(x);
	}
}

node* OST::min_node(node* np) {
	if (np == NULL) return NULL;
	
	while (np->left != NULL)
		np = np->left;
	
	return np;
}

void OST::transplant(node* u, node* v){
	if (u->parent == NULL)
		this->root = v;
	else if (u == u->parent->left)
		u->parent->left	= v;
	else
		u->parent->right = v;
	
	// if v is NULL, this means it is a leaf (NIL). 
	// so do not try to access its parent in such case
	if (v != NULL)	
		v->parent = u->parent;	
}

void OST::del_fix(node* np){
	node* sibling;
	while(np != this->root && np->color == BLACK){
		if(np == np->parent->left){
			sibling = np->parent->right;
			if (sibling == NULL) return;	// prevent segmentation fault
			
			if (sibling->color == RED){
				sibling->color = BLACK;
				np->parent->color = RED;
				rotate_left(np->parent);
				sibling = np->parent->right;
			}
			
			if (sibling->left->color == BLACK && sibling->right->color == BLACK){
				sibling->color = RED;
				np = np->parent;
			}
			
			else{
				if (sibling->right->color == BLACK) {
					sibling->left->color = BLACK;
					sibling->color = RED;
					rotate_right(sibling);
					sibling = np->parent->right;
				}
				sibling->color = np->parent->color;
				np->parent->color = BLACK;
				sibling->right->color = BLACK;
				rotate_left(np->parent);
				np = this->root;
			}
		}
		
		else {	// [if (np == np->parent->right)]
			sibling = np->parent->left;
			if (sibling == NULL) return;	// prevent segmentation fault
			
			if (sibling->color == RED){
				sibling->color = BLACK;
				np->parent->color = RED;
				rotate_right(np->parent);
				sibling = np->parent->left;
			}
			
			if (sibling->left->color == BLACK && sibling->right->color == BLACK){
				sibling->color = RED;
				np = np->parent;
			}
			
			else{
				if (sibling->left->color == BLACK) {
					sibling->right->color = BLACK;
					sibling->color = RED;
					rotate_left(sibling);
					sibling = np->parent->left;
				}
				sibling->color = np->parent->color;
				np->parent->color = BLACK;
				sibling->left->color = BLACK;
				rotate_right(np->parent);
				np = this->root;
			}
		}
	}
	np->color = BLACK;
}

void OST::rotate_left(node* np){
	node* temp = np->right;
	np->right = temp->left;
	
	if(np->right != NULL)
		np->right->parent = np;
	
	temp->parent = np->parent;
	
	if(np->parent == NULL)
		this->root = temp;
	else if(np == np->parent->left)
		np->parent->left = temp;
	else
		np->parent->right = temp;
	
	temp->left = np;
	np->parent = temp;	
}

void OST::rotate_right(node* np){
	node* temp = np->left;
	np->left = temp->right;
	
	if(np->left != NULL)
		np->left->parent = np;
		
	temp->parent = np->parent;
	
	if(np->parent == NULL)
		this->root = temp;
	else if(np == np->parent->left)
		np->parent->left = temp;
	else
		np->parent->right = temp;
		
	temp->right = np;
	np->parent = temp;
}

node* OST::get_nth_smallest(node* root, int n){
	int ord = this->tree_size(root->left) + 1;
	if (n == ord)
		return root;
	else if (n < ord)
		return get_nth_smallest(root->left, n);
	else
		return get_nth_smallest(root->right, n - ord);
}

int OST::tree_size(node* root){
	if (root == NULL) return 0;
	return tree_size(root->left) + tree_size(root->right) + 1;
}

HW3_Result hw3(  int eastWarehousePackageCount,
                 int eastWarehousePackageSizes [],
                 int eastWarehousePackageOrdinals [],
                 int westWarehousePackageCount,
                 int westWarehousePackageSizes [],
                 int westWarehousePackageOrdinals [] ){
	//
	// Create and initialize trees
	//
	OST east_wh;
	OST west_wh;
	for(int i=0; i<eastWarehousePackageCount; i++){
		east_wh.insert(eastWarehousePackageSizes[i], eastWarehousePackageOrdinals[i]);
	}
	for(int i=0; i<westWarehousePackageCount; i++){
		west_wh.insert(westWarehousePackageSizes[i], westWarehousePackageOrdinals[i]);
	}
	//cout << "East:" << endl; east_wh.inorder_print(); cout << endl;
	//cout << "West:" << endl; west_wh.inorder_print(); cout << endl;
	
	
	//
	// Start shipping and continue until one of the warehouses is empty.
	//
	bool direction = true;	// binary flag, denotes the direction of shipment.
	                        // true: e->w, false: w->e.

	bool ship_type = true;	// shipment type flag.
	                        // true: smallest, false: nth smallest
	

	vector<node*> shipment;	// to save all packages to insert into other warehouse
	vector<node*> shipment2;// this is required because receiver warehouse 
	                        // gives the necessary packages before
	                        // accepting incoming shipment
	int min_size;
	int min_ord;
	while( !( east_wh.is_empty() || west_wh.is_empty() ) ){
		
		
		if (direction){ // if east->west
			
			// give shipment to lead courier (east warehouse gives)
			if (ship_type){	// shipment type: smallest
				node* min = east_wh.min_node(east_wh.get_root());
				min_size = min->size;
				min_ord = min->ord;
				shipment.push_back(min);
				east_wh.del(min);
				while(true){
					node* min_other = east_wh.min_node(east_wh.get_root());
					if (min_other != NULL && min_other->size == min_size){
						shipment.push_back(min_other);
						east_wh.del(min_other);
						if (min_other->ord < min_ord)
							min_ord = min_other->ord;
						continue;
					}
					else
						break;
				}
			}
			else {	// shipment type: nth smallest
				node* nth_smallest_node = east_wh.get_nth_smallest(east_wh.get_root(), min_ord);
				shipment.push_back(nth_smallest_node);
				east_wh.del(nth_smallest_node);
				min_ord = nth_smallest_node->ord;
			}
			
			// take shipment from lead courier (west warehouse takes)
			if(west_wh.tree_size(west_wh.get_root()) >= min_ord || east_wh.is_empty()){	// can accept shipment
				for(int i=0; i<int(shipment.size()); i++){
					west_wh.insert(shipment[i]->size, shipment[i]->ord);
				}
				
				// incoming shipment is accepted, so next shipment 
				// will be "nth smallest shipment".
				ship_type = false;	
			} 
			else {		// cannot accept shipment
				// no insert operations, beacuse shipment is not accepted.
				
				// incoming shipment is not accepted, so next shipment 
				// will be "smallest shipment".
				ship_type = true;
			}
			
		}
		
		
		else {			// if west->east
			// give shipment to lead courier (west warehouse gives)
			if (ship_type){	// shipment type: smallest
				node* min = west_wh.min_node(west_wh.get_root());
				min_size = min->size;
				min_ord = min->ord;
				shipment.push_back(min);
				west_wh.del(min);
				while(true){
					node* min_other = west_wh.min_node(west_wh.get_root());
					if (min_other != NULL && min_other->size == min_size){
						shipment.push_back(min_other);
						west_wh.del(min_other);
						if (min_other->ord < min_ord)
							min_ord = min_other->ord;
						continue;
					}
					else
						break;
				}
				
			}
			else {	// shipment type: nth smallest
				node* nth_smallest_node = west_wh.get_nth_smallest(west_wh.get_root(), min_ord);
				shipment.push_back(nth_smallest_node);
				west_wh.del(nth_smallest_node);
				min_ord = nth_smallest_node->ord;
			}
			
			// take shipment from lead courier (east warehouse takes)
			if(east_wh.tree_size(east_wh.get_root()) >= min_ord || west_wh.is_empty()){	// can accept shipment
				for(int i=0; i<int(shipment.size()); i++){
					east_wh.insert(shipment[i]->size, shipment[i]->ord);
				}
				
				// incoming shipment is accepted, so next shipment 
				// will be "nth smallest shipment".
				ship_type = false;	
			} 
			else {		// cannot accept shipment
				// no insert operations, beacuse shipment is not accepted.
				
				// incoming shipment is not accepted, so next shipment 
				// will be "smallest shipment".
				ship_type = true;
			}
			
		}
		
		shipment.clear();	// there may be remaining packages in shipment so clear them.
		direction = !direction;	// change direction in each iteration.
	}
	
	HW3_Result res;
	if (east_wh.is_empty()){
		res.redNodeCount = west_wh.count_red(west_wh.get_root());
		res.blackNodeCount = west_wh.count_black(west_wh.get_root());
		res.packageCount = west_wh.tree_size(west_wh.get_root());
	}
	else {
		res.redNodeCount = east_wh.count_red(east_wh.get_root());
		res.blackNodeCount = east_wh.count_black(east_wh.get_root());
		res.packageCount = east_wh.tree_size(east_wh.get_root());
	}
	
	return res;
}

/*
int main(){

	int westWarehousePackageCount = 7;
	int westWarehousePackageSizes [] =    {27,3 ,4 ,8 ,11,2 ,3 };
	int westWarehousePackageOrdinals [] = {15,8 ,22,6 ,4 ,7 ,10};
	int eastWarehousePackageCount = 9;
	int eastWarehousePackageSizes [] =    {11,18,7 ,2 ,15,16,3 ,12,17};
	int eastWarehousePackageOrdinals [] = {32,5 ,16,6 ,2 ,4 ,13,7 ,13};


	// Os
	int eastWarehousePackageCount = 10;
    int eastWarehousePackageSizes[] = {1,5,34,6,7,23,5,6,34,5};
    int eastWarehousePackageOrdinals[] = {7,43,59,80,925,89,27,45,89,72};
    int westWarehousePackageCount = 10;
    int westWarehousePackageSizes[] = {10,38,51,7,38,47,8,53,2,94};
    int westWarehousePackageOrdinals[] = {5,62,86,73,74,83,68,35,7,95};

	struct HW3_Result h = hw3(eastWarehousePackageCount,
                 			eastWarehousePackageSizes,
                			eastWarehousePackageOrdinals,
                			westWarehousePackageCount,
                			westWarehousePackageSizes,
                			westWarehousePackageOrdinals );
	
	cout << " | total: " << h.packageCount 
		<< " | red: " << h.redNodeCount 
		<< "| black: " << h.blackNodeCount 
		<< " |" << endl;
	
	return 0;
}*/



