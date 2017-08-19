#include "agent.h"

#include <QVector>
#include <QDebug>
#include <QTimer>
#include <QJsonObject>

#include "commonfunction.h"

static int count_debug = 0;
static const int max_count_debug = 100000;

QString index2QString(qln::Index index)
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

QJsonObject QLearning2QJsonObject(qln::QLearning *qlearning){
    QJsonObject jsonObject;
    for(const std::pair<const qln::Index, qln::WeightSpace> &sas_pair : qlearning->Q){
        const qln::Index &index = sas_pair.first;
        if(index.size() < 3)
            continue;

        qln::Index handCard = index;
        const int lastCard = index[2];
        handCard.pop_back();

        QJsonObject handCardJsonObject = jsonObject[index2QString(handCard)].toObject();
        QJsonObject lastCardJsonObject = handCardJsonObject[QString::number(lastCard)].toObject();

        double handCardTotal = handCardJsonObject["Score"].toString().toDouble();
        int handCardCountTotal = handCardJsonObject["Count"].toString().toInt();
        double lastCardTotal = 0;
        int lastCardCountTotal = 0;
        for(const std::pair<const qln::Index, double> &pairChild : sas_pair.second){
            if(index2QString(pairChild.first) == "1")
                continue;

            lastCardTotal+=pairChild.second;
            lastCardCountTotal+=qlearning->noQ[index][pairChild.first];

            QJsonObject info;
            info["Score"] = QString::number(pairChild.second);
            info["Count"] = QString::number(qlearning->noQ[index][pairChild.first]);
            lastCardJsonObject[index2QString(pairChild.first)] = info;
        }
        lastCardJsonObject["Score"] =  QString::number(lastCardTotal);
        lastCardJsonObject["Count"] =  QString::number(lastCardCountTotal);

        handCardTotal += lastCardTotal;
        handCardCountTotal += lastCardCountTotal;
        handCardJsonObject[QString::number(lastCard)] = lastCardJsonObject;
        handCardJsonObject["Score"] = QString::number(handCardTotal);
        handCardJsonObject["Count"] = QString::number(handCardCountTotal);

        jsonObject[index2QString(handCard)] = handCardJsonObject;
    }

    return jsonObject;
}

Agent::Agent(QObject *parent) : QObject(parent), game(new LoveLetterGame), qlearning(new qln::QLearning)
{
    playerNumber = 4;

    connect(game, &LoveLetterGame::gameOver, [=](int winner){
        if(count_debug > max_count_debug){
            qDebug() << "winner:" << winner << "\n";
            saveJson(QLearning2QJsonObject(qlearning));
            mapTreeWidget.setData(QLearning2QJsonObject(qlearning));
            mapTreeWidget.exec();
        }
        QList<bool> isDead = game->isDead();
        for(int i=0;i<isDead.size();i++){
            if(i == winner){
                qlearning->learn(lastState[i], lastAction[i], getState(), 1);
                continue;
            }
            if(isDead[i] == false){
                qlearning->learn(lastState[i], lastAction[i], {0}, -1);
            }
        }


        for(int i=0;i<playerNumber;i++){
            lastAction[i] = {0};
            lastState[i] = {1};
        }

        qDebug() << "count" << ++count_debug;
    });
    connect(game, &LoveLetterGame::playerDead, [=](int playerId){
        if(count_debug > max_count_debug)
            qDebug() << "dead:" << playerId;

        qlearning->learn(lastState[playerId], lastAction[playerId], {0}, -1);
    });
    connect(game, &LoveLetterGame::notification, [=](QVector<int> state, QVector<QVector<int>> actions){
        std::vector<std::vector<int>> stdActions;
        for(QVector<int> action : actions){
            stdActions.push_back(action.toStdVector());
        }

        qlearning->learn(lastState[game->turn()], lastAction[game->turn()], state.toStdVector(), 0);

        qln::Action chooseAction = count_debug < max_count_debug ? qlearning->randAction(stdActions) : qlearning->explorationAction(state.toStdVector(), stdActions);
        lastAction[game->turn()] = chooseAction;
        lastState[game->turn()] = state.toStdVector();
        if(count_debug > max_count_debug){
            qDebug() << "\n";
            for(std::pair<const qln::Index, double> pair : qlearning->Q[state.toStdVector()]){
                qDebug() << pair.first << pair.second;
            }
            qDebug() << "player:" << game->turn() << "state" << state << "action" << chooseAction;
        }

        send(QVector<int>::fromStdVector(chooseAction));
    });

    for(int i=0;i<playerNumber;i++){
        lastAction.push_back({0});
        lastState.push_back({1});
    }
    game->setPlayerNumber(playerNumber);
    game->prepare();
}

void Agent::send(QVector<int> action)
{
    QTimer::singleShot(0, [=]{
        game->executeAction(action);
    });
}

qln::State Agent::getState()
{
    return game->currentState().toStdVector();
}

void Agent::executeAction()
{

}
