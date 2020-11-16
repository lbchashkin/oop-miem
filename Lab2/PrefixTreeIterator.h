#ifndef PREFIXTREEITERATOR_H
#define PREFIXTREEITERATOR_H
#include "PrefixTree.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

template <class V>
class PrefixTreeIterator {
public:
    PrefixTreeIterator(const PrefixTree<V>* tree, const char key[]);
    PrefixTreeIterator operator++(int);
    char* operator*() const;
    PrefixTreeIterator(const PrefixTreeIterator& iter);
    void begin();
    void end();
    ~PrefixTreeIterator();
private:
    class iteratoritem{
    public:
        iteratoritem(const char key[], const int length) {
            _key = new char[length+1];
            for (int i=0; i<length; i++)
                _key[i] = key[i];
            _key[length] = '\0';
            _length = length;
            next = NULL;
        };
        char* _key;
        int _length;
        iteratoritem* next;
        ~iteratoritem() {delete[] _key;}
    };
    iteratoritem* first = NULL;
    iteratoritem* curr = first;
    void addkey(typename PrefixTree<V>::nodeitem* root, char key[], const V& i);
};

template <class V>
PrefixTreeIterator<V>::PrefixTreeIterator(const PrefixTree<V> *tree, const char key[]) {
    int i=0;
    typename PrefixTree<V>::nodeitem* t = tree->_root;
    char* keyitem = new char[tree->getMaxLength()+1];
    while (key[i]) {
        map<char, typename PrefixTree<V>::nodeitem*> dict = t->items;
        if (t->items.count(key[i])) {
            t = dict[key[i]];
            keyitem[i] = key[i];
        }
        else
            return;
        i++;
    }
    addkey(t, keyitem, i);
    this->begin();
    delete[] keyitem;
}

template <class V>
PrefixTreeIterator<V> PrefixTree<V>::getKeys(const char key[]) const {
    return PrefixTreeIterator<V>(this, key);
}

template <class V>
PrefixTreeIterator<V> PrefixTreeIterator<V>::operator++(int) {
    PrefixTreeIterator newiterator(*this);
    if (curr) curr = curr->next;
    return newiterator;
}

template <class V>
char* PrefixTreeIterator<V>::operator*() const {
    if (curr)
        return curr->_key;
    else
        return (char *)"";
}

template <class V>
PrefixTreeIterator<V>::PrefixTreeIterator(const PrefixTreeIterator<V>& iter) {
    if (iter.first) {
        first = new iteratoritem(iter.first->_key, iter.first->_length);
        iteratoritem* t1 = iter.first->next;
        iteratoritem* t2 = first;
        curr = first;
        while (t1) {
            iteratoritem* new_item = new iteratoritem(t1->_key, t1->_length);
            if (t1 == iter.curr)
                curr = new_item;
            t2->next = new_item;
            t2 = new_item;
            t1 = t1->next;
        }
    }
    else
        return;
}

template <class V>
void PrefixTreeIterator<V>::begin() {
    curr = first;
}

template <class V>
void PrefixTreeIterator<V>::end() {
    curr = NULL;
}

template <class V>
PrefixTreeIterator<V>::~PrefixTreeIterator() {
    iteratoritem* t = first;
    while (t) {
        t = first->next;
        delete first;
        first = t;
    }
}

template <class V>
void PrefixTreeIterator<V>::addkey(typename PrefixTree<V>::nodeitem* root, char key[], const V& i) {
    if (root->isvalue) {
        iteratoritem* t = new iteratoritem(key, i);
        if (!first) {
            first = t;
            curr = t;
        }
        else
            curr->next = t;
        curr = t;
    }
    typename map<char, typename PrefixTree<V>::nodeitem*>::iterator it, end;
    it = root->items.begin();
    end = root->items.end();
    while (it != end) {
        key[i] = it->first;
        addkey(it->second, key, i+1);
        key[i] = '\0';
        it++;
    }
}

template <class V>
PrefixTree<V>::PrefixTree(PrefixTree &tree) {
    cout << "New PrefixTree" << endl;
    PrefixTreeIterator<V> iter = tree.getKeys("");
    count_keys = 0;
    count_nodes = 0;
    max_length = 0;
    _root = new nodeitem;
    char s[] = "";
    while (*(*iter) != *s) {
        add(*iter, tree[*iter]);
        iter++;
    }
}

template <class V>
bool PrefixTree<V>::operator==(const PrefixTree &tree)
{
    PrefixTreeIterator<V> iter = getKeys("");
    PrefixTreeIterator<V> iter2 = getKeys("");
    char s[] = "";
    while (*(*iter) != *s) {
        try {
            if ((*this)[*iter] != tree[*iter])
                return false;
        }  catch (out_of_range) {
            return false;
        }
        iter++;
    }
    while (*(*iter2) != *s) {
        try {
            if ((*this)[*iter2] != tree[*iter2])
                return false;
        }  catch (out_of_range) {
            return false;
        }
        iter2++;
    }
    return true;
}

/*template <class V>
bool PrefixTreeToJSON(const QString &filename, const PrefixTree<V>& tree) {
    QFile file(filename);
    if (file.open(QFile::WriteOnly)) {
        QJsonDocument json_doc;
        QJsonObject object;
        PrefixTreeIterator iter = tree.getKeys("");
        char s[] = "";
        while (*(*iter) != *s) {
            object.insert(*iter, tree[*iter]);
            iter++;
        }
        json_doc.setObject(object);
        if (file.write(json_doc.toJson()) == -1) {
            file.close();
            return false;
        }
        file.close();
        return true;
    }
    else
        return false;
}*/

#endif // PREFIXTREEITERATOR_H
