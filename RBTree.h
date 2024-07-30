#pragma once

#include <iostream>
#include <vector>
using namespace std;

struct TreeNode {
	int val;
	bool isRed;
	TreeNode* parent;
	TreeNode* left;
	TreeNode* right;

	TreeNode(int val, bool isRed = true) : val(val), isRed(isRed), parent(nullptr), left(nullptr), right(nullptr) {}

	void show() const {
		cout << "TreeNode (val: " << val << " color: ";
		if (isRed) cout << "RED ";
		else cout << "BLACK ";
		cout << "parent: ";
		if (parent) cout << parent->val << ' ';
		else cout << "None ";
		if (left) cout << left->val << ' ';
		else cout << "None ";
		if (right) cout << right->val << " )" << endl;
		else cout << "None )" << endl;
	}
};


class RBTree {
private:
	TreeNode* nil;
	TreeNode* root;

	void leftRotate(TreeNode* x) {
		TreeNode* y = x->right;
		x->right = y->left;
		if (y->left && y->left != nil)
			y->left->parent = x;
		y->parent = x->parent;
		if (!x->parent) root = y;
		else if (x->parent->left == x)
			x->parent->left = y;
		else x->parent->right = y;
		y->left = x;
		x->parent = y;
	}

	void rightRotate(TreeNode* x) {
		TreeNode* y = x->left;
		x->left = y->right;
		if (y->right && y->right != nil)
			y->right->parent = x;
		y->parent = x->parent;
		if (!x->parent) root = y;
		else if (x->parent->left == x)
			x->parent->left = y;
		else x->parent->right = y;
		y->right = x;
		x->parent = y;
	}

	TreeNode* findSuc(TreeNode* cur) {
		if (cur->right && cur->right != nil) {
			cur = cur->right;
			while (cur->left && cur->left != nil)
				cur = cur->left;
			return cur;
		}
	}

	TreeNode* findNode(int val) {
		TreeNode* cur = root;
		while (cur && cur != nil && cur->val != val) {
			if (cur->val < val) cur = cur->right;
			else cur = cur->left;
		}
		return cur;
	}

	void trans(TreeNode* x, TreeNode* y) {
		y->parent = x->parent;
		if (!x->parent) root = y;
		else if (x->parent->left == x)
			x->parent->left = y;
		else x->parent->right = y;
	}

	void fixInsert(TreeNode* cur) {
		// 只用修复父子都红的情况, 而在这种情况下, 爷结点一定存在
		// 因为不存在没有父结点的红结点
		while (cur != root && cur->parent->isRed) {
			if (cur->parent == cur->parent->parent->left) {
				TreeNode* uncle = cur->parent->parent->right;

				// 红叔: 叔父爷换色, 爷当新结点处理
				if (uncle->isRed) {
					cur->parent->isRed = false;
					uncle->isRed = false;
					cur->parent->parent->isRed = true;
					cur = cur->parent->parent;
				}

				// 黑叔: 旋转 + 染色
				else {
					if (cur == cur->parent->right) {
						cur = cur->parent;
						leftRotate(cur);
					}
                    // 此时的 cur->parent 就是原先的子结点
					cur->parent->isRed = false;
					cur->parent->parent->isRed = true;
					rightRotate(cur->parent->parent);
				}
			}
			else {
				TreeNode* uncle = cur->parent->parent->left;

				if (uncle->isRed) {
					cur->parent->isRed = false;
					uncle->isRed = false;
					cur->parent->parent->isRed = true;
					cur = cur->parent->parent;
				}

				else {
					if (cur == cur->parent->left) {
						cur = cur->parent;
						rightRotate(cur);
					}
					cur->parent->isRed = false;
					cur->parent->parent->isRed = true;
					leftRotate(cur->parent->parent);
				}
			}
		}
		root->isRed = false;
	}

