#ifndef def_H_
#define def_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


#define ROW 9
#define COL 9
#define CORRECT 0
#define WRONG -1

// 建立一个十字链表来存储cnf

typedef struct Literal
{
    int variable; // 变量
    struct Literal *next;
} Literal;

typedef struct Clause
{
    int literalNum;       // 文字数
    struct Clause *next;  // 头结点向下（指向下一个子句）
    struct Literal *lits; // 同一个子句中结点向右指针（指向该子句中的第一个文字）
} Clause;

int maxLiteralNum;

// void cnfPrint(Clause *C)
// {
//     Clause *cp = C->next;
//     while (cp)
//     {
//         int num = cp->literalNum;
//         Literal *lp = cp->lits;
//         for (int i = 0; i < num; i++)
//         {
//             printf("%d ", lp->variable);
//             lp = lp->next;
//         }
//         printf("\n");
//         cp = cp->next;
//     }
// }

// Clause *cnfRead(char *filename)
// {
//     FILE *fp = fopen(filename, "r");
//     if (!fp)
//     {
//         printf("ERROR OPPENING FILE");
//         exit(-1);
//     } // 读取失败

//     Clause *C = (Clause *)malloc(sizeof(Clause)); // 需要返回的头结点
//     Clause *chead, *ctail, *cp;
//     chead = ctail = NULL;

//     C->lits = NULL;

//     char line[105];
//     while (fgets(line, sizeof(line), fp))
//     {
//         if (line[0] == 'c')
//         {
//             // 跳过注释行
//             continue;
//         }
//         else if (line[0] == 'p')
//         {
//             char *token = strtok(line, " "); // 取出 p
//             token = strtok(NULL, " ");       // 取出cnf

//             token = strtok(NULL, " ");
//             maxLiteralNum = atoi(token);
//             everyLiteralNum = (int *)malloc((maxLiteralNum+1) * sizeof(int) );
//             for (int i = 0; i <= maxLiteralNum; i++)
//                 everyLiteralNum[i] = 0;

//             token = strtok(NULL, " ");
//             C->literalNum = atoi(token);
//         }
//         else
//         {

//             cp = (Clause *)malloc(sizeof(Clause));
//             cp->literalNum = 0;

//             Literal *lhead, *ltail, *lp;
//             lhead = ltail = NULL;
//             char *token = strtok(line, " ");
//             while (atoi(token) != 0)
//             {
//                 lp = (Literal *)malloc(sizeof(Literal));
//                 cp->literalNum++;
//                 lp->variable = atoi(token);
//                 everyLiteralNum[atoi(token)]++;

//                 if (lhead == NULL)
//                     lhead = lp;
//                 else
//                     ltail->next = lp;
//                 ltail = lp;
//                 token = strtok(NULL, " ");
//             }
//             ltail->next = NULL;
//             cp->lits = lhead;

//             if (chead == NULL)
//                 chead = cp;
//             else
//                 ctail->next = cp;
//             ctail = cp;
//         }
//     }
//     ctail->next = NULL;
//     C->next = chead;

//     fclose(fp);

//     return C;
// }

// void cnfPrint(Clause *);
// Clause *cnfRead(char *);

// int unitaryClauseJudge(Clause *);
// void freeClause(Clause *Clause);
// void unitaryClauseDelete(Clause *C, int calibratedLiteral);
// int isCnfEmpty(Clause *C);
// int isClauseEmpty(Clause *C);
// Clause *addNewClause(Clause *C, int NC);
// Clause *copyCnf(Clause *C);
// int select1(Clause *C);
// int select2(Clause *C);
// int select3(Clause *C);
// void freeCnf(Clause *C);
// int DPLL(Clause *C, int *solution, int op);

// int fillBlank(int a[][COL], int i, int j);
// int randFirstRow(int a[ROW][COL]);
// void dig(int a[ROW][COL], int holeNum);
// int transToNature(int i, int j, int k);
// void transToCnf(int a[][COL], char *filename);
// void iniSudokuPrint(int a[][COL]);

#endif
