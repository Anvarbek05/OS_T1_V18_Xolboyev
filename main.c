/*
 * Operatsion tizimlar fani | 1-topshiriq | Variant 18
 * Mavzu : Siklli kalkulyator (+ - * / %)
 *
 * 1-masala (C asoslari) : siklli kalkulyator, kiritishni tekshirish, nolga bo'lish tekshiruvi.
 * 2-masala (pointerlar) : amal belgisiga qarab funksiya pointeri tanlanadi:
 *                         double (*amal)(double, double)
 *
 * Kompilyatsiya: gcc -Wall -Wextra -o main main.c -lm
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>

#define BUF 128 /* kiritish buferi hajmi (statik massiv) */

/* ---------- Amallar (funksiya pointeri shu imzoga ega) ---------- */
static double qosh(double a, double b)     { return a + b; }
static double ayir(double a, double b)     { return a - b; }
static double kopaytir(double a, double b) { return a * b; }
static double bol(double a, double b)      { return a / b; }
/* C da % double bilan ishlamaydi, shuning uchun fmod() */
static double qoldiq(double a, double b)   { return fmod(a, b); }

/* Amal belgisiga mos funksiya pointerini qaytaradi; noto'g'ri belgi bo'lsa NULL. */
static double (*amal_tanla(char belgi))(double, double)
{
    switch (belgi) {
    case '+': return qosh;
    case '-': return ayir;
    case '*': return kopaytir;
    case '/': return bol;
    case '%': return qoldiq;
    default:  return NULL;
    }
}

/* ---------- Kiritish ---------- */
/* Qator o'qiydi va '\n' ni olib tashlaydi. 1 - ok, 0 - juda uzun, -1 - EOF. */
static int qator_oqish(const char *taklif, char *buf, int hajm)
{
    size_t n;
    printf("%s", taklif);
    if (fgets(buf, hajm, stdin) == NULL)
        return -1;
    n = strlen(buf);
    if (n > 0 && buf[n - 1] == '\n') {
        buf[n - 1] = '\0';
    } else {
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;
        printf("Xato: juda uzun kiritish.\n");
        return 0;
    }
    n = strlen(buf);
    if (n > 0 && buf[n - 1] == '\r')
        buf[n - 1] = '\0';
    return 1;
}

/*
 * Son o'qiydi. Qaytish: 1 - son, 2 - foydalanuvchi 'q' kiritdi, -1 - EOF.
 * Harf, bo'sh qator, nan/inf va oraliqdan tashqari qiymat rad etiladi.
 */
static int son_oqish(const char *taklif, double *natija)
{
    char buf[BUF];
    char *oxiri;
    double v;
    int r;

    for (;;) {
        r = qator_oqish(taklif, buf, BUF);
        if (r == -1)
            return -1;
        if (r == 0)
            continue;
        if (strcmp(buf, "q") == 0 || strcmp(buf, "Q") == 0)
            return 2;
        errno = 0;
        v = strtod(buf, &oxiri);
        while (*oxiri == ' ' || *oxiri == '\t')
            oxiri++;
        if (oxiri == buf || *oxiri != '\0' || !isfinite(v)) {
            printf("Xato: son kiriting (masalan 12, -3.5) yoki q.\n");
            continue;
        }
        if (errno == ERANGE) {
            printf("Xato: son juda katta.\n");
            continue;
        }
        *natija = v;
        return 1;
    }
}

/* Amal belgisini o'qiydi. 1 - ok, -1 - EOF. */
static int amal_oqish(char *belgi)
{
    char buf[BUF];
    int r;

    for (;;) {
        r = qator_oqish("Amal (+ - * / %): ", buf, BUF);
        if (r == -1)
            return -1;
        if (r == 0)
            continue;
        if (strlen(buf) != 1 || amal_tanla(buf[0]) == NULL) {
            printf("Xato: amal faqat + - * / %% dan biri bo'lishi kerak.\n");
            continue;
        }
        *belgi = buf[0];
        return 1;
    }
}

/* ---------- Asosiy dastur ---------- */
int main(void)
{
    double a, b, natija;
    char belgi;
    int r;
    double (*amal)(double, double); /* funksiya pointeri */

    printf("=== SIKLLI KALKULYATOR ===\n(to'xtatish uchun son o'rniga q kiriting)\n");
    for (;;) {
        r = son_oqish("\nBirinchi son: ", &a);
        if (r != 1)
            break;
        r = son_oqish("Ikkinchi son: ", &b);
        if (r != 1)
            break;
        if (amal_oqish(&belgi) != 1)
            break;

        /* nolga bo'lish tekshiruvi (/ va % uchun) */
        if ((belgi == '/' || belgi == '%') && b == 0.0) {
            printf("Xato: nolga bo'lish mumkin emas.\n");
            continue;
        }

        amal = amal_tanla(belgi);   /* pointer amal belgisiga qarab tanlanadi */
        natija = amal(a, b);        /* funksiya pointer orqali chaqiriladi */

        printf("Natija: %g %c %g = %g\n", a, belgi, b, natija);
        printf("--- Xotira manzillari ---\n");
        printf("natija o'zgaruvchisi &natija = %p\n", (void *)&natija);
        printf("pointer o'zgaruvchisi &amal   = %p\n", (void *)&amal);
    }
    printf("Dastur tugadi.\n");
    return 0;
}