	void fix_delete(TreeNode* cur) {
		while (cur != root && !cur->isRed) {
			if (cur == cur->parent->left) {
				TreeNode* brother = cur->parent->right;

				if (brother->isRed) {
					brother->isRed = false;
					cur->parent->isRed = true;
					leftRotate(cur->parent);
					brother = cur->parent->right;
				}

				if (!(brother->left->isRed) && !(brother->right->isRed)) {
					brother->isRed = true;
					cur = cur->parent;
				}
				else {
					if (!(brother->right->isRed)) {
						brother->left->isRed = false;
						brother->isRed = true;
						rightRotate(brother);
						brother = cur->parent->right;
					}

					brother->isRed = cur->parent->isRed;
					cur->parent->isRed = false;
					brother->right->isRed = false;
					leftRotate(cur->parent);
					cur = root;
				}
			}

			else {
				TreeNode* brother = cur->parent->left;

				if (brother->isRed) {
					brother->isRed = false;
					cur->parent->isRed = true;
					rightRotate(cur->parent);
					brother = cur->parent->left;
				}

				if (!(brother->left->isRed) && !(brother->right->isRed)) {
					brother->isRed = true;
					cur = cur->parent;
				}
				else {
					if (!(brother->left->isRed)) {
						brother->right->isRed = false;
						brother->isRed = true;
						leftRotate(brother);
						brother = cur->parent->left;
					}

					brother->isRed = cur->parent->isRed;
					cur->parent->isRed = false;
					brother->left->isRed = false;
					rightRotate(cur->parent);
					cur = root;
				}
			}
		}
		cur->isRed = false;
	}

	void inorderHelper(TreeNode* cur, vector<int>& arr) {
		if (cur && cur != nil) {
			inorderHelper(cur->left, arr);
			arr.emplace_back(cur->val);
			inorderHelper(cur->right, arr);
		}
	}

	int getBalanceFactor(TreeNode* cur) {
		if (!cur || cur == nil)
			return 0;
		if (cur->isRed)
			if (cur->left->isRed or cur->right->isRed)
				return -1;
		int left = getBalanceFactor(cur->left);
		int right = getBalanceFactor(cur->right);
		if (left == right) {
			if (left == -1) return -1;
			return cur->isRed ? left : 1 + left;
		}
		return -1;
	}

public:
	RBTree() :nil(new TreeNode(-0x8000, false)), root(nil) {}

	bool find(int val) {
		TreeNode* cur = root;
		while (cur && cur != nil) {
			if (cur->val == val) return true;
			else if (cur->val > val)
				cur = cur->left;
			else cur = cur->right;
		}
		return false;
	}

	void insert(int val) {
		TreeNode* node = new TreeNode(val);
		node->left = nil;
		node->right = nil;

		TreeNode* parent = nullptr;
		TreeNode* cur = root;

		while (cur && cur != nil) {
			parent = cur;
			if (val < cur->val)
				cur = cur->left;
			else cur = cur->right;
		}

		node->parent = parent;
		if (!parent) {
			root = node;
			root->isRed = false;
		}
		else if (node->val < parent->val)
			parent->left = node;
		else parent->right = node;

		fixInsert(node);
	}

	void del(int val) {
		TreeNode* node = findNode(val);
		if (!node || node == nil) {
			cout << "Node Cannot Be Found!" << endl;
			return;
		}

		TreeNode* delete_node = node;
		TreeNode* x = nullptr;
		if (delete_node->left == nil)
			x = delete_node->right;
		else if (delete_node->right == nil)
			x = delete_node->left;
		else {
			TreeNode* y = findSuc(delete_node);
			delete_node->val = y->val;
			delete_node = y;
			x = delete_node->right;
		}

		trans(delete_node, x);
		if (!delete_node->isRed)
			fix_delete(x);
	}

	bool isBalance() {
		if (root->isRed) return false;
		return getBalanceFactor(root) != -1;
	}

	vector<int> inorder() {
		vector<int> arr;
		inorderHelper(root, arr);
		return arr;
	}

	TreeNode* getRoot() const {
		return root;
	}
};
