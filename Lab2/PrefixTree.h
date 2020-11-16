#ifndef PREFIXTREE_H
#define PREFIXTREE_H

#include <map>
#include <iostream>
#include <QString>

using namespace std;
template <class V>
class PrefixTreeIterator;

template <class V>
class PrefixTree {
public:
    PrefixTree();
    PrefixTree(PrefixTree& tree);
    void add(const char key[], const V value);
    PrefixTreeIterator<V> getKeys(const char key[]) const;
    V& operator[](const char key[]) const;
    bool operator==(const PrefixTree& tree);
    void delkey(char key[]);
    void delall();
    bool isKey(char key[]);
    friend class PrefixTreeIterator<V>;
    int getCountKeys() const;
    int getCountNodes() const;
    int getMaxLength() const;
    ~PrefixTree();
private:
    class nodeitem {
    public:
        nodeitem() {cout << "New nodeitem" << endl;}
        void add(const char key) {
            items[key] = new nodeitem;
            items[key]->parent = this;
                           }
        ~nodeitem() {
            cout << "Delete nodeitem" << endl;
            typename map<char, nodeitem*>::iterator it, end;
            it = items.begin();
            end = items.end();
            while (it != end) {
                delete it->second;
                it++;
            }
        }
        V value;
        bool isvalue = false;
        nodeitem* parent = NULL;
        map<char, nodeitem*> items;
    };
    int count_nodes;
    int count_keys;
    int max_length;
    nodeitem* _root;
};

//bool PrefixTreeToJSON(const QString &filename, const PrefixTree& tree);
//PrefixTree PrefixTreeFromJSON(const QString &filename);

template <class V>
void PrefixTree<V>::add(const char key[], const V value) {
    int i=0;
    nodeitem* t = _root;
    while (key[i]) {
        if (t->items.count(key[i]) == 0) {
            t->add(key[i]);
            count_nodes++;
        }
        t = t->items.at(key[i]);
        i++;
    }
    if (t->isvalue)
        count_keys--;
    t->value = value;
    t->isvalue = true;
    if (max_length < i) max_length = i;
    count_keys++;
}

template <class V>
V& PrefixTree<V>::operator[](const char key[]) const {
    int i=0;
    nodeitem* t = _root;
    while (key[i]) {
        map<char, nodeitem*> dict = t->items;
        if (t->items.count(key[i]))
            t = dict[key[i]];
        else
            throw out_of_range("Key error");
        i++;
    }
    if (t->isvalue)
        return t->value;
    else
        throw out_of_range("Key error");
}

template <class V>
void PrefixTree<V>::delkey(char key[]) {
    if (isKey(key)) {
        int i=0;
        nodeitem* t = _root;
        while (key[i]) {
            map<char, nodeitem*> dict = t->items;
            t = dict[key[i]];
            i++;
        }
        i--;
        if (t->items.size() != 0) {
            t->isvalue = false;
            count_keys--;
            return;
        }
        while (t->parent)
            if ((t->parent->items.size() == 1) && (t->parent->isvalue == 0)) {
                t = t->parent;
                i--;
                count_nodes--;
            }
            else
                break;
        if (t->parent) {
            t->parent->items.erase(key[i]);
            delete t;
            count_nodes--;
        }
        else {
            delete t;
            _root = new nodeitem;
        }
        count_keys--;
    }
    else
        throw out_of_range("Key error");
}

template <class V>
void PrefixTree<V>::delall() {
    delete _root;
    count_nodes = 0;
    count_keys = 0;
    max_length = 0;
    _root = new nodeitem;

}

template <class V>
bool PrefixTree<V>::isKey(char key[]) {
    int i=0;
    nodeitem* t = _root;
    while (key[i]) {
        map<char, nodeitem*> dict = t->items;
        if (t->items.count(key[i]))
            t = dict[key[i]];
        else
            return false;
        i++;
    }
    return t->isvalue;
}

template <class V>
int PrefixTree<V>::getCountKeys() const {
    return count_keys;
}

template <class V>
int PrefixTree<V>::getCountNodes() const {
    return count_nodes;
}

template <class V>
int PrefixTree<V>::getMaxLength() const {
    return max_length;
}

template <class V>
PrefixTree<V>::PrefixTree() {
    count_keys = 0;
    count_nodes = 0;
    max_length = 0;
    _root = new nodeitem;
}

template <class V>
PrefixTree<V>::~PrefixTree() {
    cout << "Delete Tree" << endl;
    delete _root;
}
#endif // PREFIXTREE_H
