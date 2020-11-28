#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;



class node {
public:
	node* parents;
	int index;
	node* left;
	node* right;
	node() {};
	~node() {
		index = 0;
		left = right = parents = NULL;
		delete left;
		delete right;
		delete parents;
	};
	node(int a) {
		parents = NULL;
		index = a;
		left = NULL;
		right = NULL;
	};
	node(int a, node* par) {
		parents = par;
		index = a;
		left = right = NULL;
	}
};

class tree {
public:
	node* root;
	string post_order;
	string in_order;
	string pre_order;
	vector<vector<node*>>node_count;
	int survive_level;
	tree() {};
	tree(int a) {
		root = new node(a);
	}
	node* search(int tar, node* cur) {
		if (tar < cur->index) {
			if (cur->left != NULL) {
				return search(tar, cur->left);
			}
			else {

				//	cout << tar << " not found." << endl;
				//	cout << "now at " << cur->index << endl;
				return cur;
			}
		}
		else if (tar == cur->index) {
			// cout << tar << " found." << endl;
			return cur;
		}
		else {
			if (cur->right != NULL) {
				return 	search(tar, cur->right);

			}
			else {
				//	cout << tar << " not found." << endl;
				//	cout << "now at " << cur->index << endl;
				return cur;
			}
		}
	}

	void add(int tar) {
		node* cur = search(tar, root);
		node* NEW = new node(tar, cur);
		if (tar < cur->index) {
			// cout << "insert " << tar << " into the left of " << cur->index << endl;
			cur->left = NEW;
		}
		else {
			// cout << "insert " << tar << " into the right of " << cur->index << endl;
			cur->right = NEW;
		}
	}

	void travel(node* cur, int count = 0) {
		if (cur == NULL) {
			return;
		}
		if (node_count.size() <= count) {
			node_count.resize(count + 1);
		}
		node_count[count].push_back(cur);
		travel(cur->left, count + 1);
		travel(cur->right, count + 1);
	}

	void del(int tar) {
		node* cur = (search(tar, root)); // node to be deleted 
		node* cur_parents = cur->parents;
		node* reserve_right = cur->right;
		node* reserve_left = cur->left;
		// current is now at the left of its parents , waiting to be deleted
		if (cur == cur->parents->left) {
			//case 1 : no desenster
			if (cur->left == NULL && cur->right == NULL) {
				cur_parents->left = NULL;
			}

			//case 2 : only one subtree
			else if (cur->left == NULL) {
				cur_parents->left = cur->right;
				cur->right->parents = cur->parents;
				delete cur;
			}
			else if (cur->right == NULL) {
				cur_parents->left = cur->left;
				cur->left->parents = cur->parents;
				delete cur;
			}
			//find the succeesor of the left subtree
			else {
				node* tmp = cur->left;
				while (tmp->right != NULL) {
					tmp = tmp->right;
				}
				del(tmp->index);
				tmp->parents = cur->parents;
				if (tmp != reserve_right) {
					tmp->right = reserve_right;
				}
				if (tmp != reserve_left) {
					tmp->left = reserve_left;
				}
				cur_parents->left = tmp;
				//delete cur;
			}
		}
		else {

			//case 1 : no desenster
			if (cur->left == NULL && cur->right == NULL) {
				cur_parents->right = NULL;

			}

			//case 2 : only one subtree
			else if (cur->left == NULL) {
				cur_parents->right = cur->right;
				cur->right->parents = cur->parents;
				delete cur;
			}
			else if (cur->right == NULL) {
				//find the successor of the left

				cur_parents->right = cur->left;
				cur->left->parents = cur->parents;
				delete cur;

			}
			//find the succeesor of the left subtree
			else {
				node* tmp = cur->left;
				while (tmp->right != NULL) {
					tmp = tmp->right;
				}
				del(tmp->index);
				tmp->parents = cur->parents;
				if (tmp != reserve_right) {
					tmp->right = reserve_right;
				}
				if (tmp != reserve_left) {
					tmp->left = reserve_left;
				}
				cur_parents->right = tmp;
				//delete cur;


			}
		}
	}

	string convert2string(int a) {
		string s;
		if (a == 0) {
			s += '0';
		}
		while (a > 0)
		{
			s += (a % 10) + '0';
			a /= 10;
		}
		string b;
		for (int i = s.length() - 1; i >= 0; i--)
		{
			b += s[i];
		}
		return b;
	}

	void postorder(node* cur) {
		if (cur == NULL) {
			return;
		}
		else {
			postorder(cur->left);
			postorder(cur->right);
			post_order += convert2string(cur->index);
			post_order += " ";
		}
	}

	void cut() {
		int half = 0;
		for (int i = 0; i < node_count.size(); i++)
		{
			half += node_count[i].size();
		}
		half /= 2;
		int g = 0;
		survive_level = 0;
		while (survive_level < node_count.size()) {
			g += node_count[survive_level].size();
			if (g >= half) {
				break;
			}
			survive_level++;
		}
	}

	void inorder(node* cur, int sur = 0) {
		if (cur == NULL) {
			return;
		}
		else {
			inorder(cur->left, sur + 1);
			if (sur <= survive_level) {
				in_order += convert2string(cur->index);
				in_order += " ";
			}
			inorder(cur->right, sur + 1);
		}
	}

	void preorder(node* cur, int sur = 0) {
		if (cur == NULL) {
			return;
		}
		else {
			if (sur > survive_level) {
				pre_order += convert2string(cur->index);
				pre_order += " ";
			}
			preorder(cur->left, sur + 1);
			preorder(cur->right, sur + 1);
		}
	}

	void print(string a) {
		if (a == "postorder") {
			postorder(root);
			post_order = post_order.substr(0, post_order.length() - 1);
			cout << post_order << endl;
		}
		//determine where to cut(print)
		else if (a == "S") {

			inorder(root);
			in_order = in_order.substr(0, in_order.length() - 1);
			cout << in_order << endl;
		}
		else if (a == "V") {
			preorder(root);
			pre_order = pre_order.substr(0, pre_order.length() - 1);
			cout << pre_order << endl;
		}
	}
};

int main() {
	int T;
	cin >> T;
	tree Tree(T);
	int citizens;
	string tmp;
	getline(cin, tmp);
	stringstream ss;
	while (tmp != "") {
		if (tmp.find(" ") != -1) {
			ss << tmp.substr(0, tmp.find(" ")) << " ";
			tmp = tmp.substr(tmp.find(" ") + 1, tmp.length());
		}
		else {
			ss << tmp;
			tmp = "";
		}
	}
	while (ss >> tmp) {
		citizens = stoi(tmp);
		Tree.add(citizens);
	}
	Tree.print("postorder");
	ss.str("");
	ss.clear();
	tmp = "";
	getline(cin, tmp);
	int avengers;
	while (tmp != "") {
		if (tmp.find(" ") != -1) {
			ss << tmp.substr(0, tmp.find(" ")) << " ";
			tmp = tmp.substr(tmp.find(" ") + 1, tmp.length());
		}
		else {
			ss << tmp;
			tmp = "";
		}
	}
	while (ss >> tmp) {
		avengers = stoi(tmp);
		Tree.del(avengers);
	}
	Tree.travel(Tree.root);
	Tree.cut();
	Tree.print("S");
	Tree.print("V");
	return 0;
}