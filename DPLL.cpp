#include "def.h"
#include "cnfRead.cpp"

extern int maxLiteralNum;

// 伪代码
//  DPLL(合取范式S)
//  {
//      while (合取范式S中含有单子句)
//      {
//          令单子句为真;
//          按照单子句传播策略将S化简为S1;
//          if (S1 == 空集)
//              return TRUE;
//          else if (S1 中含有 空子句)
//              return FALSE;
//      }
//      基于某种策略选取一个变元v;
//      if (DPLL(S1并v))
//          return TRUE;
//      return DPLL(S1并 - v);
//  }

int unitaryClauseJudge(Clause *C) // 检测cnf中是否含有单子句,返回该单子句对应的文字
{
    Clause *cp = C->next;
    while (cp)
    {
        if (cp->literalNum == 1)
            return cp->lits->variable;

        cp = cp->next;
    }

    return 0;
}

void freeClause(Clause *Clause)
{
    Literal *lp = Clause->lits, *lq = lp;
    while (lp)
    {
        lq = lp;
        lp = lp->next;
        free(lq);
    }
    free(Clause);
}

// 根据标定文字删除子句（含有 + calibratedLiteral直接去，含 - calibratedLiteral的子句中去掉 - calibratedLiteral这个文字）
void unitaryClauseDelete(Clause *C, int calibratedLiteral)
{
    Clause *cp = C->next, *cq = cp;
    int flag = 0; // 表示字句是否被删除
    while (cp != NULL)
    {
        flag = 0;
        Literal *lp = cp->lits, *lq = lp;
        while (lp)
        {
            if (lp->variable == calibratedLiteral) // 删除该子句
            {
                flag = 1;
                if (cp == cq)
                {
                    C->next = cp->next;
                    cp = cp->next;
                    freeClause(cq);
                    cq = cp;
                }
                else
                {
                    cq->next = cp->next;
                    Clause *temp = cp;
                    cp = cp->next;
                    freeClause(temp);
                }
                C->literalNum--;
                break;
            }
            else if ((0 - lp->variable) == calibratedLiteral) // 删除该文字
            {
                if (lp == lq)
                {
                    cp->lits = lp->next;
                    lp = lp->next;
                    free(lq);
                    lq = lp;
                }
                else
                {
                    lq->next = lp->next;
                    Literal *temp = lp;
                    lp = lp->next;
                    free(temp);
                }
                cp->literalNum--;
            }
            if (lp)
            {
                lq = lp;
                lp = lp->next;
            }
        }
        if (!flag)
        {
            cq = cp;
            cp = cp->next;
        }
    }
}
// void unitaryClauseDelete(Clause *C, int calibratedLiteral)
// {
//     Clause *cp = C->next, *cq = C;
//     int flag = 0; // 表示字句是否被删除
//     while (cp != NULL)
//     {
//         flag = 0;
//         Literal *lp = cp->lits, *lq = lp;
//         while (lp)
//         {
//             flag = 0;
//             if (lp->variable == calibratedLiteral) // 删除该子句
//             {
//                 flag = 1;

//                 cq->next = cp->next;
//                 Clause *temp = cp;
//                 cp = cp->next;
//                 freeClause(temp);

//                 C->literalNum--;
//                 break;
//             }
//             else if ((0 - lp->variable) == calibratedLiteral) // 删除该文字
//             {
//                 if (lp == lq)
//                 {
//                     flag = 2;
//                     cp->lits = lp->next;
//                     cp->literalNum--;
//                 }
//                 else
//                 {
//                     flag = 3;
//                     lq->next = lp->next;
//                     cp->literalNum--;
//                 }
//             }
//             if (!flag)
//             {
//                 if (lq != lp)
//                     lq = lp;
//                 lp = lp->next;
//             }
//             else if (flag == 2)
//             { // 删除的结点为第一个节点的情况处理
//                 lp = lq->next;
//                 lq = lp;
//             }
//             else if (flag == 3)
//             { // 删除的节点不是第一个，在中间
//                 free(lp);
//                 lp = lq->next;
//             }
//         }
//         if (!flag)
//         {
//             cq = cp;
//             cp = cp->next;
//         }
//         else if (flag == 1)
//         {
//             free(cp);
//             cp = cq->next;
//         }
//     }
// }

// 检测cnf是否为空
int isCnfEmpty(Clause *C)
{
    if (C->literalNum)
        return 0;

    return 1;
}

// 检测cnf中是否有空子句
int isClauseEmpty(Clause *C)
{
    Clause *cp = C->next;
    while (cp)
    {
        if (cp->literalNum == 0)
            return 1;

        cp = cp->next;
    }

    return 0;
}

