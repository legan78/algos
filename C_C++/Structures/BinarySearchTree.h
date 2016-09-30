#ifndef	BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

namespace algos {
	namespace Struct {
		template <typename _Tp>
		class BST {
		public:

			class Node {
			friend class BST;
			public:
				Node();
				Node(_Tp _key);

				friend std::ostream& operator<<(std::ostream& os, Node& n) {
					os << n.key;
					return os;
				}

			protected:
				Node* left;
				Node* right;
				Node* parent;

				_Tp key;
			};

			BST();

			_Tp max();

			_Tp min();

			void insert(Node* _node);

			Node* find(_Tp _key);

			void inOrderWalk();
			void preOrder();

			void postOrder();

			Node* sucessor(Node* node);

			Node* predecessor(Node* node);

			Node* top();

			void erase(Node* z);

		protected:

			void transplant(Node& u, Node& v);
			void _inOrder(Node* node);
			void _preOrder(Node* node);
			void _postOrder(Node* node);


			Node* _find(Node* _node, int _key);
			Node* _min(Node* node);
			Node* _max(Node* node);

			Node* root;
		};
	}
}




#endif