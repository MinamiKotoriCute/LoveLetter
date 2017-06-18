#include "loveletter.h"

#include <QVector>
#include <QDebug>

#include "Dictionary.h"

LoveLetter::LoveLetter(QObject *parent) : QObject(parent)
{
}

void LoveLetter::executeAction(QVector<int> action)
{
    if(m_playerHandCard.at(m_turn) == action1){
        m_playerHandCard[m_turn] = m_playerNewCard;
    }

    if(m_isPlayerInvincible.at(m_turn)){
        m_isPlayerInvincible[m_turn] = false;
    }

    switch (action1) {
    case 1:
        if(m_playerHandCard.at(action2) == action3){
            playerDying(action2);
        }
        break;
    case 2:
        break;
    case 3:
        if(m_playerHandCard.at(action2) > m_playerHandCard.at(m_turn)){
            playerDying(m_turn);
        }
        else if(m_playerHandCard.at(action2) < m_playerHandCard.at(m_turn)){
            playerDying(action2);
        }
        break;
    case 4:
        m_isPlayerInvincible[m_turn] = true;
        break;
    case 5:
        drawNewCard(action2);
        break;
    case 6:
        swap(m_playerHandCard[m_turn], m_playerHandCard[action2]);
        break;
    case 8:
        playerDying(m_turn);
        break;
    default:
        qDebug() << "error at LoveLetter::playerAction";
        break;
    }
}

void LoveLetter::prepare(int playerNumber)
{
    m_playerNumber = playerNumber;
    m_turn = 0;
    m_isPlayerInvincible.clear();
    for(int i=0;i<m_playerNumber;i++)
        m_isPlayerInvincible.push_back(false);
    shuffleCards();
    dealCards();
    drawNewCard();
    noticePlayer();
}

void LoveLetter::playerAction(int action1, int action2, int action3)
{
    if(m_playerHandCard.at(m_turn) == action1){
        m_playerHandCard[m_turn] = m_playerNewCard;
    }

    if(m_isPlayerInvincible.at(m_turn)){
        m_isPlayerInvincible[m_turn] = false;
    }

    switch (action1) {
    case 1:
        if(m_playerHandCard.at(action2) == action3){
            playerDying(action2);
        }
        break;
    case 2:
        break;
    case 3:
        if(m_playerHandCard.at(action2) > m_playerHandCard.at(m_turn)){
            playerDying(m_turn);
        }
        else if(m_playerHandCard.at(action2) < m_playerHandCard.at(m_turn)){
            playerDying(action2);
        }
        break;
    case 4:
        m_isPlayerInvincible[m_turn] = true;
        break;
    case 5:
        drawNewCard(action2);
        break;
    case 6:
        swap(m_playerHandCard[m_turn], m_playerHandCard[action2]);
        break;
    case 8:
        playerDying(m_turn);
        break;
    default:
        qDebug() << "error at LoveLetter::playerAction";
        break;
    }

    noticePlayer();
}

QVector<int> LoveLetter::playerHandCard() const
{
    return m_playerHandCard;
}

void LoveLetter::shuffleCards()
{
    m_cards = {1,1,1,1,1,
              2,2,
              3,3,
              4,4,
              5,5,
              6,
              7,
              8};

    for(int i=0;i<m_cards.size();i++){
        swap(m_cards[i], m_cards[qrand()%m_cards.size()]);
    }
}

void LoveLetter::dealCards()
{
    m_playerHandCard.clear();
    m_cards.pop_back();
    for(int i=0;i<m_playerNumber;i++){
        m_playerHandCard.push_back(m_cards.takeLast());
    }
}

void LoveLetter::noticePlayer()
{
    QVector<int> canChoosePlayer;
    for(int i=0;i<m_isPlayerInvincible.size();i++){
        if(!m_isPlayerInvincible.at(i)){
            canChoosePlayer.push_back(i);
        }
    }
    if(m_playerHandCard.at(m_turn) == 7 && (m_playerNewCard == 5 || m_playerNewCard == 6) ||
            (m_playerNewCard == 7 && (m_playerHandCard.at(m_turn) == 5 || m_playerHandCard.at(m_turn) == 6))){
        if(m_playerNewCard != 7){
            swap(m_playerNewCard, m_playerHandCard.at(m_turn));
        }

        m_playerNewCard = -1;
    }

    Dictionary<int> cc;
    switch (m_playerHandCard.at(m_turn)) {
    case 1:
        for(int i=0;i<m_playerNumber-1;i++){
            cc.push({0, i, 6}, 0);
        }
        break;
    case 3:
        cc.push({2, canChoosePlayer.size()-1}, 0);
        break;
    case 3:
        cc.push({3}, 0);
        break;
    default:
        break;
    }

    emit playerTurn(m_turn, m_cards.size(), m_playerHandCard.at(m_turn), m_playerNewCard, canChoosePlayer);
}

void LoveLetter::drawNewCard(int playerID)
{
    if(playerID == -1){
        if(m_cards.isEmpty())
            m_playerNewCard = -1;
        else
            m_playerNewCard = m_cards.takeLast();
    }
    else{
        if(m_cards.isEmpty())
            m_playerHandCard[playerID] = -1;
        else
            m_playerHandCard[playerID] = m_cards.takeLast();
    }
}

void LoveLetter::playerDying(int playerID)
{
    m_playerNumber--;
    m_playerHandCard.removeAt(playerID);
    emit playerDied(playerID);
}
