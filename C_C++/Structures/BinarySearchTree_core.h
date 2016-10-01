#ifndef	BINARYSEARCHTREE_CORE_H
#define BINARYSEARCHTREE_CORE_H

#include <cstdlib>
#include <iostream>

#include "BinarySearchTree.h"

namespace algos {
	namespace Struct {

		template <typename _Tp>
		BST<_Tp>::Node::Node()
			: left(NULL),
			right(NULL),
			parent(NULL),
			key(_Tp()) {
		}

		template <typename _Tp>
		BST<_Tp>::Node::Node(_Tp _key) 
			:key(_key),
			left(NULL),
			right(NULL),
			parent(NULL) {
		}



		template <typename _Tp>
		BST<_Tp>::BST()
			: root(NULL) {
			__begin = NULL;
		}

		template <typename _Tp>
		_Tp BST<_Tp>::max() {
			BST<_Tp>::Node* n = _max(root);
			if (n == NULL) return _Tp();

			return n->key;
		}

		template <typename _Tp>
		_Tp BST<_Tp>::min() {
			BST<_Tp>::Node* n = _min(root);
			if (n == NULL) return _Tp();

			return n->key;
		}

		template <typename _Tp>
		void BST<_Tp>::insert(typename BST<_Tp>::Node* _node) {
			BST<_Tp>::Node* y = NULL;
			BST<_Tp>::Node *x = root;
			while (x != NULL) {
				y = x;
				if (_node->key < x->key) x = x->left;
				else x = x->right;
			}

			_node->parent = y;
			if (y == NULL) root = _node;
			else if (_node->key < y->key) y->left = _node;
			else y->right = _node;
		}

		template <typename _Tp>
		typename BST<_Tp>::Node* BST<_Tp>::top() {
			return root;
		}


		template <typename _Tp>
		typename BST<_Tp>::Node* BST<_Tp>::find(_Tp _key) {
			return _find(root, _key);
		}

		template <typename _Tp>
		void BST<_Tp>::inOrderWalk() {
			_inOrder(root);
		}

		template <typename _Tp>
		void BST<_Tp>::preOrder() {
			_preOrder(root);
		}

		template <typename _Tp>
		void BST<_Tp>::postOrder() {
			_postOrder(root);
		}

		template <typename _Tp>
		typename BST<_Tp>::Node* BST<_Tp>::sucessor(typename BST<_Tp>::Node* node) {
			if (node->right != NULL) return _min(node->right);

			BST<_Tp>::Node *x = node, *y = node->parent;
			while (y != NULL && y->right == x) {
				x = y;
				y = y->parent;
			}

			return y;
		}

		template <typename _Tp>
		typename BST<_Tp>::Node* BST<_Tp>::predecessor(typename BST<_Tp>::Node* node) {
			if (node->left != NULL) return _max(node->left);

			BST<_Tp>::Node* x = node, *y = node->parent;
			while (y != NULL && x == y->left) {
				x = y;
				y = y->parent;
			}

			return y;
		}

		template <typename _Tp>
		void BST<_Tp>::erase(typename BST<_Tp>::Node* z) {
			if (z->left == NULL) transplant(*z, *z->right);
			else if (z->right == NULL) transplant(*z, *z->left);
			else {
				BST<_Tp>::Node* y = _min(z->right);
				if (z->parent != z) {
					transplant(*z, *z->right);
					y->right = z->right;
					y->right->parent = y;
				}

				transplant(*z, *y);
				y->left = z->left;
				y->left->parent = y;
			}
		}

		template <typename _Tp>
		void BST<_Tp>::transplant(typename BST<_Tp>::Node& u, typename BST<_Tp>::Node& v) {
			if (u.parent == NULL) root = &v;
			else if (u.parent->left == &u) u.parent->left = &v;
			else u.parent->right = &v;

			if (&v != NULL) v.parent = u.parent;
		}

		template <typename _Tp>
		typename BST<_Tp>::Node* BST<_Tp>::_find(typename BST<_Tp>::Node* _node, int _key) {
			if (_node == NULL || _node->key == _key) return _node;

			if (_node->key < _key) return _find(_node->right, _key);
			return _find(_node->left, _key);

		}

		template <typename _Tp>
		void BST<_Tp>::_inOrder(typename BST<_Tp>::Node* node) {
			if (node == NULL) return;
			_inOrder(node->left);
			std::cout << node->key << " ";
			_inOrder(node->right);
		}

		template <typename _Tp>
		void BST<_Tp>::_preOrder(typename BST<_Tp>::Node* node) {
			if (node == NULL) return;
			std::cout << node->key << " ";
			_preOrder(node->left);
			_preOrder(node->right);
		}

		template <typename _Tp>
		void BST<_Tp>::_postOrder(typename BST<_Tp>::Node* node) {
			if (node == NULL) return;
			_postOrder(node->left);
			_postOrder(node->right);
			std::cout << node->key << " ";
		}

		template <typename _Tp>
		typename BST<_Tp>::Node* BST<_Tp>::_min(typename BST<_Tp>::Node* node) {
			if (node == NULL) return NULL;
			BST<_Tp>::Node* tmp = node;

			while (tmp->left != NULL) tmp = tmp->left;

			return tmp;
		}

		template <typename _Tp>
		typename BST<_Tp>::Node* BST<_Tp>::_max(typename BST<_Tp>::Node* node) {
			if (node == NULL) return NULL;
			BST<_Tp>::Node* tmp = node;

			while (tmp->right != NULL) tmp = tmp->right;

			return tmp;
		}
	}
}




#endif