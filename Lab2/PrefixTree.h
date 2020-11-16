#ifndef PREFIXTREE_H
#define PREFIXTREE_H

#include <map>
#include <iostream>
#include <QString>

using namespace std;
template <class K, class V>
class PrefixTreeIterator;

template <class K, class V>
class PrefixTree {
public:
    PrefixTree();
    PrefixTree(PrefixTree& tree);
    void add(const char key[], const V value);
    PrefixTreeIterator<K, V> getKeys(const char key[]) const;
    V& operator[](const char key[]) const;
    bool operator==(const PrefixTree& tree);
    void delkey(char key[]);
    void delall();
    bool isKey(char key[]);
    friend class PrefixTreeIterator<K, V>;
    int getCountKeys() const;
    int getCountNodes() const;
    int getMaxLength() const;
    bool PrefixTreeToJSON(const QString &filename);
    void PrefixTreeFromJSON(const QString &filename);
    ~PrefixTree();
private:
    class nodeitem {
        //Узел префиксного дерева
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
    int count_nodes; //Количество узлов
    int count_keys; //Количество ключей
    int max_length; //Максимальная длина ключа
    nodeitem* _root;
};

template <class K, class V>
void PrefixTree<K, V>::add(const char key[], const V value) {
    //Добавление пары ключ-значение в префиксное дерево
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

template <class K, class V>
V& PrefixTree<K, V>::operator[](const char key[]) const {
    //Перегрузка [] - получение значения по ключу
    //При отсутствии ключа - исключение out_of_range
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

template <class K, class V>
void PrefixTree<K, V>::delkey(char key[]) {
    //Удаление ключа
    //При отсутствии ключа - исключение out_of_range
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

template <class K, class V>
void PrefixTree<K, V>::delall() {
    //Удаление всех значений
    delete _root;
    count_nodes = 0;
    count_keys = 0;
    max_length = 0;
    _root = new nodeitem;

}

template <class K, class V>
bool PrefixTree<K, V>::isKey(char key[]) {
    //Проверка является ли данный ключ ключом префиксного дерева
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

template <class K, class V>
int PrefixTree<K, V>::getCountKeys() const {
    //Количество ключей
    return count_keys;
}

template <class K, class V>
int PrefixTree<K, V>::getCountNodes() const {
    //Количество узлов
    return count_nodes;
}

template <class K, class V>
int PrefixTree<K, V>::getMaxLength() const {
    //Максимальная длина ключа
    return max_length;
}

template <class K, class V>
PrefixTree<K, V>::PrefixTree() {
    //Конструктор по умолчанию
    count_keys = 0;
    count_nodes = 0;
    max_length = 0;
    _root = new nodeitem;
}

template <class K, class V>
PrefixTree<K, V>::~PrefixTree() {
    //Деструктор
    cout << "Delete Tree" << endl;
    delete _root;
}
#endif // PREFIXTREE_H
