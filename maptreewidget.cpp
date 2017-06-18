#include "maptreewidget.h"

#include <QVBoxLayout>

MapTreeWidget::MapTreeWidget(QWidget *parent) : QDialog(parent)
{
    treeWidget = new QTreeWidget;

    QVBoxLayout *vb = new QVBoxLayout;
    vb->addWidget(treeWidget);
    setLayout(vb);

    setMinimumWidth(400);
}

void MapTreeWidget::setData(qln::qln4::StateActionSpace sas)
{
    treeWidget->clear();

    treeWidget->setHeaderLabels(QStringList({"State / Action", "score"}));

    for(const std::pair<const qln::Index, qln::WeightSpace> &pair : sas){
        QTreeWidgetItem *parentItem = new QTreeWidgetItem({index2QString(pair.first)});
        double total = 0;
        for(const std::pair<const qln::Index, double> &pairChild : pair.second){
            total+=pairChild.second;
            parentItem->addChild(new QTreeWidgetItem({index2QString(pairChild.first), QString::number(pairChild.second)}));
        }
        parentItem->setText(1, QString::number(total));
        treeWidget->addTopLevelItem(parentItem);
    }
}

void MapTreeWidget::setData(QJsonObject jsonObject)
{
    treeWidget->clear();

    treeWidget->setHeaderLabels(QStringList({"HandCard / LastCard / Action", "Score", "Count"}));
    treeWidget->setColumnWidth(0, 200);

    for(const QString handCard : jsonObject.keys()){
        QJsonObject handCardJsonObject = jsonObject[handCard].toObject();
        QTreeWidgetItem *handCardItem = new QTreeWidgetItem({handCard, handCardJsonObject["Score"].toString(), handCardJsonObject["Count"].toString()});
        for(const QString lastCard : handCardJsonObject.keys()){
            if(lastCard == "Score" || lastCard == "Count")
                continue;
            QJsonObject lastCardJsonObject = handCardJsonObject[lastCard].toObject();

            QTreeWidgetItem *lastCardItem = new QTreeWidgetItem({lastCard, lastCardJsonObject["Score"].toString(), lastCardJsonObject["Count"].toString()});
            for(const QString action : lastCardJsonObject.keys()){
                if(action == "Score" || action == "Count")
                    continue;
                QJsonObject info = lastCardJsonObject[action].toObject();
                QString score = info["Score"].toString();
                QString count = info["Count"].toString();
                lastCardItem->addChild(new QTreeWidgetItem({action, score, count}));
            }
            handCardItem->addChild(lastCardItem);
        }

        treeWidget->addTopLevelItem(handCardItem);
    }
}

QString MapTreeWidget::index2QString(qln::qln4::Index index)
{
    QString s;
    for(int i : index){
        s+=QString::number(i) + " ";
    }
    if(!s.isEmpty()){
        s.remove(s.size()-1,1);
    }
    return s;
}
