//학생의 Action의 return 값은 
//return Wait(hp, name);
//return Heal(hp, name);
//둘중 하나여야함

#include "ES1_1101.h"
#include <stdio.h>
#include <stdlib.h> 


int ES1action(int hp, char* name) {
    printf(">>> [%s (mm)] 턴 (현재 HP: %d) <<<\n", name, hp);

    // ES1의 전략: HP가 30 이하일 때 회복을 시도하고, 그 외에는 공격(Wait)을 시도합니다.
    if (hp <= 30) {
        printf("[%s]가 위험을 느껴 '긴급 회복'을 시도합니다!\n", name);
        // 조건: return Heal(hp, name);
        return Heal(hp, name);
    }
    else {
        printf("[%s]가 상대에게 '강력한 공격'을 시도합니다.\n", name);
        // 조건: return Wait(hp, name);
        return Wait(hp, name);
    }
} 