#define _CRT_SECURE_NO_WARNINGS
#include "api.h"
#include <stdio.h>
#include <stdlib.h>


typedef struct {
    int index;
    char name[128];
    char answer[128];
} QuizData;


QuizData g_quiz[100];
int g_quiz_count = 0;
int my_secret_key_B;



int my_strlen(const char* str) {
    int len = 0;
    while (str[len] != '\0') len++;
    return len;
}

void my_strcpy(char* dest, const char* src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

// 유틸리티: 문자열 끝 줄바꿈 제거
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


// 해당 파트는 혹시나! csv파일 명을 여려개로 고정한 상태로 찾는 코드입니다. 
// 그냥 지정단 파일만 로드는 아래 코드를 참고해주세요
/*
void load_quiz_data() {
    FILE* fp = fopen("quiz_data.csv", "r");

    if (fp == NULL) {
        printf("\n[ERROR] 'quiz_data.csv' 파일을 찾을 수 없습니다!\n");
        return;
    }

    char line[512];
*/
const char* CANDIDATE_FILES[] = {
    "quiz_data.csv", "Test_data.csv", "game_puzzle_en.csv", "data.csv", "final_exam.csv"
};
const int NUM_CANDIDATES = sizeof(CANDIDATE_FILES) / sizeof(CANDIDATE_FILES[0]);


void load_quiz_data() {
    FILE* fp = NULL;
    char line[512];

    // 파일 찾기
    for (int i = 0; i < NUM_CANDIDATES; i++) {
        fp = fopen(CANDIDATE_FILES[i], "r");
        if (fp != NULL) {
            printf("TEAM-BETA [System]: 파일 로드 성공 -> %s\n", CANDIDATE_FILES[i]);
            break;
        }
    }
    if (fp == NULL) {
        printf("\n[ERROR] 데이터 파일을 찾을 수 없습니다!\n");
        return;
    }

    
    fgets(line, sizeof(line), fp); // 헤더 스킵
    g_quiz_count = 0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        char* ptr = line;
        char* token_start = ptr;

       
        while (*ptr != ',' && *ptr != '\0') ptr++;
        if (*ptr == '\0') continue;
        *ptr = '\0';
        g_quiz[g_quiz_count].index = atoi(token_start);

        
        ptr++;
        token_start = ptr;
        while (*ptr != ',' && *ptr != '\0') ptr++;
        if (*ptr == '\0') continue;
        *ptr = '\0';
        my_strcpy(g_quiz[g_quiz_count].name, token_start);

       
        ptr++;
        token_start = ptr;
        my_strcpy(g_quiz[g_quiz_count].answer, token_start);
        trim_newline(g_quiz[g_quiz_count].answer);

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

// ==========================================================
// 2. 문제 3 해금 로직 (점멸 - 스킬 8~11)
// ==========================================================
void unlock_blink_skill(int player_key) {
    if (g_quiz_count == 0) return;


    const char* found_key = get_answer_by_index(8);

    printf("TEAM-BETA [Debug]: 점멸(Index 8) 키 찾음 = [%s]\n", found_key);

    if (my_strlen(found_key) > 0) {
        // 8~11번 스킬 모두 시도
        attempt_skill_unlock(player_key, CMD_BLINK_UP, found_key);    
        attempt_skill_unlock(player_key, CMD_BLINK_DOWN, found_key);  
        attempt_skill_unlock(player_key, CMD_BLINK_LEFT, found_key);  
        attempt_skill_unlock(player_key, CMD_BLINK_RIGHT, found_key); 
    }
    else {
        printf("TEAM-BETA [Error]: 점멸 정답(Index 8)을 찾지 못했습니다.\n");
    }

    if (is_skill_unlocked(player_key, CMD_BLINK_UP))
        printf("TEAM-BETA [Success]: 점멸 4종 해금 성공!\n");
}

// ==========================================================
// 3. 문제 4 해금 로직 (회복2 - 스킬 13)
// ==========================================================
void unlock_heal2_skill(int player_key) {
    if (g_quiz_count == 0) return;

   
    const char* found_key = get_answer_by_index(13);

    printf("TEAM-BETA [Debug]: 회복2(Index 13) 키 찾음 = [%s]\n", found_key);

    if (my_strlen(found_key) > 0) {
        
        attempt_skill_unlock(player_key, CMD_HEAL_ALL, found_key);
    }
    else {
        printf("TEAM-BETA [Error]: 회복2 정답(Index 13)을 찾지 못했습니다.\n");
    }

    if (is_skill_unlocked(player_key, CMD_HEAL_ALL))
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

    // 체력이 낮고(2 이하), MP가 충분하면(2 이상) 회복2 사용 
    if (get_player_hp(my_info) <= 2 && get_player_mp(my_info) >= 2) {
        if (is_skill_unlocked(my_secret_key_B, CMD_HEAL_ALL)) {
            return CMD_HEAL_ALL;
        }
        return CMD_REST;
    }

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
