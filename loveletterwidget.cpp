#include "loveletterwidget.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>

#include "lovelettergame.h"

LoveLetterWidget::LoveLetterWidget(QWidget *parent) : QWidget(parent), game(new LoveLetterGame)
{
    _playerNumber = 4;
    textEdit = new QTextEdit;

    QHBoxLayout *hb = new QHBoxLayout;
    for(int i=0;i<_playerNumber;i++){
        QGroupBox *groupBox = new QGroupBox(QString::asprintf("Player %d", i));

        isPlayerDead << false;
        groups << groupBox;
        hb->addWidget(groupBox);
    }

    hb->addWidget(textEdit);
    setLayout(hb);

    connect(game, &LoveLetterGame::notification, [=](QVector<int> state, QVector<QVector<int>> actions){

        for(QGroupBox *groupBox : groups){
            for(QObject *const object : groupBox->children()){
                object->deleteLater();
            }

            QLayout *layout = groupBox->layout();
            if(layout != 0){
                QLayoutItem *child;
                while ((child = layout->takeAt(0)) != 0) {
                    delete child;
                }
                delete layout;
            }
        }

        QString s;
        QVBoxLayout *vb = new QVBoxLayout;
        for(int i=0;i<state.size();i++){
            s += QString::asprintf("%d ", state.at(i));
        }
        vb->addWidget(new QLabel(s));
        textEdit->append("State: " + s);
        for(int i=0;i<actions.size();i++){
            QString s;
            for(int j=0;j<actions.at(i).size();j++){
                int small_action = actions.at(i).at(j);
                if(j==1)
                    small_action = id2realId(small_action);
                s += QString::asprintf("%d ", small_action);
            }
            QPushButton *button = new QPushButton(s);
            connect(button, &QPushButton::clicked, [=]{
                QVector<int> action;
                QStringList s = button->text().split(' ');
                for(int j=0;j<s.size()-1;j++){
                    int small_action = s.at(j).toInt();
                    if(j==1)
                        small_action = realId2id(small_action);
                    action.push_back(small_action);
                }
                textEdit->append(QString::asprintf("player: %d, Action: ", id2realId(game->turn())) + button->text());
                send(action);
            });
            vb->addWidget(button);
        }
        vb->addStretch(1);
        groups[id2realId(game->turn())]->setLayout(vb);
    });

    connect(game, &LoveLetterGame::gameOver, [=](int winner){
        textEdit->append(QString::asprintf("winner: %d", id2realId(winner)));
        for(bool &isDead : isPlayerDead){
            isDead = false;
        }
    });

    connect(game, &LoveLetterGame::playerDead, [=](int playerId){
        textEdit->append(QString::asprintf("dead: %d", id2realId(playerId)));
        isPlayerDead[id2realId(playerId)] = true;
    });

    game->setPlayerNumber(_playerNumber);
    game->prepare();

}

void LoveLetterWidget::send(QVector<int> action)
{
    QTimer::singleShot(0, [=]{
        game->executeAction(action);
    });
}

int LoveLetterWidget::realId2id(int realId)
{
    for(int i=realId;i>=0;i--){
        if(isPlayerDead[i]){
            --realId;
        }
    }
    return realId;
}

int LoveLetterWidget::id2realId(int id)
{
    for(int i=0;i<=id && i<isPlayerDead.size();i++){
        if(isPlayerDead[i]){
            ++id;
        }
    }
    return id;
}
