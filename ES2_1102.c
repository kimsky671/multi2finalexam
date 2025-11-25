#include "ES2_1102.h" 

 // --- ES2action 함수 정의 ---
int ES2action(int hp, char* name)
{

    if (hp < 30) {
        // Heal 함수는 Action.obj에 정의되어 있습니다.
        return Heal(hp, name);
    }
   
    else {
        // Wait 함수는 Action.obj에 정의되어 있습니다.
        return Heal(hp, name);
    }
}