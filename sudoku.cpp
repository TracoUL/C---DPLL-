#include "def.h"

int fillBlank(int a[][COL], int i, int j) // i,j 是要填数字的空的坐标
{
    if (i < ROW && j < COL)
    {

        int x, y, k;
        int check[COL + 1] = {CORRECT}; // 初始化check数组用于排除已经使用过的的数字

        // 标记列中已经使用的数字
        for (x = 0; x < i; x++)
            check[a[x][j]] = WRONG;

        // 标记行中已经使用的数字
        for (y = 0; y < j; y++)
            check[a[i][y]] = WRONG;

        // 标记当前宫格中已经使用的数字
        int startRow = i / 3 * 3;
        int startCol = j / 3 * 3;
        for (x = startRow; x <= i; x++)
        {
            if (x == i)
            {
                for (y = startCol; y < j; y++)
                    check[a[x][y]] = WRONG;
            }
            else
            {
                for (y = startCol; y < startCol + 3; y++)
                    check[a[x][y]] = WRONG;
            }
        }

        // 标记对角线上已经使用的数字
        if (i == j)
        {
            for (x = 0; x < i; x++)
            {
                if (a[x][x])
                    check[a[x][x]] = WRONG;
            }
        }
        if ((i + j) == 8)
        {
            for (x = 0; x < i; x++)
            {
                if (a[x][8 - x])
                    check[a[x][8 - x]] = WRONG;
            }
        }

        // 尝试填入一个有效的数字
        int flag = 0;
        for (k = 1; k <= COL && flag == 0; k++)
        {
            if (check[k] == CORRECT)
            {
                a[i][j] = k;
                flag = 1;

                // 递归填充下一个位置
                if (j == COL - 1 && i != ROW - 1) // 尝试填下一列的第一个位置
                {
                    if (fillBlank(a, i + 1, 0) == CORRECT)
                        return CORRECT;
                    else
                        flag = 0;
                }
                else if (j != COL - 1) // 尝试填同一行的下一个位置
                {
                    if (fillBlank(a, i, j + 1) == CORRECT)
                        return CORRECT;
                    else 
                        flag = 0;
                }
            }
        }
        if (flag == 0)
        {
            a[i][j] = 0;
            return WRONG;
        }
    }
    return CORRECT;
}

int randFirstRow(int a[ROW][COL])
{
    int check[10] = {CORRECT};
    int i = 0; // 已填的数目

    srand((unsigned int)time(NULL));

    while (i < 9)
    {
        int num = 1 + rand() % 9;
        if (check[num] == CORRECT)
        {
            a[0][i] = num;
            i++;
            check[num] = WRONG;
        }
    }

    return 0;
}

// int **dig(int a[ROW][COL], int holeNum)
// {
//     int b[ROW][COL] = {0}; // 记录hole的位置
//     int **c;               // 生成的数独初局
//     c = (int **)malloc(ROW * sizeof(int *));
//     for (int i = 0; i < 9; i++)
//         c[i] = (int *)malloc(COL * sizeof(int));

//     int num = 0; // 记录现在hole的数量

//     // srand((unsigned int)time(NULL));

//     while (num <= holeNum)
//     {
//         int i, j;
//         i = 1 + rand() % 9;
//         j = 1 + rand() % 9;
//         if (b[i][j] == 0)
//         {
//             b[i][j] = 1;
//             num++;
//         }
//     }

//     for (int i = 0; i < ROW; i++)
//     {
//         for (int j = 0; j < COL; j++)
//         {
//             if (b[i][j])
//                 c[i][j] = 0;
//             c[i][j] = (*a[i]) + j;
//         }
//     }

//     return c;
// }

int **dig(int a[ROW][COL], int holeNum)
{

    int b[ROW][COL] = {0}; // 记录hole的位置

    int **c = (int **)malloc(ROW * sizeof(int *));
    for (int i = 0; i < ROW; i++)
        c[i] = (int *)malloc(COL * sizeof(int));

    int num = 0; // 记录现在hole的数量

    srand((unsigned int)time(NULL));

    while (num < holeNum)
    {
        int i, j;
        i = rand() % 9;
        j = rand() % 9;
        if (b[i][j] == 0)
        {
            b[i][j] = 1;
            num++;
        }
    }

    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            // if (b[i][j])
            //     a[i][j] = 0;

            if (b[i][j])
                c[i][j] = 0;
            else
                c[i][j] = a[i][j];
        }
    }

    return c;
}

