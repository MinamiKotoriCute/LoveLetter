# 情書（Love Letter）
主要是體驗加強學習 QLearning 的演算法，應用在最近和朋友玩過的一款桌遊上面，結果看來起是成功的。

# 功能
#### 1.簡單介面（用來測試遊戲核心）
![a1.png](/github_image/a1.png)  
Player X：第X位玩家  
3 2 9：我手上有 3 號牌，這回合抽到 2 號牌，抽牌區剩餘 9 張牌  
按鈕說明：  
第一個數字代表想打出的牌  
第二個數字代表指定的玩家  
第三個數字代表額外參數（像是使用 1 號牌，第三個數字意思是猜測的手牌）

### 2. QLearning 學習
![a2.png](/github_image/a2.png)  
訓練中沒有視窗，debug會輸出目前進行的遊戲場次，到達指定場次後會停止，出輸出訓練結果至 json.txt 並顯示訓練結果。（訓練場次的參數在 agent.cpp 的 max_count_debug）

### 3. 訓練結果
![a3.png](/github_image/a3.png)  
這是一個訓練結果，外層的 score 和 count 是內層的值直接加總起來的結果，總覺得這樣算不對，之後再來改。

# 參考
遊戲規則：http://merrich999.pixnet.net/blog/post/320689397-%E6%83%85%E6%9B%B8-love-letter
QLearning：http://computing.dcu.ie/~HUMPHRYS/Notes/RL/Code/index.html