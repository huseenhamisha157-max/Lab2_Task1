#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <windows.h>

const int PORTION = 5; // ??????? ?????? ?? ???? ?? 5 ????

// ??????? ?????? ????? ??????
const char* evenWord(int d)
{
    switch (d) {
    case 0: return "ноль";
    case 2: return "два";
    case 4: return "четыре";
    case 6: return "шесть";
    default: return "восемь";
    }
}

// ????? ??? ??????? ??? realloc ?? ??????
char* resize(char* p, int newSize)
{
    char* tmp = (char*)realloc(p, newSize);
    if (tmp == NULL) {
        printf("Memory not allocated.\n");
        free(p);
        exit(1);
    }
    return tmp;
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // ??? ????? ???????
    FILE* fin = fopen("input.txt", "r");
    if (fin == NULL) {
        printf("Не удалось открыть файл input.txt\n");
        return 1;
    }

    int cap = PORTION;
    char* s = (char*)calloc(cap, sizeof(char));
    if (s == NULL) {
        printf("Memory not allocated.\n");
        fclose(fin);
        return 1;
    }

    int len = 0;
    char buffer[PORTION + 1];

    // ??????? ?? ????? ?????? (5 ???? ??? ????)
    while (fgets(buffer, sizeof(buffer), fin) != NULL) {
        int chunkLen = (int)strlen(buffer);

        while (len + chunkLen > cap) {
            cap += PORTION;
            s = resize(s, cap);
        }

        for (int i = 0; i < chunkLen; i++) {
            if (buffer[i] == '\n' || buffer[i] == '\r') continue;
            s[len++] = buffer[i];
        }
    }
    fclose(fin);

    if (len == 0) {
        printf("Файл пуст.\n");
        free(s);
        return 0;
    }

    // ???? ??? ??????? ????????
    int extra = 0;
    for (int i = 0; i < len; i++) {
        if (s[i] >= '0' && s[i] <= '9') {
            int d = s[i] - '0';
            if (d % 2 == 0)
                extra += (int)strlen(evenWord(d)) - 1;
            else
                extra += 2;
        }
    }

    // ????? ??? ?????? ????????? ?? ???????
    if (len + extra > cap) {
        cap = len + extra;
        s = resize(s, cap);
    }

    int j = len + extra;
    for (int i = len - 1; i >= 0; i--) {
        char ch = s[i];
        if (ch >= '0' && ch <= '9') {
            int d = ch - '0';
            if (d % 2 == 0) {
                const char* w = evenWord(d);
                for (int k = (int)strlen(w) - 1; k >= 0; k--)
                    s[--j] = w[k];
            }
            else {
                s[--j] = ch;
                s[--j] = ch;
                s[--j] = '#';
            }
        }
        else {
            s[--j] = ch;
        }
    }

    // ????? ???????
    printf("Результат из файла: ");
    for (int i = 0; i < len + extra; i++)
        putchar(s[i]);
    printf("\n");

    free(s);
    return 0;
}