// void createSudoku(int a[][COL], int holeNum)
// {
//     randFirstRow(a);
//     fillBlank(a, 1, 0);
//     dig(a, holeNum);
// }

int transToNature(int i, int j, int k) // 将数独位置编码转化为自然编码
{
    return ((i - 1) * 81 + (j - 1) * 9 + k);
    // return (i * 100 + j * 10 + k);
}

void transToCnf(int a[ROW][COL], char *filename)
{
    FILE *fp = fopen(filename, "w");
    if (!fp)
    {
        printf("FILE OPENING ERROR");
        exit(-1);
    }
    fprintf(fp, "p cnf 729 9477 0 \n");

    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (a[i][j])
            {
                fprintf(fp, "%d ", transToNature(i + 1, j + 1, a[i][j]));
                fprintf(fp, "0\n");
            }

            else
            {
                for (int k = 1; k <= 9; k++)
                {
                    fprintf(fp, "%d ", transToNature(i + 1, j + 1, k));
                }
                fprintf(fp, "0\n");
            }
        }
    }

    // 行约束
    for (int x = 1; x <= 9; x++)
    {
        for (int y1 = 1; y1 <= 8; y1++)
        {
            for (int y2 = y1 + 1; y2 <= 9; y2++)
            {
                for (int k = 1; k <= 9; k++)
                {
                    fprintf(fp, "%d ", -transToNature(x, y1, k));
                    fprintf(fp, "%d ", -transToNature(x, y2, k));
                    fprintf(fp, "0\n");
                }
            }
        }
    }

    // 列约束
    for (int y = 1; y <= 9; y++)
    {
        for (int x1 = 1; x1 <= 8; x1++)
        {
            for (int x2 = x1 + 1; x2 <= 9; x2++)
            {
                for (int k = 1; k <= 9; k++)
                {
                    fprintf(fp, "%d ", -transToNature(x1, y, k));
                    fprintf(fp, "%d ", -transToNature(x2, y, k));
                    fprintf(fp, "0\n");
                }
            }
        }
    }

    // 宫约束
    int grid[ROW][COL] = {0};
    int x = 0, y = 0;
    for (int starti = 1; starti <= 7; starti += 3)
    {
        for (int startj = 1; startj <= 7; startj += 3)
        {
            for (int i = starti; i < starti + 3; i++)
            {
                for (int j = startj; j < startj + 3; j++)
                {
                    if (y > 8)
                    {
                        y = 0;
                        x++;
                    }
                    grid[x][y] = 10 * i + j;
                    y++;
                }
            }
        }
    }

    for (int x = 0; x <= 8; x++)
    {
        for (int y1 = 0; y1 <= 7; y1++)
        {
            for (int y2 = y1 + 1; y2 <= 8; y2++)
            {
                for (int k = 1; k <= 9; k++)
                {
                    fprintf(fp, "%d ", -transToNature(grid[x][y1] / 10, grid[x][y1] % 10, k));
                    fprintf(fp, "%d ", -transToNature(grid[x][y2] / 10, grid[x][y2] % 10, k));
                    fprintf(fp, "0\n");
                }
            }
        }
    }

    // 对角线约束
    for (int i = 1; i <= 8; i++)
    {
        for (int j = i + 1; j <= 9; j++)
        {
            for (int k = 1; k <= 9; k++)
            {
                fprintf(fp, "%d ", -transToNature(i, i, k));
                fprintf(fp, "%d ", -transToNature(j, j, k));
                fprintf(fp, "0\n");
            }
        }
    }

    for (int i = 1; i <= 8; i++)
    {
        for (int j = i + 1; j <= 9; j++)
        {
            for (int k = 1; k <= 9; k++)
            {
                fprintf(fp, "%d ", -transToNature(i, 10 - i, k));
                fprintf(fp, "%d ", -transToNature(j, 10 - j, k));
                fprintf(fp, "0\n");
            }
        }
    }
    fclose(fp);
}

