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


typedef struct {
    int index;
    char name[128];
    char answer[128];
} QuizData;

QuizData g_quiz[100];
int g_quiz_count = 0;

#define MAX_LINE_LENGTH 512
#define MAX_ITEMS 100
#define DELIMITER ","

int find_t_index(const char* frag) {
    const char* first_t = strchr(frag, 'T');
    if (first_t == NULL) {
        return 0;
    }
    else {
        return (int)(first_t - frag);
    }
}

void get_csv_field(char* line, int n, char* destination, size_t dest_size) {
    char temp_line[MAX_LINE_LENGTH];

#ifdef _MSC_VER
    
    strncpy_s(temp_line, MAX_LINE_LENGTH, line, _TRUNCATE);
    char* context = NULL;
    char* token = strtok_s(temp_line, DELIMITER, &context);
#else
    
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    char* token = strtok(temp_line, DELIMITER);
#endif

    int current_field = 0;
    destination[0] = '\0';

    while (token != NULL) {
        if (current_field == n) {
#ifdef _MSC_VER
            strncpy_s(destination, dest_size, token, _TRUNCATE);
#else
            strncpy(destination, token, dest_size - 1);
            destination[dest_size - 1] = '\0';
#endif
            return;
        }

#ifdef _MSC_VER
        token = strtok_s(NULL, DELIMITER, &context);
#else
        token = strtok(NULL, DELIMITER);
#endif

        current_field++;
    }
}

// -------------------------------------------------------------------------
//  문제 1
// -------------------------------------------------------------------------
void load_quiz_1_answer() {
    FILE* fp = fopen("AI1-2_C_Final.csv", "r");
    if (fp == NULL) {
        printf("\n[ERROR] 'AI1-2_C_Final.csv' 파일을 찾을 수 없습니다!\n");
        return;
    }

    char line[MAX_LINE_LENGTH];


    if (fgets(line, sizeof(line), fp) == NULL) {
        fclose(fp);
        return;
    }

    char item_names[MAX_ITEMS][32];
    int item_name_count = 0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        line[strcspn(line, "\r\n")] = 0;
        if (line[0] == '\0') continue;

        char* token;
        char temp_buffer[MAX_LINE_LENGTH];
        strcpy(temp_buffer, line);

        char item_name[32] = "";
        int atk = 0, def = 0, hp = 0;
        int field_index = 0;

#ifdef _MSC_VER
        char* context = NULL;
        token = strtok_s(temp_buffer, DELIMITER, &context);
#else
        token = strtok(temp_buffer, DELIMITER);
#endif

        while (token != NULL) {
            field_index++;

            switch (field_index) {
            case 2: // NAME
                strncpy(item_name, token, 31);
                item_name[31] = '\0';
                break;
            case 4: // ATK
                atk = atoi(token);
                break;
            case 5: // DEF
                def = atoi(token);
                break;
            case 6: // HP
                hp = atoi(token);
                break;
            }

#ifdef _MSC_VER
            token = strtok_s(NULL, DELIMITER, &context);
#else
            token = strtok(NULL, DELIMITER);
#endif
        }

        if (field_index < 6) continue;

        // 문제 1 조건: ATK >= 4 && DEF <= 5 && HP <= 100
        if (atk >= 4 && def <= 5 && hp <= 100) {
            if (item_name_count < MAX_ITEMS) {
                strcpy(item_names[item_name_count], item_name);
                item_name_count++;
            }
        }
    }

    fclose(fp);

    if (item_name_count > 0) {
     
        g_quiz[g_quiz_count].index = 1;
        strcpy(g_quiz[g_quiz_count].name, "Quiz 1 Answer");
        g_quiz[g_quiz_count].answer[0] = '\0';

        
        for (int i = item_name_count - 1; i >= 0; i--) {
            strcat(g_quiz[g_quiz_count].answer, item_names[i]);
            if (i > 0) {
                strcat(g_quiz[g_quiz_count].answer, "|");
            }
        }
        g_quiz_count++;
        printf("TEAM-BETA [System]: 퀴즈 1 (Index 1) 정답 로드 완료.\n");
    }
}

// -------------------------------------------------------------------------
//  문제 2:
// -------------------------------------------------------------------------
void load_quiz_2_answer() {
    FILE* fp;
    const char* filename = "AI1-2_C_Final.csv";

#ifdef _MSC_VER
    if (fopen_s(&fp, filename, "r") != 0 || fp == NULL) {
#else
    fp = fopen(filename, "r");
    if (fp == NULL) {
#endif
        printf("\n[ERROR] 'AI1-2_C_Final.csv' 파일을 찾을 수 없습니다!\n");
        return;
    }

    char line[MAX_LINE_LENGTH];
    char slot_field[MAX_LINE_LENGTH];
    char key_frag_field[MAX_LINE_LENGTH];

    int total_N = 0;

   
    if (fgets(line, sizeof(line), fp) == NULL) {
        fclose(fp);
        return;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        line[strcspn(line, "\r\n")] = 0;

        
        get_csv_field(line, 2, slot_field, sizeof(slot_field));

        if (strcmp(slot_field, "W") == 0) {
           
            get_csv_field(line, 7, key_frag_field, sizeof(key_frag_field));

            int index = find_t_index(key_frag_field);
            total_N += index;
        }
    }

    fclose(fp);


    g_quiz[g_quiz_count].index = 2;
    strcpy(g_quiz[g_quiz_count].name, "Quiz 2 Answer");

    char num_str[10];
#ifdef _MSC_VER
    sprintf_s(num_str, sizeof(num_str), "%d", total_N);
#else
    sprintf(num_str, "%d", total_N);
#endif

    strcpy(g_quiz[g_quiz_count].answer, num_str);
    strcat(g_quiz[g_quiz_count].answer, "key");

    g_quiz_count++;
    printf("TEAM-BETA [System]: 퀴즈 2 (Index 2) 정답 로드 완료.\n");
    }


int my_secret_key_B;

void student2_ai_entry() {
    my_secret_key_B = register_player_ai("TEAM-BETA", student2_ai_logic);

    load_quiz_1_answer();
    load_quiz_2_answer();

    printf("TEAM-BETA: 준비 완료.\n");
    getchar();
}