#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <PrefixTree.h>

typedef QVector<QString> K;
typedef QString V;
typedef PrefixTree<QVector<QString>, QString> ColorTree;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addkeyitem_clicked();

    void on_keylist_itemSelectionChanged();

    void on_deletekeyitem_clicked();

    void on_clearkeylist_clicked();

    void on_program_info_triggered();

    void on_addkeyvalue_clicked();

    void on_cleartree_clicked();

    void on_treeWidget_itemSelectionChanged();

    void on_delkey_clicked();

    void on_getkeys_clicked();

    void on_prev_clicked();

    void on_next_clicked();

    void on_save_triggered();

    void on_open_triggered();

private:
    QStringList colorlist_english = {"Red", "Orange", "Yellow", "Green", "Cyan", "Blue", "Magenta"};
    QStringList colorlist_local;
    QVector<K>* vect = NULL;
    int position = 0;
    Ui::MainWindow *ui;
    ColorTree tree;
    void drawtree();
    void drawkey(K& key);
    void noiterator();
};

#endif // MAINWINDOW_H
