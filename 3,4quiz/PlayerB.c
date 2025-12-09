#define _CRT_SECURE_NO_WARNINGS
#include "api.h"
#include <stdio.h>
#include <stdlib.h>


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

const char* CANDIDATE_FILES[] = {
    "quiz_data.csv", "Test_data.csv", "game_puzzle_en.csv", "data.csv", "final_exam.csv"
};
const int NUM_CANDIDATES = sizeof(CANDIDATE_FILES) / sizeof(CANDIDATE_FILES[0]);

void load_quiz_data() {
    FILE* fp = NULL;
    char line[512];

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

    fgets(line, sizeof(line), fp);
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
    printf("TEAM-BETA [System]: 데이터 %d개 로드 완료.\n", g_quiz_count);
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

    if (my_strlen(found_key) > 0) {
        attempt_skill_unlock(player_key, CMD_BLINK, found_key);
        attempt_skill_unlock(player_key, CMD_BLINK_DOWN, found_key);
        attempt_skill_unlock(player_key, CMD_BLINK_LEFT, found_key);
        attempt_skill_unlock(player_key, CMD_BLINK_RIGHT, found_key);
    }

    if (is_skill_unlocked(player_key, CMD_BLINK))
        printf("TEAM-BETA [Success]: 점멸 4종 해금 완료!\n");
}

void unlock_heal2_skill(int player_key) {
    if (g_quiz_count == 0) return;
    const char* found_key = get_answer_by_index(13);

    if (my_strlen(found_key) > 0) {
        attempt_skill_unlock(player_key, CMD_HEAL2, found_key);
    }

    if (is_skill_unlocked(player_key, CMD_HEAL2))
        printf("TEAM-BETA [Success]: 회복2 해금 완료!\n");
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
