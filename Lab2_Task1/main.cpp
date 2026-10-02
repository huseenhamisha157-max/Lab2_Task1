#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <clocale>
#include <windows.h>
// Ножной выделяем новый блок через malloc, копируем вручную, старый освобождаем
char* grow(char* old, int oldSize, int newSize)
{
    char* p = (char*)malloc(newSize);
    if (p == NULL) {
        printf("Memory not allocated.\n");
        exit(1);
    }
    for (int i = 0; i < oldSize && i < newSize; i++)
        p[i] = old[i];
    free(old);
    return p;
}

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

int main()
{
    SetConsoleCP(1251);        // ввод
    SetConsoleOutputCP(1251);  // вывод

    printf("Введите строку: ");

    // ---- Ввод: каждый раз память на 1 символ больше ----
    char* s = NULL;
    int len = 0;
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        if (len == 0) {
            s = (char*)malloc(1);
            if (s == NULL) { printf("Memory not allocated.\n"); return 1; }
        }
        else {
            s = grow(s, len, len + 1);
        }
        s[len++] = (char)c;
    }

    if (len == 0) {
        printf("Строка пустая.\n");
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
    if (extra > 0)
        s = grow(s, len, len + extra);

    int j = len + extra; // позиция записи (идём справа налево)
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