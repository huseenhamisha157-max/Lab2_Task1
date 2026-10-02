#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <windows.h>

const int PORTION = 5; // порция памяти (не более 5)

// Словесное представление чётной цифры
const char* evenWord(int d)
{
    switch (d) {
    case 0: return "ноль";
    case 2: return "два";
    case 4: return "четыре";
    case 6: return "шесть";
    default: return "восемь"; // 8
    }
}

// Изменяем размер блока через realloc с проверкой
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

    printf("Введите строку: ");

    // ---- Ввод: первая порция через calloc ----
    int cap = PORTION;
    char* s = (char*)calloc(cap, sizeof(char));
    if (s == NULL) {
        printf("Memory not allocated.\n");
        return 1;
    }

    int len = 0;
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        if (len == cap) {          // порция заполнена -> добавляем ещё одну
            cap += PORTION;
            s = resize(s, cap);
        }
        s[len++] = (char)c;
    }

    if (len == 0) {
        printf("Строка пустая.\n");
        free(s);
        return 0;
    }

    // ---- Подсчёт, на сколько вырастет строка ----
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

    // ---- Увеличиваем блок и обрабатываем с конца ----
    if (len + extra > cap) {
        cap = len + extra;
        s = resize(s, cap);
    }

    int j = len + extra; // позиция записи (справа налево)
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

    // ---- Вывод результата ----
    printf("Результат: ");
    for (int i = 0; i < len + extra; i++)
        putchar(s[i]);
    printf("\n");

    free(s);
    return 0;
}