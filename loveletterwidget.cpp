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
                s += QString::asprintf("%d ", actions.at(i).at(j));
            }
            QPushButton *button = new QPushButton(s);
            connect(button, &QPushButton::clicked, [=]{
                QVector<int> action;
                QStringList s = button->text().split(' ');
                for(int j=0;j<s.size()-1;j++){
                    action.push_back(s.at(j).toInt());
                }
                textEdit->append(QString::asprintf("player: %d, Action: ", game->turn()) + button->text());
                send(action);
            });
            vb->addWidget(button);
        }
        vb->addStretch(1);
        groups[game->turn()]->setLayout(vb);
    });

    connect(game, &LoveLetterGame::gameOver, [=](int winner){
        textEdit->append(QString::asprintf("winner: %d\n", winner));
    });

    connect(game, &LoveLetterGame::playerDead, [=](int playerId){
        textEdit->append(QString::asprintf("dead: %d", playerId));
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
