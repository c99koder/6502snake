#!/bin/bash
cl65 -D__REDPOWER__ --cpu 65816 -Osir -T -t none -c -o snake.o snake.c
ld65 -o snake.img -C rpc8e.cfg -m snake.map snake.o rpc8e.lib
