#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

void initData();
int dealCards();
void getWeight(int card,int player);
void printPlayer(int player);
int findKing();
void autoChose(int player);
void printChose();
void printResult(int player);
void gameStart(int player);
int chosePlayer();

/**
 * flag:0代表A为1点，1代表A为11点
 */
struct Player {
    int handCards[6];
    int length;
    int flag;
    int weight;
    int chose;
} players[8];
char cards[][3] = {"A","2","3","4","5","6","7","8","9","10","J","Q","K"};
int cardsNumber[13];
char states[][10] = {"要牌","停牌"};

int main() {
    int player = chosePlayer();
    int chose=0;
    while(!chose) {
		initData();
    	gameStart(player);
    	printf("0继续游戏 1结束游戏\n");
    	scanf("%d",&chose);
	}
    return 0;
}

void initData() {
    int i;
    for(i=0; i<8; ++i) {
        players[i].length=players[i].flag=players[i].weight=players[i].chose=0;
    }
    for(i=0; i<13; ++i) {
        cardsNumber[i]=4;
    }
}
int dealCards() {
    int i,card,flag=0;
    srand(time(NULL));
    //给8个玩家发牌
    for(i=0; i<8; ++i) {
        do {
            //随机抽牌
            card = random()%13;
        } while(cardsNumber[card]==0);
        //牌数减一
        --cardsNumber[card];
        if(players[i].chose==1||players[i].weight>21)continue;
        flag=1;
        players[i].handCards[players[i].length++]=card;
        getWeight(card,i);
    }
    return flag;
}
void getWeight(int card,int player) {
    if(card>8) {
        players[player].weight += 10;
    } else if(card>0) {
        players[player].weight += card+1;
    } else {
        players[player].weight+=11;
        ++players[player].flag;
    }
    if(players[player].weight>21 && players[player].flag>0) {
        --players[player].flag;
        players[player].weight -= 10;
    }
}
void printChose() {
    int i;
    for(i=0; i<8; ++i) {
        printf("%d:",i+1);
        if(players[i].weight>21) {
            printf("%2d点 ",players[i].weight);
        } else
            printf("%s ",states[players[i].chose]);
        if(i==3)printf("\n");
    }
    printf("\n");
}
void printPlayer(int player) {
    int i;
    printf("您的手牌：");
    for(i=0; i<players[player].length; ++i) {
        printf("%s ", cards[players[player].handCards[i]]);
    }
    printf("\n您当前的点数为：%d点\n\n",players[player].weight);
}
void printResult(int player) {
    int i,king;
    printf("\n===========================\n");
    for(i=0; i<8; ++i) {
        printf("%d:%2d点 ",i+1,players[i].weight);
        if(i==3)printf("\n");
    }
    printf("\n\n您所在的位置是 %d:%2d点\n",player+1,players[player].weight);

    king = findKing();
    if (0==king) {
    	printf("全员爆牌！\n");
    }else{
	    printf("本轮的牌王是：");
	    //i做标记变量，标记自己是否是牌王
  		i=0;
	    while(king){
	    	if(player==king%10-1)i=1;
	    	printf("%d号 ", king%10);
	    	king /= 10;
	    }
	    if(i) {
	        printf("\n恭喜您获得胜利！\n");
	    } else printf("\n很遗憾您失败了！\n");
	}

}
int findKing() {
    int king=-1,i,result=0;
    for(i=0; i<8; ++i) {
        if(players[i].weight>21)continue;
        if(king == -1 || players[i].weight > players[king].weight) {
            king=i;
            result = king+1;
        }else if(players[i].weight == players[king].weight){
        	result = result*10 + i+1;
        }
    }
    return result;
}
void autoChose(int player) {
    if(players[player].weight<17 && players[player].length<6) {
        players[player].chose=0;
    } else players[player].chose=1;
}
void gameStart(int player){
	int i;
    printf("==========游戏开始==========\n");
    while(dealCards()) {
        printPlayer(player);
        for(i=0; i<8; ++i) {
            if(i==player)continue;
            autoChose(i);
        }
        if(players[player].weight<21&&players[player].chose==0&&players[player].length<6) {
            printf("请选择：0要牌 1停牌\n");
            scanf("%d",&players[player].chose);
        } else players[player].chose=1;
        printChose();
    }
    printResult(player);
}
int chosePlayer(){
	int player;
    printf("请选择位置1-8\n");
    //验证输入
    while(1) {
        scanf("%d",&player);
        if(player>=1 && player<=8) {
            --player;
            break;
        }
        printf("输入有误，请重新选择1-8\n");
    }
    return player;
}