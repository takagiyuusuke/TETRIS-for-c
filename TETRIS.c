#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

/*　コピペ用の四角
🟥
🟧
🟨
🟩
🟦
🟪
🟫
⬛
⬜
*/

int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    // 設定を保存
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // バッファ無効化
    newt.c_lflag &= ~(ICANON | ECHO);

    // 設定を適用
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // ファイルディスクリプタのフラグを保存
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    // ファイルディスクリプタを非同期モードに設定
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    // キー入力を読み込む
    ch = getchar();

    // 設定を元に戻す
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    // ファイルディスクリプタのフラグを元に戻す
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

void wait(int milliseconds) {
    clock_t start = clock();
    while ((clock() - start) * 1000 / CLOCKS_PER_SEC < milliseconds) {}
}

void printboard (int array1[23][12], int array2[23][12], int array3[23][12], int random[3], int h, int score){
    int blocks[7][2][4] = {{{0,0,0,0},{7,7,7,7}},{{6,6,6,0},{0,6,0,0}},
                            {{2,2,2,0},{2,0,0,0}},{{5,5,5,0},{0,0,5,0}},
                            {{0,4,4,0},{4,4,0,0}},{{1,1,0,0},{0,1,1,0}},
                            {{0,3,3,0},{0,3,3,0}}};
    printf("\n⬜ ⬜ ⬜ ⬜ ⬜ ⬜ ⬜ ⬜ ⬜ ⬜ ⬜ ⬜      NEXT\n");
    for (int i = 2; i < 22; i ++){
        printf("⬜ ");
        for (int j = 1; j < 11; j++){
            if (array1[i][j] == 1 || array2[i][j] == 1){
                printf("🟥") ;
            } else if (array1[i][j] == 2 || array2[i][j] == 2){
                printf("🟧") ;
            } else if (array1[i][j] == 3 || array2[i][j] == 3){
                printf("🟨") ;
            } else if (array1[i][j] == 4 || array2[i][j] == 4){
                printf("🟩") ;
            } else if (array1[i][j] == 5 || array2[i][j] == 5){
                printf("🟦") ;
            } else if (array1[i][j] == 6 || array2[i][j] == 6){
                printf("🟪") ;
            } else if (array1[i][j] == 7 || array2[i][j] == 7){
                printf("🟫") ;
            } else if (array1[i][j] == 9 || array2[i][j] == 9){
                printf("💥") ;    
            } else  if (array3[i][j] == 8){
                printf("⬛") ;
            } else {
                printf("  ");
            }
            printf(" ");
        }
        printf("⬜  ");
        if (i == 4 || i == 7){
            printf("     ⬆");
        } else if (i <= 9 || (i == 20 || i == 21) && h != -1){
            int k = (i-1) / 3;
            int l = (i-2) % 3;
            printf(" ");
            int target = random[k];
            if (i > 9) target = h;
            for (int m = 0; m < 4; m ++){
                if (blocks[target][l][m] == 1){
                printf("🟥") ;
                } else if (blocks[target][l][m] == 2){
                    printf("🟧") ;
                } else if (blocks[target][l][m] == 3){
                    printf("🟨") ;
                } else if (blocks[target][l][m] == 4){
                    printf("🟩") ;
                } else if (blocks[target][l][m] == 5){
                    printf("🟦") ;
                } else if (blocks[target][l][m] == 6){
                    printf("🟪") ;
                } else if (blocks[target][l][m] == 7){
                    printf("🟫") ;
                } else {
                    printf("  ");
                }
                printf(" ");
            }
        } else if (i == 15){
            printf("   SCORE");
        } else if (i == 16){
            printf("   %d",score);
        } else if (i == 19){
            printf("   HOLD");
        }
        printf("\n");
    }
    printf("⬜ ⬜ ⬜ ⬜ ⬜ ⬜ ⬜ ⬜ ⬜ ⬜ ⬜ ⬜\n");
}

