#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //Инициализация
    ui->setupUi(this);
    int i = 0;
    for (QString colorname: colorlist_english) {
        ui->colorBox->addItem(tr(colorname.toLatin1()));
        ui->colorBox->setItemData(i++, QColor(colorname), Qt::DecorationRole);
        colorlist_local.append(tr(colorname.toLatin1()));
    }
    QString rus("АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ");
    drawtree();
    for (auto letter: rus)
        ui->charbox->insertItem(rus.length(), letter);
    ui->treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->key_count->setVisible(false);
    ui->next->setVisible(false);
    ui->prev->setVisible(false);
}

void MainWindow::drawtree()
{
    //Перерисовка дерева
    ui->treeWidget->clear();
    ui->treeWidget->setColumnCount(2);
    ui->treeWidget->header()->setVisible(true);
    ui->treeWidget->headerItem()->setData(0, 0, "Ключ");
    ui->treeWidget->headerItem()->setData(1, 0, "Значение");
    QTreeWidgetItem* item = new QTreeWidgetItem;
    item->setData(0, 0, "*КОРЕНЬ*");
    item->setData(0, 0x100, 0);
    item->setFlags(Qt::ItemIsEnabled);
    ui->treeWidget->insertTopLevelItem(0, item);
    ui->nodenumber->setText(QString::number(tree.getCountNodes()));
    ui->keynumber->setText(QString::number(tree.getCountKeys()));
    QVector<QString> vect;
    auto keys = tree.getKeys(vect);
    for (auto key: keys) {
        //Добавление ключа
        QTreeWidgetItem* t = item;
        for (int i = 0; i<key.size(); i++) {
            //Добавление элементов ключа
            if (t->text(1).isEmpty())
                t->setFlags(Qt::ItemIsEnabled);
            t->setExpanded(true);
            int child_number = t->childCount();
            for (int j = 0; j<t->childCount(); j++)
                //Поиск среди потомков
                if (t->child(j)->data(0, 0).toString() == key[i]) {
                    child_number = j;
                    break;
                }
            if (child_number == t->childCount()) {
                QTreeWidgetItem* key_item = new QTreeWidgetItem(t);
                key_item->setData(0, 0, key[i]);
                t->addChild(key_item);
            }
            int ind = colorlist_local.indexOf(key[i]);
            if (ind != -1)
                t->child(child_number)->setBackground(0, QColor(colorlist_english[ind]));
            t = t->child(child_number);
        }
        t->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        t->setData(1, 0, tree[key]);
        ui->cleartree->setEnabled(true);
        ui->getkeys->setEnabled(true);
    }
}

MainWindow::~MainWindow()
{
    //Деструктор
    delete ui;
}

void MainWindow::on_addkeyitem_clicked()
{
    //Добавление элемента ключа
    noiterator();
    QString color(colorlist_english[ui->colorBox->currentIndex()]);
    ui->keylist->addItem(ui->colorBox->currentText());
    ui->keylist->item(ui->keylist->count()-1)->setBackground(QColor(color));
    K key;
    for (int i = 0; i < ui->keylist->count(); i++)
        key.push_back(ui->keylist->item(i)->text());
    if (tree.isKey(key)) {
        ui->iskey->setText("<font color='green'>✓</font>");
        ui->charbox->setCurrentText(tree[key]);
    }
    else
        ui->iskey->setText("<font color='red'>✕</font>");
    ui->clearkeylist->setEnabled(true);
    ui->charbox->setEnabled(true);
    ui->addkeyvalue->setEnabled(true);
}

void MainWindow::on_keylist_itemSelectionChanged()
{
    //Выбор элемента ключа
    int count = ui->keylist->selectedItems().size();
    ui->deletekeyitem->setEnabled(count);
}

void MainWindow::on_deletekeyitem_clicked()
{
    //Удаление элемента ключа
    noiterator();
    auto item = ui->keylist->selectedItems()[0];
    ui->keylist->removeItemWidget(item);
    delete item;
    ui->keylist->clearSelection();
    if (!ui->keylist->count())
        on_clearkeylist_clicked();
    K key;
    for (int i = 0; i < ui->keylist->count(); i++)
        key.push_back(ui->keylist->item(i)->text());
    if (tree.isKey(key)) {
        ui->iskey->setText("<font color='green'>✓</font>");
        ui->charbox->setCurrentText(tree[key]);
    }
    else
        ui->iskey->setText("<font color='red'>✕</font>");
}

void MainWindow::on_clearkeylist_clicked()
{
    //Очистить ключ
    noiterator();
    ui->keylist->clear();
    ui->clearkeylist->setEnabled(false);
    ui->charbox->setEnabled(false);
    ui->addkeyvalue->setEnabled(false);
    ui->iskey->setText("<font color='red'>✕</font>");
}

void MainWindow::on_program_info_triggered()
{
        //О программе
        QMessageBox::about(this, "О программе", "<h2>Объектно-ориентированное програмиирование</h2>" \
                                                "<font size=4>Программа для работы с классом Префиксное дерево</font><br><font size=4>Вариант 14</font>" \
                                                "<br><font size=4>Разработчик: Чашкин Леонид, БИВ194</font><p align='center'>МИЭМ НИУ ВШЭ, 2020</p>");
}

