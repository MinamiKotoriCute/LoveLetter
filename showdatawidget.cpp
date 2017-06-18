#include "showdatawidget.h"

#include <QTreeWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QJsonArray>

void listJsonArray(QTreeWidgetItem *parentWidgetItem, QJsonArray &json);
void listJsonObject(QTreeWidgetItem *parentWidgetItem, QJsonObject &json);

ShowDataWidget::ShowDataWidget(QWidget *parent) : QDialog(parent)
{
    treeWidget = new QTreeWidget;

    QVBoxLayout *vb = new QVBoxLayout;
    vb->addWidget(treeWidget);
    setLayout(vb);
}

void listJsonArray(QTreeWidgetItem *parentWidgetItem, QJsonArray &json) {
    for(int i=0;i<json.size();i++){
        QJsonValue value = json.at(i);
        if(value.isArray()){
            QTreeWidgetItem *child = new QTreeWidgetItem(QStringList({QString::number(i+1)}));
            parentWidgetItem->addChild(child);
            listJsonArray(child, value.toArray());
        }
        else if(value.isBool()){
            QTreeWidgetItem *child = new QTreeWidgetItem(QStringList({QString::number(i+1), value.toBool() ? "true" : "false"}));
            parentWidgetItem->addChild(child);
        }
        else if(value.isDouble()){
            QTreeWidgetItem *child = new QTreeWidgetItem(QStringList({QString::number(i+1), QString::number(value.toDouble())}));
            parentWidgetItem->addChild(child);
        }
        else if(value.isObject()){
            QTreeWidgetItem *child = new QTreeWidgetItem(QStringList({QString::number(i+1)}));
            parentWidgetItem->addChild(child);
            listJsonObject(child, value.toObject());
        }
        else if(value.isString()){
            QTreeWidgetItem *child = new QTreeWidgetItem(QStringList({QString::number(i+1), value.toString()}));
            parentWidgetItem->addChild(child);
        }
    }
}

void listJsonObject(QTreeWidgetItem *parentWidgetItem, QJsonObject &json) {
    const QStringList keysList = json.keys();

    for(const QString key : keysList){
        const QJsonValue value = json.value(key);
        if(value.isArray()){
            QTreeWidgetItem *child = new QTreeWidgetItem(QStringList({key}));
            parentWidgetItem->addChild(child);
            listJsonArray(child, value.toArray());
        }
        else if(value.isBool()){
            QTreeWidgetItem *child = new QTreeWidgetItem(QStringList({key, value.toBool() ? "true" : "false"}));
            parentWidgetItem->addChild(child);
        }
        else if(value.isDouble()){
            QTreeWidgetItem *child = new QTreeWidgetItem(QStringList({key, QString::number(value.toDouble())}));
            parentWidgetItem->addChild(child);
        }
        else if(value.isObject()){
            QTreeWidgetItem *child = new QTreeWidgetItem(QStringList({key}));
            parentWidgetItem->addChild(child);
            listJsonObject(child, value.toObject());
        }
        else if(value.isString()){
            QTreeWidgetItem *child = new QTreeWidgetItem(QStringList({key, value.toString()}));
            parentWidgetItem->addChild(child);
        }
    }
}

void ShowDataWidget::setData(QJsonObject json)
{
    treeWidget->clear();

    treeWidget->setHeaderLabels(QStringList({"key", "value"}));
    //progressBar->setRange(0, json.size());

    QTreeWidgetItem *top = new QTreeWidgetItem;
    treeWidget->addTopLevelItem(top);

    listJsonObject(top, json);
    //progressBar->setValue(json.size());
}

void ShowDataWidget::setData(QJsonArray json)
{
    treeWidget->clear();

    treeWidget->setHeaderLabels(QStringList({"key", "value"}));
    //progressBar->setRange(0, json.size());

    QTreeWidgetItem *top = new QTreeWidgetItem;
    treeWidget->addTopLevelItem(top);

    listJsonArray(top, json);
    //progressBar->setValue(json.size());
}