// 将一个新的单子句（NC）添加到cnf中
Clause *addNewClause(Clause *C, int NC)
{
    Clause *cp = (Clause *)malloc(sizeof(Clause));
    cp->literalNum = 1;
    cp->lits = (Literal *)malloc(sizeof(Literal));
    cp->lits->variable = NC;
    cp->lits->next = NULL;
    cp->next = C->next;
    C->next = cp;
    C->literalNum++;
    return C;
}

Clause *copyCnf(Clause *C)
{
    Clause *temp = (Clause *)malloc(sizeof(Clause)), *cp1, *cp2 = C->next; // cp1跟踪temp，cp2跟踪C
    temp->literalNum = C->literalNum;

    Clause *chead, *ctail;
    chead = ctail = NULL;

    while (cp2)
    {
        cp1 = (Clause *)malloc(sizeof(Clause));
        cp1->literalNum = cp2->literalNum;

        Literal *lhead, *ltail, *lp1, *lp2 = cp2->lits;
        lhead = ltail = NULL;
        while (lp2)
        {
            lp1 = (Literal *)malloc(sizeof(Literal));
            lp1->variable = lp2->variable;

            if (lhead == NULL)
                lhead = lp1;
            else
                ltail->next = lp1;
            ltail = lp1;

            lp2 = lp2->next;
        }
        ltail->next = NULL;
        cp1->lits = lhead;

        if (chead == NULL)
            chead = cp1;
        else
            ctail->next = cp1;
        ctail = cp1;

        cp2 = cp2->next;
    }
    ctail->next = NULL;
    temp->next = chead;

    return temp;
}

int select1(Clause *C) // 策略1：就近
{
    return C->next->lits->variable;
}

int select2(Clause *C) // 策略2：就近后取绝对值
{
    return abs(C->next->lits->variable);
}

int select3(Clause *C) // 策略3：选择当前cnf中出现次数最多的文字
{
    int literalNum[maxLiteralNum + 1] = {0};
    Clause *cp = C->next;
    Literal *lp = cp->lits;
    while (cp)
    {
        lp = cp->lits;
        while (lp)
        {
            literalNum[lp->variable]++;
            lp = lp->next;
        }
        cp = cp->next;
    }

    int max = literalNum[1];
    int maxLiteral = 1;
    for (int i = 2; i < maxLiteralNum; i++)
    {
        if (literalNum[i] > max)
        {
            max = literalNum[i];
            maxLiteral = i;
        }
    }

    return maxLiteral;
}

// int select4(Clause *C) // 策略4：选择短子句中出现次数最多的文字
// {
// }

void freeCnf(Clause *C)
{
    Clause *cp = C->next, *cq = cp;
    while (cp)
    {
        cq = cp;
        cp = cp->next;
        freeClause(cq);
    }

    free(C);
}

int DPLL(Clause *C, int *solution, int op)
{
    int calibratedLiteral;
    while (calibratedLiteral = unitaryClauseJudge(C))
    {
        if (calibratedLiteral > 0)
            solution[calibratedLiteral] = 1;
        else
            solution[-calibratedLiteral] = 0;

        unitaryClauseDelete(C, calibratedLiteral);
        if (isCnfEmpty(C))
            return 1;
        else if (isClauseEmpty(C))
            return 0;
    }

    int NC;
    switch (op)
    {
    case 1:
        NC = select1(C); // 构建新的单子句
        break;
    case 2:
        NC = select2(C);
        break;
    case 3:
        NC = select3(C);
        break;
        // case 4:
        //     NC = select4(C);
    }

    Clause *temp = copyCnf(C);

    addNewClause(C, NC);
    if (DPLL(C, solution, op))
        return 1;

    addNewClause(temp, (0 - NC));
    int result = DPLL(temp, solution, op);
    // freeClause(temp);
    return result;
}

// test
// int main()
// {
//     char filename[10];
//     printf("输入文件名：\n");
//     scanf("%s", filename);

//     int op;
//     printf("请选择策略：\n");
//     scanf("%d", &op);

//     clock_t start, end;
//     start = clock();

//     Clause *C = cnfRead(filename);
//     int solution[(maxLiteralNum + 1)] = {0};
//     int flag = DPLL(C, solution, op);
//     freeCnf(C);

//     end = clock();

//     printf("%d\n", flag);

//     if (flag)
//     {
//         for (int i = 1; i <= maxLiteralNum; i++)
//         {
//             if (solution[i])
//                 printf("%d ", i);
//             else
//                 printf("-%d ", i);
//         }
//         printf("\n");
//     }

//     printf("The time was: %d\n", (end - start));

//     return 0;
// }