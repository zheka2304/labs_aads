#include <iostream>
#include "list.h"


#ifndef M_MAP_H
#define M_MAP_H

template <typename K, typename V>
class rb_map {
    class rb_tree {
    public:
        enum node_color : int {
            BLACK = 0,
            RED = 1
        };

        // red-black tree node
        class rb_node {
        public:
            typedef typename list<K>::iterator key_iter;
            typedef typename list<V>::iterator value_iter;

            K key;
            key_iter key_iterator;
            value_iter value_iterator;
            node_color color = BLACK;

            rb_node* left = nullptr;
            rb_node* right = nullptr;
            rb_node* parent = nullptr;

            rb_node(key_iter key_iter, value_iter value_iter) {
                this->key = *key_iter;
                key_iterator = key_iter;
                value_iterator = value_iter;
            }

            ~rb_node() {
                delete(right);
                delete(left);
            }

            int get_size() {
                return 1 + (left != nullptr ? left->get_size() : 0) + (right != nullptr ? right->get_size() : 0);
            }

            void show_tree(int depth = 0) {
                if (left != nullptr) {
                    left->show_tree(depth + 1);
                }
                for (int i = 0; i < depth; i++) {
                    std::cout << "    ";
                }
                std::cout << key << ":" << *value_iterator << (color == RED ? "[R]" : "[B]") << "\n";
                if (right != nullptr) {
                    right->show_tree(depth + 1);
                }
            }

            void print() {
                if (left != nullptr) {
                    left->print();
                }
                std::cout << key << ": " << *value_iterator << ", ";
                if (right) {
                    right->print();
                }
            }
        };

        rb_node* root = nullptr;

        int get_size() {
            return root != nullptr ? root->get_size() : 0;
        }

        ~rb_tree() {
            delete(root);
        }

        void clear() {
            delete(root);
            root = nullptr;
        }

        void show_tree() {
            if (root != nullptr) {
                root->show_tree();
            } else {
                std::cout << "empty tree\n";
            }
        }

        rb_node* get_node(K key) {
            rb_node* node = root;
            while (node != nullptr) {
                if (node->key == key) {
                    return node;
                }
                if (node->key < key) {
                    node = node->right;
                } else {
                    node = node->left;
                }
            }
            return nullptr;
        }

        void left_rotate(rb_node* node) {
            rb_node* tmp = node->right;
            node->right = tmp->left;
            if (tmp->left != nullptr) {
                tmp->left->parent = node;
            }
            tmp->parent = node->parent;

            if (node->parent == nullptr) {
                root = tmp;
            } else {
                if (node == node->parent->left) {
                    node->parent->left = tmp;
                } else {
                    node->parent->right = tmp;
                }
            }
            tmp->left = node;
            node->parent = tmp;
        }

        void right_rotate(rb_node* node) {
            rb_node* tmp = node->left;
            node->left = tmp->right;
            if (tmp->right != nullptr) {
                tmp->right->parent = node;
            }
            tmp->parent = node->parent;

            if (node->parent == nullptr) {
                root = tmp;
            } else {
                if (node == node->parent->left) {
                    node->parent->left = tmp;
                } else {
                    node->parent->right = tmp;
                }
            }
            tmp->right = node;
            node->parent = tmp;
        }

        void insert_fixup(rb_node* x) {
            while (x->parent != nullptr && x->parent->color == RED) {
                if (x->parent == x->parent->parent->left) {
                    rb_node* y = x->parent->parent->right;
                    if (y != nullptr && y->color == RED) {
                        x->parent->color = BLACK;
                        y->color = BLACK;
                        x->parent->parent->color = RED;
                        x = x->parent->parent;
                    } else {
                        if (x == x->parent->right) {
                            x = x->parent;
                            left_rotate(x);
                        }
                        x->parent->color = BLACK;
                        x->parent->parent->color = RED;
                        right_rotate(x->parent->parent);
                    }
                } else {
                    rb_node* y = x->parent->parent->left;
                    if (y != nullptr && y->color == RED) {
                        x->parent->color = BLACK;
                        y->color = BLACK;
                        x->parent->parent->color = RED;
                        x = x->parent->parent;
                    } else {
                        if (x == x->parent->left) {
                            x = x->parent;
                            right_rotate(x);
                        }
                        x->parent->color = BLACK;
                        x->parent->parent->color = RED;
                        left_rotate(x->parent->parent);
                    }
                }
            }
            root->color = BLACK;
        }

        bool insert(rb_node* node) {
            rb_node* last_node = nullptr;
            rb_node* current_node = root;
            while (current_node != nullptr) {
                last_node = current_node;
                if (node->key == current_node->key) {
                    *current_node->value_iterator = *node->value_iterator;
                    return false;
                }
                if (node->key < current_node->key) {
                    current_node = current_node->left;
                } else {
                    current_node = current_node->right;
                }
            }
            node->parent = last_node;
            if (last_node == nullptr) {
                root = node;
            } else if (node->key < last_node->key) {
                last_node->left = node;
            } else {
                last_node->right = node;
            }
            node->left = node->right = nullptr;
            node->color = RED;
            insert_fixup(node);
            return true;
        }