void MainWindow::on_addkeyvalue_clicked()
{
    //Добавить пару ключ-значение
    K key;
    for (int i = 0; i < ui->keylist->count(); i++)
        key.push_back(ui->keylist->item(i)->text());
    V value = ui->charbox->currentText();
    tree.add(key, value);
    ui->iskey->setText("<font color='green'>✓</font>");
    ui->cleartree->setEnabled(true);
    ui->getkeys->setEnabled(true);
    drawtree();
}

void MainWindow::on_cleartree_clicked()
{
    //Удаление дерева
    noiterator();
    tree.delAll();
    drawtree();
    ui->iskey->setText("<font color='red'>✕</font>");
    ui->cleartree->setEnabled(false);
    ui->getkeys->setEnabled(false);
}

void MainWindow::on_treeWidget_itemSelectionChanged()
{
    //Выбор ключа в дереве
    int count = ui->treeWidget->selectedItems().size();
    ui->delkey->setEnabled(count);
}

void MainWindow::on_delkey_clicked()
{
    //Удаление выбранного ключа
    noiterator();
    QTreeWidgetItem* item = ui->treeWidget->selectedItems()[0];
    QTreeWidgetItem* t = item;
    QTreeWidgetItem* end = ui->treeWidget->topLevelItem(0);
    K key;
    while (t != end) {
        key.push_front(t->text(0));
        t = t->parent();
    }
    tree.delKey(key);
    drawtree();
    if (!tree.getCountKeys())
        on_cleartree_clicked();
    K key2;
    for (int i = 0; i < ui->keylist->count(); i++)
        key2.push_back(ui->keylist->item(i)->text());
    if (tree.isKey(key2)) {
        ui->iskey->setText("<font color='green'>✓</font>");
        ui->charbox->setCurrentText(tree[key2]);
    }
    else
        ui->iskey->setText("<font color='red'>✕</font>");
}

void MainWindow::on_getkeys_clicked()
{
    K key;
    for (int i = 0; i < ui->keylist->count(); i++)
        key.push_back(ui->keylist->item(i)->text());
    vect = new QVector<K>(tree.getKeys(key));
    ui->key_count->setText(QString("Найдено ключей: %1").arg(vect->size()));
    ui->key_count->setVisible(true);
    if (vect->size()) {
        drawkey((*vect)[0]);
        ui->prev->setVisible(true);
        position = 0;
        ui->key_label->setText("Ключ #1:");
        ui->prev->setEnabled(false);
        ui->next->setVisible(true);
        if (vect->size() == 1)
            ui->next->setEnabled(false);
        else
            ui->next->setEnabled(true);
    }
}

void MainWindow::drawkey(K &key)
{
    //Установка ключа
    on_clearkeylist_clicked();
    auto curr = ui->colorBox->currentText();
    for (int i=0; i<key.size(); i++) {
        ui->colorBox->setCurrentText(key[i]);
        on_addkeyitem_clicked();
    }
    ui->colorBox->setCurrentText(curr);
    ui->prev->setVisible(true);
    ui->next->setVisible(true);
    ui->key_count->setVisible(true);
}

void MainWindow::noiterator() {
    //Удаление кнопок итератора
    ui->prev->setVisible(false);
    ui->next->setVisible(false);
    ui->key_count->setVisible(false);
    ui->key_label->setText("Ключ:");
}

void MainWindow::on_prev_clicked()
{
    //Итератор - предыдщий элемент
    drawkey((*vect)[--position]);
    ui->next->setEnabled(true);
    if (position == 0)
        ui->prev->setEnabled(false);
    ui->key_label->setText(QString("Ключ #%1:").arg(position+1));
}

void MainWindow::on_next_clicked()
{
    //Итератор - следующий элемент
    drawkey((*vect)[++position]);
    ui->prev->setEnabled(true);
    if (position == vect->size() - 1)
        ui->next->setEnabled(false);
    ui->key_label->setText(QString("Ключ #%1:").arg(position+1));
}

void MainWindow::on_save_triggered()
{
    //Сохранение в файл
    QString filename = QFileDialog::getSaveFileName(this, "Save PrefixTree", ".", "LAB2 files (*.lab2)");
        if (not filename.isEmpty()) {
            if (tree.getCountKeys() && filename.endsWith(".lab2")) {
                if (!tree.toFile(filename)) {
                    QMessageBox::warning(this, "Предупреждение", "Что-то пошло не так! "
                                                                 "Коллекция не была сохранена. Проблема с открытием файла. Попробуйте ещё раз.");
                }
            }
            else if (!filename.endsWith(".lab2"))
                QMessageBox::critical(this, "Ошибка", "Неверный формат!");
            else
                QMessageBox::critical(this, "Ошибка", "Дерево пустое!");
        }
}

void MainWindow::on_open_triggered()
{
    //Загрузка из файла
    QString filename = QFileDialog::getOpenFileName(this, "Open PrefixTree", ".", "LAB2 files (*.lab2)");
        if (!filename.isEmpty()) {
            ui->charbox->setCurrentIndex(0);
            ui->colorBox->setCurrentIndex(0);
            on_clearkeylist_clicked();
            on_cleartree_clicked();
            if (!filename.endsWith(".lab2"))
                QMessageBox::critical(this, "Ошибка", "Неверный формат!");
            else if (!tree.fromFile(filename))
                QMessageBox::critical(this, "Ошибка", "Ошибка при чтении данных! Неверно созданный файл");
        }
        drawtree();
}
