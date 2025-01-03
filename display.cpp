#include "def.h"
#include "DPLL.cpp"
#include "sudoku.cpp"

extern int maxLiteralNum;

int main()
{
start:
    printf("______________菜单______________\n");
    printf("1.SAT问题求解  2.数独游戏  0.退出\n");
    int op;
    scanf("%d", &op);
    switch (op)
    {
    case 1:
    {
        char filename[10];
        printf("请输入文件名：\n");
        scanf("%s", filename);

        int op1;
        printf("策略1：选择当前cnf中第一个子句的第一个文字\n");
        printf("策略2：选择当前cnf中第一个子句的第一个文字对应的正文字\n");
        printf("策略3：选择给定cnf中出现次数最多的文字\n");
        printf("策略4：选择当前cnf中第一个子句的第一个文字\n");
        printf("请选择策略（重复选择，按0退出）：\n");

        scanf("%d", &op1);
        double preTime = 0;

        while (op1)
        {

            clock_t start, end;
            start = clock();

            Clause *C = cnfRead(filename);
            int solution[(maxLiteralNum + 1)] = {0};
            int flag = DPLL(C, solution, op1);

            // freeCnf(C);

            end = clock();
            double time = end - start;

            printf("%d\n", flag);

            printf("用时: %.2fms\n", time);

            if (preTime == 0)
                preTime = time;
            else
            {
                double rate;
                if (time < preTime)
                {
                    rate = (preTime - time) / preTime * 100;
                    printf("优化率为：%.2f%%\n", rate);
                }

                else
                {
                    rate = (time - preTime) / preTime * 100;
                    printf("优化率为：-%.2f%%\n", rate);
                }
                preTime = time;
            }

            char Filename[20];
            strcpy(Filename, filename);
            char Ofilename[20];
            char *token = strtok(Filename, ".");
            strcpy(Ofilename, token);
            strcat(Ofilename, ".res");
            FILE *fp = fopen(Ofilename, "w");
            fprintf(fp, "s %d\n", flag);
            fprintf(fp, "v ");
            for (int i = 1; i <= maxLiteralNum; i++)
            {
                if (solution[i])
                    fprintf(fp, "%d ", i);
                else
                    fprintf(fp, "-%d ", i);
            }

            fprintf(fp, "\n");
            fprintf(fp, "t %.2f", time);
            fclose(fp);

            printf("\n");
            printf("策略1：选择当前cnf中第一个子句的第一个文字\n");
            printf("策略2：选择当前cnf中第一个子句的第一个文字对应的正文字\n");
            printf("策略3：选择当前cnf中出现次数最多的文字\n");
            printf("请选择策略（重复选择，按0退出）：\n");
            scanf("%d", &op1);
        }
        goto start;
        break;
    }

    case 2:
    {
    case2:
        printf("______________模式选择________________\n");
        printf("1.从文件中读入数独 2.随机生成数独 0.返回\n");
        int op2;
        scanf("%d", &op2);
        switch (op2)
        {
        case 1:
        {
            printf("请输入文件名：\n");
        }
        break;

        case 2:
        {
            printf("请输入数独中空白的个数：\n");
            int blankNum;
            scanf("%d", &blankNum);

            char filename[20];
            printf("请输入数独cnf存储的文件名：\n");
            scanf("%s", filename);

            int a[ROW][COL] = {0};
            randFirstRow(a);
            fillBlank(a, 1, 0);

            int **C = dig(a, blankNum);
            int c[ROW][COL];
            for (int i = 0; i < ROW; i++)
            {
                for (int j = 0; j < COL; j++)
                    c[i][j] = C[i][j];
            }

            SudokuPrint(c);
            transToCnf(c, filename);

            int op3;
            printf("1.游玩 2.求解 3.返回\n");
            scanf("%d", &op3);
            switch (op3)
            {
            case 1:
            {
                int temp[ROW][COL];
                for (int i = 0; i < ROW; i++)
                {
                    for (int j = 0; j < COL; j++)
                        temp[i][j] = c[i][j];
                }

                SudokuPrint(temp);
                printf("请输入要填的空的坐标和值组成的三位数(按0返回)：\n");
                int n;
                scanf("%d", &n);
                while (n)
                {
                    int i = n / 100 - 1, j = (n % 100) / 10 - 1, k = (n % 100) % 10;
                    if (a[i][j] == k)
                    {
                        temp[i][j] = k;
                        printf("正确\n");
                    }
                    else
                    {
                        printf("错误\n");
                    }
                    SudokuPrint(temp);
                    printf("请输入要填的空的坐标和值组成的三位数：\n");
                    scanf("%d", &n);
                }
                goto case2;
                break;
            }
            case 2:
            {
                Clause *C = cnfRead(filename);
                int solution[(maxLiteralNum + 1)] = {0};
                DPLL(C, solution, 1);
                int k = 1;
                for (int i = 0; i < ROW; i++)
                {
                    for (int j = 0; j < COL; j++)
                    {
                        int num2 = (solution[k] % 100) % 10;
                        c[i][j] = num2;
                    }
                }

                printf("数独的解为：\n");
                SudokuPrint(a);
                goto case2;
                break;
            }
            case 0:
            {
                goto case2;
            }
            }
        }

        case 3:
            goto start;
        }

        break;
    }

    case 0:
    {
        exit(1);
        break;
    }
    }

    return 0;
}
