#define _CRT_SECURE_NO_WARNINGS
#include "api.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef CMD_UP
#define CMD_UP 1
#define CMD_DOWN 2
#define CMD_LEFT 3
#define CMD_RIGHT 4
#define CMD_ATTACK 5
#endif


#ifndef CMD_BLINK
#define CMD_BLINK 8         
#endif
#ifndef CMD_BLINK_DOWN
#define CMD_BLINK_DOWN 9
#endif
#ifndef CMD_BLINK_LEFT
#define CMD_BLINK_LEFT 10
#endif
#ifndef CMD_BLINK_RIGHT
#define CMD_BLINK_RIGHT 11
#endif


#ifndef CMD_HEAL2
#define CMD_HEAL2 13        
#endif


typedef struct {
    int index;          
    char name[128];     
    char answer[128];   
} QuizData;

QuizData g_quiz[100];
int g_quiz_count = 0;

// 문자열 유틸리티 (줄바꿈 제거)
void trim_newline(char* str) {
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == '\r' || str[i] == '\n') {
            str[i] = '\0';
            break;
        }
        i++;
    }
}


void load_quiz_data() {
    FILE* fp = fopen("quiz_data.csv", "r"); 
    if (fp == NULL) {
        printf("\n[ERROR] 'quiz_data.csv' 파일을 찾을 수 없습니다!\n");
        return;
    }

    char line[512];
   
    fgets(line, sizeof(line), fp);

    g_quiz_count = 0;
    while (fgets(line, sizeof(line), fp) != NULL) {
        
        char* token = strtok(line, ",");
        if (token == NULL) continue;

        g_quiz[g_quiz_count].index = atoi(token);

        token = strtok(NULL, ",");
        if (token) strcpy(g_quiz[g_quiz_count].name, token); 

        
        token = strtok(NULL, ",");
        if (token) {
            strcpy(g_quiz[g_quiz_count].answer, token);
            trim_newline(g_quiz[g_quiz_count].answer);
        }

        g_quiz_count++;
    }
    fclose(fp);

    printf("TEAM-BETA [System]: 퀴즈 데이터 %d개 로드 완료.\n", g_quiz_count);
}


const char* get_answer_by_index(int target_index) {
    for (int i = 0; i < g_quiz_count; i++) {
        if (g_quiz[i].index == target_index) {
            return g_quiz[i].answer;
        }
    }
    return ""; 
}


void unlock_blink_skill(int player_key) {
    if (g_quiz_count == 0) return;

    
    const char* found_key = get_answer_by_index(8);

    printf("TEAM-BETA [Debug]: 점멸(Index 8) 키 찾음 = [%s]\n", found_key);

    if (strlen(found_key) > 0) {
       
        attempt_skill_unlock(player_key, CMD_BLINK, found_key);       
        attempt_skill_unlock(player_key, CMD_BLINK_DOWN, found_key);  
        attempt_skill_unlock(player_key, CMD_BLINK_LEFT, found_key); 
        attempt_skill_unlock(player_key, CMD_BLINK_RIGHT, found_key); 
    }
    else {
        printf("TEAM-BETA [Error]: 점멸 정답(Index 8)을 찾지 못했습니다.\n");
    }

    if (is_skill_unlocked(player_key, CMD_BLINK))
        printf("TEAM-BETA [Success]: 점멸 4종 해금 성공!\n");
}

// [문제 4] 회복2(13) 해금 로직
void unlock_heal2_skill(int player_key) {
    if (g_quiz_count == 0) return;

   
    const char* found_key = get_answer_by_index(13);

    printf("TEAM-BETA [Debug]: 회복2(Index 13) 키 찾음 = [%s]\n", found_key);

    if (strlen(found_key) > 0) {
        attempt_skill_unlock(player_key, CMD_HEAL2, found_key);
    }
    else {
        printf("TEAM-BETA [Error]: 회복2 정답(Index 13)을 찾지 못했습니다.\n");
    }

    if (is_skill_unlocked(player_key, CMD_HEAL2))
        printf("TEAM-BETA [Success]: 회복2(ID 13) 해금 완료.\n");
}

// 해당 파트는 제가 임의로 짠 공격로직입니다. 지우셔도 무방합니다.
static int get_dist(const Player* p1, const Player* p2) {
    int dx = abs(get_player_x(p1) - get_player_x(p2));
    int dy = abs(get_player_y(p1) - get_player_y(p2));
    return dx + dy;
}

int student2_ai_logic(const Player* my_info, const Player* opponent_info) {
    int dist = get_dist(my_info, opponent_info);

    // 거리 1 이내면 공격
    if (dist <= 1) return CMD_ATTACK;

    int my_x = get_player_x(my_info);
    int opp_x = get_player_x(opponent_info);
    int my_y = get_player_y(my_info);
    int opp_y = get_player_y(opponent_info);

    // 추격
    if (my_x != opp_x) return (my_x < opp_x) ? CMD_RIGHT : CMD_LEFT;
    if (my_y != opp_y) return (my_y < opp_y) ? CMD_DOWN : CMD_UP;

    return CMD_ATTACK;
}


// 메인 진입점
int my_secret_key_B;

void student2_ai_entry() {
    my_secret_key_B = register_player_ai("TEAM-BETA", student2_ai_logic);

    
    load_quiz_data();
 
    if (g_quiz_count > 0) {
        unlock_blink_skill(my_secret_key_B);
        unlock_heal2_skill(my_secret_key_B);
    }
    else {
        printf("TEAM-BETA [Error]: 데이터 로드 실패.\n");
    }

    printf("TEAM-BETA: 준비 완료.\n");
    getchar(); 
}