int down (int array1[23][12],int array2[23][12], int info[4]){
    int count = 0;
    for (int i = 0; i <= 21; i ++){
        for (int j = 1; j <= 11; j ++){
            if (array1[i][j] != 0 && array2[i+1][j] == 0){
                count += 1 ;
            }
        }
    }
    if (count < 4){
        return 1;
    } else {
        for (int i = 20; i >= 0; i --){
            for (int j = 1; j < 11; j ++){
                array1[i+1][j] = array1[i][j];
            }
        }
        for (int i = 0; i < 12; i ++){
            array1[0][i] = 0;
        }
        info[1] = info[1] + 1;
        return 0;
    }
}

int right (int array1[23][12], int array2[23][12], int info[4]){
    int count = 0;
    for (int i = 0; i <= 21; i ++){
        for (int j = 1; j <= 11; j ++){
            if (array1[i][j] != 0 && array2[i][j+1] == 0){
                count += 1 ;
            }
        }
    }
    if (count < 4){
        return 1;
    } else {
        for (int i = 9; i >= 0; i --){
            for (int j = 0;j < 22;j ++){
                if (array1[j][i] == 9){
                    array1[j][i+1] = 0;
                } else {
                    array1[j][i+1] = array1[j][i];
                }
            }
        }
        info[2] = info[2] + 1;
        return 0;
    }
}

int left (int array1[23][12], int array2[23][12], int info[4]){
    int count = 0;
    for (int i = 0; i <= 21; i ++){
        for (int j = 1; j <= 11; j ++){
            if (array1[i][j] != 0 && array2[i][j-1] == 0){
                count += 1 ;
            }
        }
    }
    if (count < 4){
        return 1;
    } else {
        for (int i = 2; i <= 11; i ++){
            for (int j = 0;j < 22;j ++){
                if (array1[j][i] == 9){
                    array1[j][i-1] = 0;
                } else {
                    array1[j][i-1] = array1[j][i];
                }
            }
        }
        info[2] = info[2] - 1;
        return 0;
    }
}

void setblock (int array1[23][12], int info[4], int random[3]){
    int n = random[0];
    for (int i = 0; i < 2; i ++){
        random[i] = random[i+1];
    }
    
    random[2] = rand() % 7;
    int blocks[7][2][4] = {{{0,0,0,0},{7,7,7,7}},{{6,6,6,0},{0,6,0,0}},
                            {{2,2,2,0},{2,0,0,0}},{{5,5,5,0},{0,0,5,0}},
                            {{0,4,4,0},{4,4,0,0}},{{1,1,0,0},{0,1,1,0}},
                            {{0,3,3,0},{0,3,3,0}}};
    for (int i = 0; i <= 1; i ++){
        for (int j = 0; j <= 3; j ++){
            array1[i+1][j+4] = blocks[n][i][j];
        }
    }
    info[0] = n;
    info[1] = 1;
    info[2] = 4;
    info[3] = 0;
}