// void transToCnf(int a[][COL], int holes, char *filename)
// {

//     FILE *fp = fopen(filename, "w");
//     if (!fp)
//     {
//         printf("FILE OPENING ERROR");
//         exit(-1);
//     }
//     fprintf(fp, "p 729 9477 0 \n");

//     // single clause
//     for (int x = 0; x < ROW; ++x)
//     {
//         for (int y = 0; y < COL; ++y)
//             if (a[x][y] != 0)
//                 fprintf(fp, "%d 0\n", (x + 1) * 100 + (y + 1) * 10 + a[x][y]);
//     }
//     // entry
//     for (int x = 1; x <= 9; ++x)
//     {
//         for (int y = 1; y <= 9; ++y)
//         {
//             for (int z = 1; z <= 9; ++z)
//                 fprintf(fp, "%d 0\n", x * 100 + y * 10 + a[x][y]);
//         }
//     }
//     // row
//     for (int y = 1; y <= 9; ++y)
//     {
//         for (int z = 1; z <= 9; ++z)
//             for (int x = 1; x <= 8; ++x)
//                 for (int i = x + 1; i <= 9; ++i)
//                 {
//                     fprintf(fp, "%d ", -(x * 100 + y * 10 + a[x][y]));
//                     fprintf(fp, "%d 0\n", -(i * 100 + y * 10 + a[x][y]));
//                 }
//     }
//     // column
//     for (int x = 1; x <= 9; ++x)
//     {
//         for (int z = 1; z <= 9; ++z)
//             for (int y = 1; y <= 8; ++y)
//                 for (int i = y + 1; i <= 9; ++i)
//                 {
//                     fprintf(fp, "%d ", -(x * 100 + y * 10 + a[x][y]));
//                     fprintf(fp, "%d 0\n", -(x * 100 + i * 10 + a[x][y]));
//                 }
//     }
//     // 3*3 sub-grids
//     for (int z = 1; z <= 9; ++z)
//     {
//         for (int i = 0; i <= 2; ++i)
//             for (int j = 0; j <= 2; ++j)
//                 for (int x = 1; x <= 3; ++x)
//                     for (int y = 1; y <= 3; ++y)
//                         for (int k = y + 1; k <= 3; ++k)
//                         {
//                             fprintf(fp, "%d ", -((3 * i + x) * 100 + (3 * j + y) * 10 + z));
//                             fprintf(fp, "%d 0\n", -((3 * i + x) * 100 + (3 * j + k) * 10 + z));
//                         }
//     }
//     for (int z = 1; z <= 9; z++)
//     {
//         for (int i = 0; i <= 2; i++)
//             for (int j = 0; j <= 2; j++)
//                 for (int x = 1; x <= 3; x++)
//                     for (int y = 1; y <= 3; y++)
//                         for (int k = x + 1; k <= 3; k++)
//                             for (int l = 1; l <= 3; l++)
//                             {
//                                 fprintf(fp, "%d ", -((3 * i + x) * 100 + (3 * j + y) * 10 + z));
//                                 fprintf(fp, "%d 0\n", -((3 * i + x) * 100 + (3 * j + k) * 10 + z));
//                             }
//     }

//     fclose(fp);
// }

void SudokuPrint(int a[][COL])
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (a[i][j])
                printf("%d ", a[i][j]);
            else
                printf("_ ");
        }
        printf("\n");
    }
}

// void finSudokuPrint(int **a)
// {
//     for (int i = 0; i < ROW; i++)
//     {
//         for (int j = 0; j < COL; j++)
//         {
//             if (*(*(a + i) + j))
//                 printf("%d ", *(*(a + i) + j));
//             else
//                 printf("_ ");
//         }
//         printf("\n");
//     }
// }

// test
// int main()
// {
//     int a[ROW][COL] = {0};
//     randFirstRow(a);
//     fillBlank(a, 1, 0);
//     // iniSudokuPrint(a);

//     int **c;
//     c = dig(a, 20);
//     iniSudokuPrint(a);
//     printf("\n");
//     finSudokuPrint(c);
//     printf("\n");

//     char filename[20];
//     printf("请输入数独cnf存储的文件名：");
//     scanf("%s", filename);
//     transToCnf(a, filename);

//     return 0;
// }