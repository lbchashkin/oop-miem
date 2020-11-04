#include <iostream>
#include <map>
#include <assert.h>
#include <stdio.h>
using namespace std;

class nodeitem {
public:
    nodeitem() {cout << "New nodeitem" << endl;}
    void add(char key) {
        items[key] = new nodeitem;
        items[key]->parent = this;
                       }
    ~nodeitem() {
        cout << "Delete nodeitem" << endl;
        map<char, nodeitem*>::iterator it, end;
        it = items.begin();
        end = items.end();
        while (it != end) {
            delete it->second;
            it++;
        }
    }
    int value;
    bool isvalue = false;
    nodeitem* parent = NULL;
    map<char, nodeitem*> items;
};


class PrefixTree {
public:
    PrefixTree() {cout << "New PrefixTree" << endl; _root = new nodeitem; count_keys = 0; count_nodes = 0;}
    void add(char key[], int value) {
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
        count_keys++;
    }
    int& operator[](char key[]) {
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
    void delkey(char key[]) {
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
    void delall() {
        delete _root;
        count_nodes = 0;
        count_keys = 0;
        _root = new nodeitem;

    }
    bool isKey(char key[]) {
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
    int getCountKeys() {return count_keys;}
    int getCountNodes() {return count_nodes;}
    ~PrefixTree() {cout << "Delete Tree" << endl; delete _root;}
private:
    int count_nodes;
    int count_keys;
    nodeitem* _root;
};


void testDelete() {
    PrefixTree tree;
    char key1[] = "Leonid";
    char key2[] = "Leopold";
    int value = 2001;
    tree.add(key1, value);
    tree.add(key2, value);
    assert(tree.getCountKeys() == 2);
    tree.delkey(key1);
    assert(tree.getCountKeys() == 1);
    try {
        tree.delkey(key1);
        assert(false);
    }  catch (out_of_range) {
        assert(true);
        assert(tree.getCountKeys() == 1);
    }
    assert(!tree.isKey(key1));
    assert(tree.isKey(key2));
    assert(tree.getCountNodes() == 7);
}

void testDeleteAll() {
    PrefixTree tree;
    char key[] = "Leonid";
    int value = 2001;
    tree.add(key, value);
    tree.delkey(key);
    assert(!tree.isKey(key));
    try {
        tree[key];
        assert(false);
    }  catch (out_of_range) {
        assert(true);
    }
    assert(tree.getCountNodes() == 0);
}

void testDeletePartDelete() {
    PrefixTree tree;
    char key1[] = "Leonid";
    char key2[] = "Leo";
    int value = 2001;
    tree.add(key1, value);
    tree.add(key2, value);
    tree.delkey(key1);
    assert(!tree.isKey(key1));
    assert(tree.isKey(key2));
    assert(tree.getCountNodes() == 3);
}

void testDeleteNoDelete() {
    PrefixTree tree;
    char key1[] = "Leonid";
    char key2[] = "Leo";
    int value = 2001;
    tree.add(key1, value);
    tree.add(key2, value);
    tree.delkey(key2);
    assert(!tree.isKey(key2));
    assert(tree.isKey(key1));
    assert(tree.getCountNodes() == 6);
}

void testAddOne() {
    PrefixTree tree;
    char key[] = "Leonid";
    int value = 2001;
    assert(tree.getCountKeys() == 0);
    tree.add(key, value);
    assert(tree.getCountKeys() == 1);
    tree.add(key, value);
    assert(tree.getCountKeys() == 1);
    assert(tree.isKey(key));
    assert(tree[key] == value);
    assert(tree.getCountNodes() == 6);
}

void testAddLot() {
    PrefixTree tree;
    char key1[] = "Leonid";
    char key2[] = "Leo";
    char key3[] = "Lamp";
    char key4[] = "Cupivan";
    int value1 = 2001;
    int value2 = 2019;
    int value4 = 1985;
    int value3 = 2020;
    tree.add(key1, value1);
    assert(tree.isKey(key1));
    assert(tree[key1] == value1);
    assert(tree.getCountNodes() == 6);
    tree.add(key2, value2);
    assert(tree.isKey(key2));
    assert(tree[key2] == value2);
    assert(tree.getCountNodes() == 6);
    tree.add(key3, value3);
    assert(tree.isKey(key3));
    assert(tree[key3] == value3);
    assert(tree.getCountNodes() == 9);
    tree.add(key4, value4);
    assert(tree.isKey(key4));
    assert(tree[key4] == value4);
    assert(tree.getCountNodes() == 16);
}

void test() {
    testAddOne();
    testAddLot();
    testDelete();
    testDeleteAll();
    testDeletePartDelete();
    testDeleteNoDelete();
    cout << "All is OK";
}

int main()
{
    test();
    return 0;
}
