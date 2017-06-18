#include "lovelettergame.h"

#include <QDebug>

LoveLetterGame::LoveLetterGame(QObject *parent) : QObject(parent)
{

}

void LoveLetterGame::executeAction(QVector<int> action)
{
    int useCard = action.at(0);
    int targetPlayer = action.size() >= 2 ? action.at(1) : -1;
    if(_handCards[_turn] == useCard){
        qSwap(_handCards[_turn], _extraHandCard);
    }

    switch (useCard) {
    case 1:
        if(targetPlayer == -1)
            break;
        if(_handCards.at(targetPlayer) == action.at(2)){
            playerDying(targetPlayer);
        }
        break;
    case 2:
        break;
    case 3:
        if(targetPlayer == -1)
            break;
        if(_handCards.at(targetPlayer) > _handCards.at(_turn)){
            playerDying(_turn);
        }
        else if(_handCards.at(targetPlayer) < _handCards.at(_turn)){
            playerDying(targetPlayer);
        }
        break;
    case 4:
        _isInvincible[_turn] = true;
        break;
    case 5:{
        if(targetPlayer == -1)
            break;
        /// 自定義，背棄牌時若沒牌可抽，判定死亡
        if(_handCards[targetPlayer] == 8 || _cards.isEmpty()){
            playerDying(targetPlayer);
        }
        else{
            _handCards[targetPlayer] = _cards.takeFirst();
        }
        break;
    }
    case 6:
        if(targetPlayer == -1)
            break;
        qSwap(_handCards[_turn], _handCards[targetPlayer]);
        break;
    case 7:
        break;
    case 8:
        playerDying(_turn);
        break;
    default:
        qDebug() << "LoveLetterGame::executeAction";
        break;
    }

    nextRound();
}

void LoveLetterGame::settle()
{
    int maxPlayerId = -1;
    int maxCard = INT_MIN;

    for(int i=0;i<_handCards.size();i++){
        if(_isDead[i] == true)
            continue;
        if(maxCard < _handCards[i]){
            maxCard = _handCards[i];
            maxPlayerId = i;
        }
    }

    emit gameOver(maxPlayerId);
    prepare();
}

int LoveLetterGame::playerNumber() const
{
    return _playerNumber;
}

void LoveLetterGame::setPlayerNumber(int playerNumber)
{
    _playerNumber = playerNumber;
}

void LoveLetterGame::prepare()
{
    _handCards.clear();
    _isInvincible.clear();
    _isDead.clear();
    _alivePlayerNumber = _playerNumber;

    // 洗牌
    _cards = {1,1,1,1,1,2,2,3,3,4,4,5,5,6,7,8};
    for(int i=0;i<_cards.size();i++){
        qSwap(_cards[i], _cards[qrand() % _cards.size()]);
    }

    // 一張牌移出場外
    _cards.removeFirst();

    // 發牌
    for(int i=0;i<_playerNumber;i++){
        _handCards << _cards.takeFirst();
        _isInvincible << false;
        _isDead << false;
    }

    _turn = -1;

    nextRound();
}

void LoveLetterGame::nextRound()
{
    do{
        ++_turn;
        if(_turn == _handCards.size()){
            _turn = 0;
            continue;
        }
    }while(_isDead[_turn] == true);

    if(_isInvincible.at(_turn) == true){
        _isInvincible[_turn] = false;
    }

    if(_cards.size() == 0){
        settle();
        return;
    }
    if(_alivePlayerNumber == 1){
        settle();
        return;
    }
    _extraHandCard = _cards.takeFirst();

    QVector<int> state;
    state.push_back(_handCards[_turn]);
    state.push_back(_extraHandCard);
    state.push_back(_cards.size());

    QVector<QVector<int>> actions;
    if(_handCards[_turn] == 1 || _extraHandCard == 1){
        for(int i=0;i<_handCards.size();i++){
            if(i==_turn)
                continue;
            if(_isDead[i] == true)
                continue;
            if(_isInvincible.at(i) == true)
                continue;
            for(int j=2;j<=8;j++){
                actions.push_back({1,i,j});
            }
        }
    }
    if(_handCards[_turn] == 2 || _extraHandCard == 2){
        for(int i=0;i<_handCards.size();i++){
            if(i==_turn)
                continue;
            if(_isDead[i] == true)
                continue;
            if(_isInvincible.at(i) == true)
                continue;
            actions.push_back({2,i});
        }
    }
    if(_handCards[_turn] == 3 || _extraHandCard == 3){
        for(int i=0;i<_handCards.size();i++){
            if(i==_turn)
                continue;
            if(_isDead[i] == true)
                continue;
            if(_isInvincible.at(i) == true)
                continue;
            actions.push_back({3,i});
        }
    }
    if(_handCards[_turn] == 4 || _extraHandCard == 4){
        actions.push_back({4});
    }
    if(_handCards[_turn] == 7 || _extraHandCard == 7){
        if(_handCards[_turn] == 5 || _extraHandCard == 5 || _handCards[_turn] == 6 || _extraHandCard == 6){
        }
        actions.push_back({7});
    }
    else{
        if(_handCards[_turn] == 5 || _extraHandCard == 5){
            for(int i=0;i<_handCards.size();i++){
                if(_isDead[i] == true)
                    continue;
                if(_isInvincible.at(i) == true)
                    continue;
                actions.push_back({5,i});
            }
        }
        if(_handCards[_turn] == 6 || _extraHandCard == 6){
            for(int i=0;i<_handCards.size();i++){
                if(i==_turn)
                    continue;
                if(_isDead[i] == true)
                    continue;
                if(_isInvincible.at(i) == true)
                    continue;
                actions.push_back({6,i});
            }
        }
    }
    if(_handCards[_turn] == 8 || _extraHandCard == 8){
        actions.push_back({8});
    }

    // 自定義：沒手牌可以出，只能選擇丟棄
    if(actions.size() == 0){
        actions.push_back({_handCards[_turn]});
        actions.push_back({_extraHandCard});
    }

    emit notification(state, actions);
}

void LoveLetterGame::playerDying(int playerId)
{
    _isDead[playerId] = true;
    --_alivePlayerNumber;

    emit playerDead(playerId);
}

QList<bool> LoveLetterGame::isDead() const
{
    return _isDead;
}

int LoveLetterGame::turn() const
{
    return _turn;
}

QVector<int> LoveLetterGame::currentState() const
{
    QVector<int> state;
    state.push_back(_handCards[_turn]);
    state.push_back(_extraHandCard);
    state.push_back(_cards.size());
    return state;
}
