# OS_T1_V18 — Siklli kalkulyator (Variant 18)

## Kompilyatsiya va ishga tushirish
    gcc -Wall -Wextra -o main main.c -lm
    ./main

## A qism (Hello World, 4 bosqich)
    gcc -E hello.c -o hello.i
    gcc -S hello.i -o hello.s
    gcc -c hello.s -o hello.o
    gcc hello.o -o hello
    ./hello
