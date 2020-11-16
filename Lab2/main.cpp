#include <iostream>
#include <map>
#include <assert.h>
#include "PrefixTree.h"
#include "PrefixTreeIterator.h"

using namespace std;

void testDelete() {
    //Тестирование удаления
    PrefixTree<int, int> tree;
    char key1[] = "Leonid";
    char key2[] = "Leopold";
    int value = 2001;
    tree.add(key1, value);
    tree.add(key2, value);
    assert(tree.getCountKeys() == 2);
    tree.delkey(key1);
    assert(tree.getCountKeys() == 1); //Тест 5а
    try {
        tree.delkey(key1);
        assert(false);
    }  catch (out_of_range) {
        assert(true);
        assert(tree.getCountKeys() == 1); //Тест 5б
    }
    assert(!tree.isKey(key1)); //Тест 6
    assert(tree.isKey(key2));
    assert(tree.getCountNodes() == 7);
}

void testDeleteOne() {
    //Тестирование удаления единственного ключа
    PrefixTree<int, int> tree;
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
    //Тестирование частичного удаления
    PrefixTree<int, int> tree;
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
    //Тестирование невозможности удаления
    PrefixTree<int, int> tree;
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
    //Тестирование добавления одного ключа
    PrefixTree<int, int> tree;
    char key[] = "Leonid";
    int value = 2001;
    assert(tree.getCountKeys() == 0); //Тест 1
    tree.add(key, value);
    assert(tree.getCountKeys() == 1); //Тест 2а
    tree.add(key, value);
    assert(tree.getCountKeys() == 1); //Тест 2б
    assert(tree.isKey(key)); //Тест 3
    assert(tree[key] == value); //Тест 4
    assert(tree.getCountNodes() == 6);
}

void testAddLot() {
    //Тестирование добавления нескольких ключей
    PrefixTree<int, int> tree;
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

bool isEqual(char s1[], char s2[]) {
    //Проверка на равенство двух векторов
    char* t1 = s1;
    char* t2 = s2;
    while (*t1) {
        if (*t1 != *t2)
            return false;
        t1++;
        t2++;
    };
    if (*t1 != *t2)
        return false;
    return true;
};

void testPrefixIterator() {
    //Тестирование итератора
    PrefixTree<int, int> tree;
    char s1[] = "Leonid";
    char s2[] = "Leopold";
    char s3[] = "Leofed";
    char s4[] = "Leonu";
    tree.add(s1, 10);
    tree.add(s2, 11);
    tree.add(s3, 10);
    tree.add(s4, 11);
    PrefixTreeIterator<int, int> iter3 = tree.getKeys((char *)"K");
    assert(!*iter3);
    PrefixTreeIterator<int, int> iter2 = tree.getKeys(s3);
    assert(isEqual(s3, *iter2));
    iter2++;
    assert(!*iter2);
    s1[0] = 0;
    PrefixTreeIterator<int, int> iter = tree.getKeys(s1);
    assert(isEqual(s3, *iter));
    iter++;
    s1[0] = 'L';
    assert(isEqual(s1, *iter));
    iter++;
    assert(isEqual(s4, *iter));
    iter++;
    assert(isEqual(s2, *iter));
    iter++;
    assert(!*iter);
}

void testDeleteAll() {
    //Тестирование полного удаления
    PrefixTree<int, int> tree1;
    PrefixTree<int, int> tree2;
    tree1.add("Leonid", 2001);
    tree1.add("Leo", 2001);
    tree1.delall();
    assert(tree1 == tree2); //Тест 7
};

void testCopyConstructor() {
    //Тестирование конструктора копирования
    PrefixTree<int, int> tree;
    tree.add("Leonid", 2001);
    tree.add("Leo", 2002);
    PrefixTree<int, int> copytree(tree);
    assert(copytree == tree); //Тест 8а
    copytree.add("Leo", 2003);
    assert(!(copytree == tree)); //Тест 8б
}
/*
void testPrefixTreeToJSON() {
    PrefixTree tree;
    tree.add("Leonid", 2001);
    tree.add("Leo", 2002);
    PrefixTreeToJSON("tree.json", tree);
    PrefixTreeFromJSON("tree.json");
}
*/
void test() {
    testAddOne(); //Тесты 1,2,3,4
    testAddLot();
    testDelete(); //Тесты 5,6
    testDeleteOne();
    testDeletePartDelete();
    testDeleteNoDelete();
    testDeleteAll(); //Тест 7
    testCopyConstructor(); //Тест 8
    testPrefixIterator();
    //testPrefixTreeToJSON(); //Тест 9
    cout << "All is OK";
}

int main()
{
    test();
    return 0;
}