int turn (int array1[23][12], int array2[23][12], int info[4], int direction){
    int turnBlock[7][4][4][4] = {
        {{{0,0,0,0},{7,7,7,7},{0,0,0,0},{0,0,0,0}},{{0,0,7,0},{0,0,7,0},{0,0,7,0},{0,0,7,0}},{{0,0,0,0},{0,0,0,0},{7,7,7,7},{0,0,0,0}},{{0,7,0,0},{0,7,0,0},{0,7,0,0},{0,7,0,0}}},
        {{{0,0,0,0},{6,6,6,0},{0,6,0,0},{0,0,0,0}},{{0,6,0,0},{6,6,0,0},{0,6,0,0},{0,0,0,0}},{{0,6,0,0},{6,6,6,0},{0,0,0,0},{0,0,0,0}},{{0,6,0,0},{0,6,6,0},{0,6,0,0},{0,0,0,0}}},
        {{{0,0,0,0},{2,2,2,0},{2,0,0,0},{0,0,0,0}},{{2,2,0,0},{0,2,0,0},{0,2,0,0},{0,0,0,0}},{{0,0,2,0},{2,2,2,0},{0,0,0,0},{0,0,0,0}},{{0,2,0,0},{0,2,0,0},{0,2,2,0},{0,0,0,0}}},
        {{{0,0,0,0},{5,5,5,0},{0,0,5,0},{0,0,0,0}},{{0,5,0,0},{0,5,0,0},{5,5,0,0},{0,0,0,0}},{{5,0,0,0},{5,5,5,0},{0,0,0,0},{0,0,0,0}},{{0,5,5,0},{0,5,0,0},{0,5,0,0},{0,0,0,0}}},
        {{{0,0,0,0},{0,4,4,0},{4,4,0,0},{0,0,0,0}},{{0,0,0,0},{0,4,0,0},{0,4,4,0},{0,0,4,0}},{{0,0,0,0},{0,0,0,0},{0,4,4,0},{4,4,0,0}},{{0,0,0,0},{4,0,0,0},{4,4,0,0},{0,4,0,0}}},
        {{{0,0,0,0},{1,1,0,0},{0,1,1,0},{0,0,0,0}},{{0,0,0,0},{0,0,1,0},{0,1,1,0},{0,1,0,0}},{{0,0,0,0},{0,0,0,0},{1,1,0,0},{0,1,1,0}},{{0,0,0,0},{0,1,0,0},{1,1,0,0},{1,0,0,0}}},
        {{{0,0,0,0},{0,3,3,0},{0,3,3,0},{0,0,0,0}},{{0,0,0,0},{0,3,3,0},{0,3,3,0},{0,0,0,0}},{{0,0,0,0},{0,3,3,0},{0,3,3,0},{0,0,0,0}},{{0,0,0,0},{0,3,3,0},{0,3,3,0},{0,0,0,0}}}
    };
    int count = 0;
    for (int i = 0; i < 4; i ++){
        for (int j = 0; j < 4; j ++){
            if (array2[info[1]+i-1][info[2]+j] == 0 && turnBlock[info[0]][(info[3]+direction)%4][i][j] != 0){
                count ++;
            }
        }
    }
    if (count < 4){
        return 1;
    } else {
        for (int i = 0; i < 4; i ++){
            for (int j = 0; j < 4; j ++){
                array1[info[1]+i-1][info[2]+j] = turnBlock[info[0]][(info[3]+direction)%4][i][j];
            }
        }
        info[3] = (info[3]+direction) % 4;
        return 0;
    }
}

int setBoard (int array1[23][12], int array2[23][12]){
    for (int i = 0; i < 22; i ++){
        for (int j = 1; j <= 10; j ++){
            if (array1[i][j] != 0){
                array2[i][j] = array1[i][j];
                array1[i][j] = 0;
            }
        }
    }
}

int delLine (int array1[23][12], int array2[23][12], int array3[23][12], int random[3], int h, int *score){
    int line[22] = {0};
    int branch = 0;
    int number = 1;
    for (int i = 2; i < 22; i ++){
        int count = 0;
        for (int j = 1; j < 11; j ++){
            if (array2[i][j] != 0){
                count ++;
            }
        }
        if (count == 10){
            for (int j = 1; j < 11; j ++){
                array2[i][j] = 9;
                printboard(array1,array2,array3,random,h,*score);
                *score += 10*number;
                wait(15);
            }
            line[i] = 1;
            branch = 1;
            number ++;
        }
    }
    if (branch == 1) {
        printboard(array1,array2,array3,random,h,*score);
        wait(25);
        for (int i = 1; i <= 21; i ++){
            if (line[i] == 1){
                for (int j = i-1; j > 0; j --){
                    for (int k = 1; k < 11; k ++){
                        array2[j+1][k] = array2[j][k];
                    }
                }
            }
        }
    }
}

