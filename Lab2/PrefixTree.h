#ifndef PREFIXTREE_H
#define PREFIXTREE_H

#include <map>
#include <QString>
#include <QFile>
#include <QVector>
#include <QDataStream>

using namespace std;

template <class K, class V>
class PrefixTree {
public:
    PrefixTree();
    PrefixTree(PrefixTree& tree);
    void add(const K& key, const V& value);
    QVector<K> getKeys(const K& key);
    V& operator[](const K& key) const;
    bool operator==(const PrefixTree& tree);
    void delkey(const K& key);
    void delall();
    bool isKey(const K& key);
    int getCountKeys() const;
    int getCountNodes() const;
    int getMaxLength() const;
    bool toFile(const QString &filename);
    bool fromFile(const QString &filename);
    ~PrefixTree();
private:
    class nodeitem {
        //Узел префиксного дерева
    public:
        nodeitem() {
            isvalue = false;
            parent = NULL;
        }
        void add(const K key) {
            items[key] = new nodeitem;
            items[key]->parent = this;
                           }
        ~nodeitem() {
            typename map<K, nodeitem*>::iterator it, end;
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
        map<K, nodeitem*> items;
    };
    void addkey(typename PrefixTree<K, V>::nodeitem* root, K* key, const int i, QVector<K>* vect);
    int count_nodes; //Количество узлов
    int count_keys; //Количество ключей
    int max_length; //Максимальная длина ключа
    nodeitem* _root;
};

template <class K, class V>
void PrefixTree<K, V>::add(const K& key, const V& value) {
    //Добавление пары ключ-значение в префиксное дерево
    int i=0;
    nodeitem* t = _root;
    while (i<(int)key.size()) {
        K new_key = {key[i]};
        if (t->items.count(new_key) == 0) {
            t->add(new_key);
            count_nodes++;
        }
        t = t->items.at(new_key);
        i++;
    }
    if (t->isvalue)
        count_keys--;
    t->value = value;
    t->isvalue = true;
    if (max_length < i) max_length = i;
    count_keys++;
}

template<class K, class V>
QVector<K> PrefixTree<K, V>::getKeys(const K &key) {
    //Получение вектора ключей, начинающихся с заданной последовательности
    QVector<K> vect;
    int i=0;
    typename PrefixTree<K, V>::nodeitem* t = _root;
    K* keyitem = new K;
    while (i<(int)key.size()) {
        map<K, typename PrefixTree<K, V>::nodeitem*> dict = t->items;
        K new_key = {key[i]};
        if (t->items.count(new_key)) {
            t = dict[new_key];
            keyitem->push_back(key[i]);
        }
        else
            return vect;
        i++;
    }
    addkey(t, keyitem, i, &vect);
    delete keyitem;
    return vect;
}



template <class K, class V>
V& PrefixTree<K, V>::operator[](const K& key) const {
    //Перегрузка [] - получение значения по ключу
    //При отсутствии ключа - исключение out_of_range
    int i=0;
    nodeitem* t = _root;
    while (i<(int)key.size()) {
        map<K, nodeitem*> dict = t->items;
        K new_key = {key[i]};
        if (t->items.count(new_key))
            t = dict[new_key];
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
void PrefixTree<K, V>::delkey(const K& key) {
    //Удаление ключа
    //При отсутствии ключа - исключение out_of_range
    if (isKey(key)) {
        int i=0;
        nodeitem* t = _root;
        while (i<(int)key.size()) {
            map<K, nodeitem*> dict = t->items;
            K new_key = {key[i]};
            t = dict[new_key];
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
            K new_key = {key[i]};
            t->parent->items.erase(new_key);
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
bool PrefixTree<K, V>::isKey(const K& key) {
    //Проверка является ли данный ключ ключом префиксного дерева
    int i=0;
    nodeitem* t = _root;
    while (i<(int)key.size()) {
        map<K, nodeitem*> dict = t->items;
        K new_key = {key[i]};
        if (t->items.count(new_key))
            t = dict[new_key];
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

template<class K, class V>
bool PrefixTree<K, V>::toFile(const QString &filename)
{
    if (!filename.endsWith(".lab2")) return false;
    QFile file(filename);
    if (file.open(QFile::WriteOnly)) {
        K empty;
        QVector<K> keys = getKeys(empty);
        int i = 0;
        QDataStream out(&file);
        out << QString("Chashkin oop lab2 miem");
        out << keys;
        V values[keys.size()];
        while (i < (int)keys.size()) {
            values[i] = (*this)[keys[i]];
            i++;
        }
        out.writeRawData((char*)values, sizeof(values));
        file.close();
        return true;
    }
    else
        return false;
}

template<class K, class V>
bool PrefixTree<K, V>::fromFile(const QString &filename)
{
    if (!filename.endsWith(".lab2")) return false;
    QFile file(filename);
    if (file.open(QFile::ReadOnly)) {
        QDataStream in(&file);
        QString secret;
        in >> secret;
        if (secret != "Chashkin oop lab2 miem") {
            file.close();
            return false;
        }
        delall();
        QVector<K> keys;
        in >> keys;
        V values[keys.size()];
        in.readRawData((char*)values, sizeof(values));
        for (int i = 0; i<keys.size(); i++)
            add(keys[i], values[i]);
        file.close();
        return true;
    }
    else
        return false;
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
PrefixTree<K, V>::PrefixTree(PrefixTree &tree) {
    //Конструктор копирования
    K empty;
    QVector<K> vect = tree.getKeys(empty);
    count_keys = 0;
    count_nodes = 0;
    max_length = 0;
    _root = new nodeitem;
    int i = 0;
    while (i < (int)vect.size()) {
        add(vect[i], tree[vect[i]]);
        i++;
    }
}

template <class K, class V>
bool PrefixTree<K, V>::operator==(const PrefixTree &tree)
{
    //Перегрузка оператора == равенство 2 деревьев
    K empty;
    QVector<K> vect1 = getKeys(empty);
    QVector<K> vect2 = getKeys(empty);
    int i = 0;
    while (i < (int)vect1.size()) {
        try {
            if ((*this)[vect1[i]] != tree[vect1[i]])
                return false;
        }  catch (out_of_range) {
            return false;
        }
        i++;
    }
    i = 0;
    while (i < (int)vect2.size()) {
        try {
            if ((*this)[vect2[i]] != tree[vect2[i]])
                return false;
        }  catch (out_of_range) {
            return false;
        }
        i++;
    }
    return true;
}

template <class K, class V>
PrefixTree<K, V>::~PrefixTree() {
    //Деструктор
    delete _root;
}

template<class K, class V>
void PrefixTree<K, V>::addkey(typename PrefixTree<K, V>::nodeitem* root, K* key, const int i, QVector<K>* vect) {
    //Добавление ключа в вектор
    if (root->isvalue) {
        vect->push_back(*key);
    }
    typename map<K, typename PrefixTree<K, V>::nodeitem*>::iterator it, end;
    it = root->items.begin();
    end = root->items.end();
    while (it != end) {
        key->push_back((it->first)[0]);
        addkey(it->second, key, i+1, vect);
        key->pop_back();
        it++;
    }
}
#endif // PREFIXTREE_H
