// cnf文件的读取
#include "def.h"
extern int maxLiteralNum;

void cnfPrint(Clause *C)
{
    Clause *cp = C->next;
    while (cp)
    {
        int num = cp->literalNum;
        Literal *lp = cp->lits;
        for (int i = 0; i < num; i++)
        {
            printf("%d ", lp->variable);
            lp = lp->next;
        }
        printf("\n");
        cp = cp->next;
    }
}

Clause *cnfRead(char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        printf("ERROR OPPENING FILE");
        exit(-1);
    } // 读取失败

    Clause *C = (Clause *)malloc(sizeof(Clause)); // 需要返回的头结点
    Clause *chead, *ctail, *cp;
    chead = ctail = NULL;

    C->lits = NULL;

    char line[105];
    while (fgets(line, sizeof(line), fp))
    {
        if (line[0] == 'c')
        {
            // 跳过注释行
            continue;
        }
        else if (line[0] == 'p')
        {
            char *token = strtok(line, " "); // 取出 p
            token = strtok(NULL, " ");       // 取出cnf

            token = strtok(NULL, " ");
            maxLiteralNum = atoi(token);

            token = strtok(NULL, " ");
            C->literalNum = atoi(token);
        }
        else
        {

            cp = (Clause *)malloc(sizeof(Clause));
            cp->literalNum = 0;

            Literal *lhead, *ltail, *lp;
            lhead = ltail = NULL;
            char *token = strtok(line, " ");
            while (atoi(token) != 0)
            {
                lp = (Literal *)malloc(sizeof(Literal));
                cp->literalNum++;
                lp->variable = atoi(token);

                if (lhead == NULL)
                    lhead = lp;
                else
                    ltail->next = lp;
                ltail = lp;
                token = strtok(NULL, " ");
            }
            ltail->next = NULL;
            cp->lits = lhead;

            if (chead == NULL)
                chead = cp;
            else
                ctail->next = cp;
            ctail = cp;
        }
    }
    ctail->next = NULL;
    C->next = chead;

    fclose(fp);

    return C;
}

// test
// int main()
// {
//     char filename[10];
//     printf("输入文件名：\n");
//     scanf("%s", filename);
//     Clause *C = cnfRead(filename);
//     cnfPrint(C);

//     return 0;
// }