void makeSup(int array1[23][12], int array2[23][12], int info[4]){
    for (int i = 1; i < 22; i ++){
        for (int j = 1; j < 11; j ++){
            array2[i][j] = 0;
        }
    }
    int turnBlock[7][4][4][4] = {
        {{{0,0,0,0},{7,7,7,7},{0,0,0,0},{0,0,0,0}},{{0,0,7,0},{0,0,7,0},{0,0,7,0},{0,0,7,0}},{{0,0,0,0},{0,0,0,0},{7,7,7,7},{0,0,0,0}},{{0,7,0,0},{0,7,0,0},{0,7,0,0},{0,7,0,0}}},
        {{{0,0,0,0},{6,6,6,0},{0,6,0,0},{0,0,0,0}},{{0,6,0,0},{6,6,0,0},{0,6,0,0},{0,0,0,0}},{{0,6,0,0},{6,6,6,0},{0,0,0,0},{0,0,0,0}},{{0,6,0,0},{0,6,6,0},{0,6,0,0},{0,0,0,0}}},
        {{{0,0,0,0},{2,2,2,0},{2,0,0,0},{0,0,0,0}},{{2,2,0,0},{0,2,0,0},{0,2,0,0},{0,0,0,0}},{{0,0,2,0},{2,2,2,0},{0,0,0,0},{0,0,0,0}},{{0,2,0,0},{0,2,0,0},{0,2,2,0},{0,0,0,0}}},
        {{{0,0,0,0},{5,5,5,0},{0,0,5,0},{0,0,0,0}},{{0,5,0,0},{0,5,0,0},{5,5,0,0},{0,0,0,0}},{{5,0,0,0},{5,5,5,0},{0,0,0,0},{0,0,0,0}},{{0,5,5,0},{0,5,0,0},{0,5,0,0},{0,0,0,0}}},
        {{{0,0,0,0},{0,4,4,0},{4,4,0,0},{0,0,0,0}},{{0,0,0,0},{0,4,0,0},{0,4,4,0},{0,0,4,0}},{{0,0,0,0},{0,0,0,0},{0,4,4,0},{4,4,0,0}},{{0,0,0,0},{4,0,0,0},{4,4,0,0},{0,4,0,0}}},
        {{{0,0,0,0},{1,1,0,0},{0,1,1,0},{0,0,0,0}},{{0,0,0,0},{0,0,1,0},{0,1,1,0},{0,1,0,0}},{{0,0,0,0},{0,0,0,0},{1,1,0,0},{0,1,1,0}},{{0,0,0,0},{0,1,0,0},{1,1,0,0},{1,0,0,0}}},
        {{{0,0,0,0},{0,3,3,0},{0,3,3,0},{0,0,0,0}},{{0,0,0,0},{0,3,3,0},{0,3,3,0},{0,0,0,0}},{{0,0,0,0},{0,3,3,0},{0,3,3,0},{0,0,0,0}},{{0,0,0,0},{0,3,3,0},{0,3,3,0},{0,0,0,0}}}
    };
    int n = 0;
    for (int i = 0; i <22; i ++){
        int count = 0;
        for (int j = 0; j < 4; j ++){
            for (int k = 0; k < 4; k ++){
                if (array1[info[1]+i+j-1][info[2]+k] == 0 && turnBlock[info[0]][info[3]][j][k] != 0){
                    count ++;
                }
            }
        }
        if (count < 4){
            n = i-1;
            break;
        }
    }
    /*printf("%d\n",n);*/
    for (int j = 0; j < 4; j ++){
        for (int k = 0; k < 4; k ++){
            if (turnBlock[info[0]][info[3]][j][k] != 0){
                array2[info[1]+n+j-1][info[2]+k] = 8;
            }
        }
    } 
}

