#include <iostream>
#include <QVector>
#include <assert.h>
#include "PrefixTree.h"

using namespace std;

void testDelete() {
    //Тестирование удаления
    PrefixTree<QVector<QString>, int> tree;
    QVector<QString> my_vect1 = {"Red", "Yellow", "Green", "Blue"};
    QVector<QString> my_vect2 = {"Red", "Yellow", "Orange", "White", "Green"};
    char value = 'F';
    tree.add(my_vect1, value);
    tree.add(my_vect2, value);
    assert(tree.getCountKeys() == 2);
    tree.delkey(my_vect1);
    assert(tree.getCountKeys() == 1); //Тест 5а
    try {
        tree.delkey(my_vect1);
        assert(false);
    }  catch (out_of_range) {
        assert(true);
        assert(tree.getCountKeys() == 1); //Тест 5б
    }
    assert(!tree.isKey(my_vect1)); //Тест 6
    assert(tree.isKey(my_vect2));
    assert(tree.getCountNodes() == 5);
}

void testDeleteOne() {
    //Тестирование удаления единственного ключа
    PrefixTree<QVector<QString>, int> tree;
    QVector<QString> my_vect = {"Yellow", "Green", "Red"};
    char value = 'X';
    tree.add(my_vect, value);
    tree.delkey(my_vect);
    assert(!tree.isKey(my_vect));
    try {
        tree[my_vect];
        assert(false);
    }  catch (out_of_range) {
        assert(true);
    }
    assert(tree.getCountNodes() == 0);
}

void testDeletePartDelete() {
    //Тестирование частичного удаления
    PrefixTree<QVector<QString>, int> tree;
    QVector<QString> my_vect1 = {"Red", "Yellow", "Green", "Blue"};
    QVector<QString> my_vect2 = {"Red", "Yellow", "Green"};
    char value = 'E';
    tree.add(my_vect1, value);
    tree.add(my_vect2, value);
    tree.delkey(my_vect1);
    assert(!tree.isKey(my_vect1));
    assert(tree.isKey(my_vect2));
    assert(tree.getCountNodes() == 3);
}

void testDeleteNoDelete() {
    //Тестирование невозможности удаления
    PrefixTree<QVector<QString>, int> tree;
    QVector<QString> my_vect1 = {"Red", "Yellow", "Green", "Blue"};
    QVector<QString> my_vect2 = {"Red", "Yellow", "Green"};
    char value = 'E';
    tree.add(my_vect1, value);
    tree.add(my_vect2, value);
    tree.delkey(my_vect2);
    assert(!tree.isKey(my_vect2));
    assert(tree.isKey(my_vect1));
    assert(tree.getCountNodes() == 4);
}

void testAddOne() {
    //Тестирование добавления одного ключа
    PrefixTree<QVector<QString>, char> tree;
    QVector<QString> my_vect = {"Red", "Yellow", "Green"};
    char value = 'L';
    assert(tree.getCountKeys() == 0); //Тест 1
    tree.add(my_vect, value);
    assert(tree.getCountKeys() == 1); //Тест 2а
    tree.add(my_vect, value);
    assert(tree.getCountKeys() == 1); //Тест 2б
    assert(tree.isKey(my_vect)); //Тест 3
    assert(tree[my_vect] == value); //Тест 4
    assert(tree.getCountNodes() == 3);
}

void testAddLot() {
    //Тестирование добавления нескольких ключей
    PrefixTree<QVector<QString>, char> tree;
    QVector<QString> my_vect1 = {"Red", "Yellow", "Green", "Blue"};
    QVector<QString> my_vect2 = {"Red", "Yellow", "Green"};
    QVector<QString> my_vect3 = {"Red", "Yellow", "Blue", "Blue"};
    QVector<QString> my_vect4 = {"White", "Green", "Blue", "Blue"};
    char value1 = 'A';
    char value2 = 'B';
    char value4 = 'C';
    char value3 = 'D';
    tree.add(my_vect1, value1);
    assert(tree.isKey(my_vect1));
    assert(tree[my_vect1] == value1);
    assert(tree.getCountNodes() == 4);
    tree.add(my_vect2, value2);
    assert(tree.isKey(my_vect2));
    assert(tree[my_vect2] == value2);
    assert(tree.getCountNodes() == 4);
    tree.add(my_vect3, value3);
    assert(tree.isKey(my_vect3));
    assert(tree[my_vect3] == value3);
    assert(tree.getCountNodes() == 6);
    tree.add(my_vect4, value4);
    assert(tree.isKey(my_vect4));
    assert(tree[my_vect4] == value4);
    assert(tree.getCountNodes() == 10);
}

void testDeleteAll() {
    //Тестирование полного удаления
    PrefixTree<QVector<QString>, char> tree1;
    PrefixTree<QVector<QString>, char> tree2;
    QVector<QString> my_vect1 = {"Red", "Yellow", "Green", "Blue"};
    QVector<QString> my_vect2 = {"Red", "Yellow", "Green"};
    tree1.add(my_vect1, 'L');
    tree1.add(my_vect2, 'M');
    tree1.delall();
    assert(tree1 == tree2); //Тест 7
};

void testCopyConstructor() {
    //Тестирование конструктора копирования
    PrefixTree<QVector<QString>, char> tree;
    QVector<QString> my_vect1 = {"Red", "Yellow", "Green", "Blue"};
    QVector<QString> my_vect2 = {"Red", "Yellow", "Green"};
    tree.add(my_vect1, 'L');
    tree.add(my_vect2, 'M');
    PrefixTree<QVector<QString>, char> copytree(tree);
    assert(copytree == tree); //Тест 8а
    copytree.add(my_vect1, 'K');
    assert(!(copytree == tree)); //Тест 8б
}

void testPrefixTreeToFile() {
    //Тестирование записи в файл
    PrefixTree<QVector<QString>, char> tree;
    QVector<QString> my_vect1 = {"Red", "Yellow", "Green", "Blue"};
    QVector<QString> my_vect2 = {"Red", "Yellow", "Green"};
    tree.add(my_vect1, 'L');
    tree.add(my_vect2, 'M');
    if (!tree.toFile("1.lab2"))
        assert(false);
    PrefixTree<QVector<QString>, char> newtree;
    if (!newtree.fromFile("1.lab2"))
        assert(false);
    assert(tree == newtree);
}

void test() {
    //Тестирование
    testAddOne(); //Тесты 1,2,3,4
    testAddLot();
    testDelete(); //Тесты 5,6
    testDeleteOne();
    testDeletePartDelete();
    testDeleteNoDelete();
    testDeleteAll(); //Тест 7
    testCopyConstructor(); //Тест 8
    testPrefixTreeToFile(); //Тест 9
    cout << "All is OK" << endl;
}

int main()
{
    test();
    return 0;
}