        void remove_fixup(rb_node* x) {
            while (x != root && (x == nullptr || x->color == BLACK)) {
                if (x == x->parent->left) {
                    rb_node* y = x->parent->right;
                    if (y != nullptr && y->color == RED) {
                        y->color = BLACK;
                        x->parent->color = RED;
                        left_rotate(x->parent);
                        y = x->parent->right;
                    }
                    if (y == nullptr) {
                        break;
                    }
                    if ((y->left == nullptr || y->left->color == BLACK) &&
                        (y->right == nullptr || y->right->color == BLACK)) {
                        y->color = RED;
                        x = x->parent;
                    } else {
                        if (y->right == nullptr || y->right->color == BLACK) {
                            y->left->color = BLACK;
                            y->color = RED;
                            right_rotate(y);
                            y = x->parent->right;
                        }
                        y->color = x->parent->color;
                        x->parent->color = BLACK;
                        y->right->color = BLACK;
                        left_rotate(x->parent);
                        x = root;
                    }
                } else {
                    rb_node* y = x->parent->left;
                    if (y != nullptr && y->color == RED) {
                        y->color = BLACK;
                        x->parent->color = RED;
                        right_rotate(x->parent);
                        y = x->parent->left;
                    }
                    if (y == nullptr) {
                        break;
                    }
                    if ((y->left == nullptr || y->left->color == BLACK) &&
                        (y->right == nullptr || y->right->color == BLACK)) {
                        y->color = RED;
                        x = x->parent;
                    } else {
                        if (y->left == nullptr || y->left->color == BLACK) {
                            y->right->color = BLACK;
                            y->color = RED;
                            left_rotate(y);
                            y = x->parent->left;
                        }
                        y->color = x->parent->color;
                        x->parent->color = BLACK;
                        y->left->color = BLACK;
                        right_rotate(x->parent);
                        x = root;
                    }

                }
            }
        }

        rb_node* tree_successor(rb_node* node) {
            if (node->right != nullptr) {
                while (node->left != nullptr) {
                    node = node->left;
                }
                return node;
            }
            rb_node* tmp = node->parent;
            while (tmp != nullptr && node == tmp->right) {
                node = tmp;
                tmp = tmp->parent;
            }
            return tmp;
        }

        rb_node* remove(rb_node* node) {
            rb_node* y;
            if (node->left == nullptr || node->right == nullptr) {
                y = node;
            } else {
                y = tree_successor(node);
            }
            if (y == nullptr) {
                show_tree();
                std::cout << " " << node->key << " ";
            }
            rb_node* x;
            if (y->left != nullptr) {
                x = y->left;
            } else {
                x = y->right;
            }

            if (x != nullptr) {
                x->parent = y->parent;
            }
            if (y->parent == nullptr) {
                root = x;
            } else {
                if (y == y->parent->left) {
                    y->parent->left = x;
                } else {
                    y->parent->right = x;
                }
            }
            if (y != node) {
                node->key = y->key;
                *node->value_iterator = *y->value_iterator;
            }
            if (y->color == BLACK && x != nullptr) {
                remove_fixup(x);
            }
            return y;
        }
    };

public:
    class invalid_key_exception : public std::exception {

    };

private:
    rb_tree tree;
    list<V> key_list;
    list<V> value_list;

public:
    typedef typename rb_map<K, V>::rb_tree::rb_node node_t;

    V& operator[] (K const& key) { // insert
        node_t* found = tree.get_node(key);
        if (found != nullptr) {
            return *(found->value_iterator);
        } else {
            node_t* node = new node_t(key_list.add(key), value_list.add(V()));
            tree.insert(node);
            return *(node->value_iterator);
        }
    }

    V const& operator[] (K const& key) const { // access
        node_t* node = tree.get_node(key);
        if (node != nullptr) {
            return *(node->value_iterator);
        }
        throw invalid_key_exception();
    }

    bool remove(K key) {
        node_t* node = tree.get_node(key);
        if (node != nullptr) {
            node = tree.remove(node);
            key_list.erase(node->key_iterator);
            value_list.erase(node->value_iterator);
            node->right = node->left = node->parent = nullptr;
            delete(node);
            return true;
        }
        return false;
    }

    node_t* find(K key) {
        return tree.get_node(key);
    }

    bool has(K key) {
        return find(key) != nullptr;
    }

    void print() {
        std::cout << "{";
        if (tree.root != nullptr) {
            tree.root->print();
        }
        std::cout << "}\n";
    }

    void show_tree() {
        std::cout << "rb_map tree:\n";
        tree.show_tree();
        std::cout << "\n";
    }

    list<K>& keys() {
        return key_list;
    }

    list<V>& values() {
        return value_list;
    }

    int length() {
        return value_list.get_length();
    }

    int tree_size() {
        return tree.get_size();
    }

    void clear() {
        tree.clear();
        key_list.clear();
        value_list.clear();
    }
};

#endif