void hold(int *i,int *n,int array2[23][12], int info[4], int random[3]){
    if (*i == 0) return;
    if (*n == -1){
        *n = info[0];
        for (int j = 0; j < 23; j ++){
            for (int k = 0; k < 12; k ++){
                array2[j][k] = 0;
            }
        }
        setblock(array2,info,random);
        *i = 0;
        return;
    }
    int blocks[7][2][4] = {{{0,0,0,0},{7,7,7,7}},{{6,6,6,0},{0,6,0,0}},
                            {{2,2,2,0},{2,0,0,0}},{{5,5,5,0},{0,0,5,0}},
                            {{0,4,4,0},{4,4,0,0}},{{1,1,0,0},{0,1,1,0}},
                            {{0,3,3,0},{0,3,3,0}}};
    int nowColor = info[0];
    for (int i = 0; i < 23; i ++){
        for (int j = 0; j < 12; j ++){
            if (i >= 1 && i <= 2 && j >= 4 && j <= 7)
            array2[i][j] = blocks[*n][i-1][j-4];
            else array2[i][j] = 0;
        }
    }
    info[0] = *n;
    info[1] = 1;
    info[2] = 4;
    info[3] = 0;
    *n = nowColor;
    *i = 0; 
}

int main () {
    srand((unsigned)time(NULL));
    int board[23][12] = {{9,0,0,0,0,0,0,0,0,0,0,9},
                        {9,0,0,0,0,0,0,0,0,0,0,9},
                        {9,0,0,0,0,0,0,0,0,0,0,9},
                        {9,0,0,0,0,0,0,0,0,0,0,9},
                        {9,0,0,0,0,0,0,0,0,0,0,9},
                        {9,0,0,0,0,0,0,0,0,0,0,9},
                        {9,0,0,0,0,0,0,0,0,0,0,9},
                        {9,0,0,0,0,0,0,0,0,0,0,9},
                        {9,0,0,0,0,0,0,0,0,0,0,9},
                        {9,0,0,0,0,0,0,0,0,0,0,9},
                        {9,0,0,0,0,0,0,0,0,0,0,9},
                        {9,0,0,0,0,0,0,0,0,0,0,9},
                        {9,0,0,0,0,0,0,0,0,0,0,9},
                        {9,0,0,0,0,0,0,0,0,0,0,9},
                        {9,0,0,0,0,0,0,0,0,0,0,9},
                        {9,0,0,0,0,0,0,0,0,0,0,9},
                        {9,0,0,0,0,0,0,0,0,0,0,9},
                        {9,0,0,0,0,0,0,0,0,0,0,9},
                        {9,0,0,0,0,0,0,0,0,0,0,9},
                        {9,0,0,0,0,0,0,0,0,0,0,9},
                        {9,0,0,0,0,0,0,0,0,0,0,9},
                        {9,0,0,0,0,0,0,0,0,0,0,9},
                        {9,9,9,9,9,9,9,9,9,9,9,9}};
    int move[23][12] = {{0,0,0,0,0,0,0,0,0,0,0,0}};
    int sup[23][12] = {{0,0,0,0,0,0,0,0,0,0,0,0}};
    int info[4] = {0,0,0,0};
    int b;
    int random[3];
    int canHold = 1;
    int nowHold = -1;
    int score = 0;

    for (int i = 0; i < 3; i ++){
        random[i] = rand() % 7;
    }
    setblock(move,info,random);
    canHold = 1;
    makeSup(board,sup,info);
    printboard(board,move,sup,random,nowHold,score);
    for(;;){
        clock_t c = clock();
        for (;(clock()-c)*1000/CLOCKS_PER_SEC<500;){
            
            if (kbhit()){
                char a = getchar();
                if (a == 32){
                    break;
                } else if (a == 102){
                    right(move,board,info);
                } else if (a == 100){
                    left(move,board,info);
                } else if (a == 107){
                    turn(move,board,info,1);
                } else if (a == 106){
                    turn(move,board,info,3);
                } else if (a == 104){
                    hold(&canHold,&nowHold,move,info,random);
                }
                makeSup(board,sup,info);
                printboard(board,move,sup,random,nowHold,score);
            }
        }
        b = down(move,board,info);
        if (b == 1){
            setBoard(move,board);
            delLine(move,board,sup,random,nowHold,&score);
            setblock(move,info,random);
            makeSup(board,sup,info);
            canHold = 1;
        }
        printboard(board,move,sup,random,nowHold,score);
    }   
    return 0;
}

//TODO: 回転いれの実装