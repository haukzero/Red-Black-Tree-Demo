#include <random>
#include "RBTree.h"
using namespace std;

bool judge(RBTree tree, vector<int>& deleted) {
	bool isBalance = tree.isBalance();
	if (!isBalance) return false;
	
	vector<int> inorder = tree.inorder();
	int n = inorder.size();
	for (int i = 1; i < n; ++i)
		if (inorder[i - 1] > inorder[i])
			return false;

	for (int val : deleted) {
		int l = 0, r = n;
		while (l < r) {
			int mid = (l + r) >> 1;
			if (inorder[mid] == val)
				return false;
			else if (inorder[mid] > val)
				r = mid;
			else l = mid + 1;
		}
	}

	return true;
}


int main()
{
	cout << boolalpha;
	for (int i = 0; i < 5; ++i) {
		cout << "=========== Time " << i << " ===========" << endl;
		RBTree tree = RBTree();
		vector<int> deleted;
		for (int i = 0; i < 0x7fff; ++i)
			tree.insert(i);
		cout << "OK after Insert? " << judge(tree, deleted) << endl;
		for (int i = 0; i < 5; ++i) {
			int x = rand() % 0x7fff;
			deleted.emplace_back(x);
			cout << "Deleting node: " << x << endl;
			tree.del(x);
			cout << "Delete node [ " << x << " ] successfully!" << endl;
		}
		cout << "OK after Delete? " << judge(tree, deleted) << endl;
		cout << "=============================\n" << endl;
	}
	return 0;
}
