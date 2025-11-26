//20251388 김민서

#include "ES1_1101.h"
#include <stdio.h>
#include <stdlib.h> 


int ES1action(int hp, char* name) {
    printf(">>> [%s (mm)] 턴 (현재 HP: %d) <<<\n", name, hp);

    if (hp <= 30) {
        printf("[%s]가 위험을 느껴 '긴급 회복'을 시도합니다!\n", name);
        return Heal(hp, name);
    }
    else {
        printf("[%s]가 상대에게 '강력한 공격'을 시도합니다.\n", name);
        return Wait(hp, name);
    }
} 