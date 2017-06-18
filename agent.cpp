#include "agent.h"

#include <QVector>

#define MYID 0

Agent::Agent(QObject *parent) : QObject(parent), game(new LoveLetterGame), qlearning(new qln::QLearning)
{
    connect(game, &LoveLetterGame::gameOver, [=](int winner){
        double reward = -1;
        if(winner == MYID)
            reward = 1;
        qlearning->learn(lastState, lastAction, getState(), reward);

    });
    connect(game, &LoveLetterGame::playerDead, [=](int playerId){

    });
    connect(game, &LoveLetterGame::notification, [=](QVector<int> state, QVector<QVector<int>> actions){

    });

    game->setPlayerNumber(2);
    game->prepare();
}

qln::State Agent::getState()
{
    return game->currentState().toStdVector();
}

qln::qln4::Action Agent::getAction()
{
    return {};
}
