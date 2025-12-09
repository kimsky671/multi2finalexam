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


typedef struct {
    int index;
    char name[128];
    char answer[128];
} QuizData;

QuizData g_quiz[100];
int g_quiz_count = 0;

#define MAX_LINE_LENGTH 512
#define MAX_ITEMS 100
#define DELIMITER ','


int custom_strlen(const char* str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}


void custom_strcpy(char* dest, const char* src, size_t dest_size) {
    size_t i = 0;
    while (i < dest_size - 1 && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}


int custom_strcmp(const char* str1, const char* str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(const unsigned char*)str1 - *(const unsigned char*)str2;
}


void custom_strcat(char* dest, const char* src, size_t dest_size) {
    char* d = dest;
    while (*d != '\0') { 
        d++;
    }
    while (*src != '\0' && (d - dest) < dest_size - 1) { 
        *d++ = *src++;
    }
    *d = '\0';
}


const char* custom_strchr(const char* str, int c) {
    while (*str != '\0') {
        if (*str == (char)c) {
            return str;
        }
        str++;
    }
    if ((char)c == '\0') {
        return str;
    }
    return NULL;
}


void custom_chomp(char* str) {
    int len = custom_strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[len - 1] = '\0';
        len--;
    }
}


const char* get_csv_field_ptr(const char* line, int n) {
    int current_field = 0;
    const char* ptr = line;

   
    while (*ptr != '\0' && current_field < n) {
        if (*ptr == DELIMITER) {
            current_field++;
        }
        ptr++;
    }

    if (current_field == n) {
        return ptr;
    }
    return NULL; 
}


void get_csv_field(const char* line, int n, char* destination, size_t dest_size) {
    const char* start = get_csv_field_ptr(line, n);
    if (start == NULL) {
        destination[0] = '\0';
        return;
    }

    size_t i = 0;
    const char* current = start;

    while (*current != '\0' && *current != DELIMITER && i < dest_size - 1) {
        destination[i] = *current;
        current++;
        i++;
    }
    destination[i] = '\0';
}


int find_t_index(const char* frag) {
    const char* first_t = custom_strchr(frag, 'T');
    if (first_t == NULL) {
        return 0;
    }
    else {
        return (int)(first_t - frag);
    }
}
void custom_itoa(int n, char* buffer) {
    
    int i = 0;
    int divisor = 1;
    int temp = n;

    if (n == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }


    while (temp > 0) {
        temp /= 10;
        if (temp > 0) divisor *= 10;
    }

    
    while (divisor > 0) {
        buffer[i++] = (char)((n / divisor) % 10 + '0');
        divisor /= 10;
    }
    buffer[i] = '\0';
}

// -------------------------------------------------------------------------
// 문제 1
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
        custom_chomp(line); 

        if (line[0] == '\0') continue;

        char item_name[32] = "";
        char str_atk[16] = "", str_def[16] = "", str_hp[16] = "";

        
        get_csv_field(line, 1, item_name, 32); 
        get_csv_field(line, 3, str_atk, 16);    
        get_csv_field(line, 4, str_def, 16);    
        get_csv_field(line, 5, str_hp, 16);     

        
        int atk = atoi(str_atk);
        int def = atoi(str_def);
        int hp = atoi(str_hp);

        // 문제 1 조건: ATK >= 4 && DEF <= 5 && HP <= 100
        if (atk >= 4 && def <= 5 && hp <= 100) {
            if (item_name_count < MAX_ITEMS) {
                custom_strcpy(item_names[item_name_count], item_name, 32);
                item_name_count++;
            }
        }
    }

    fclose(fp);

    if (item_name_count > 0) {
       
        g_quiz[g_quiz_count].index = 1;
        custom_strcpy(g_quiz[g_quiz_count].name, "Quiz 1 Answer", 128);
        g_quiz[g_quiz_count].answer[0] = '\0';

        
        for (int i = item_name_count - 1; i >= 0; i--) {
            custom_strcat(g_quiz[g_quiz_count].answer, item_names[i], 128);
            if (i > 0) {
                custom_strcat(g_quiz[g_quiz_count].answer, "|", 128);
            }
        }
        g_quiz_count++;
        printf("TEAM-BETA [System]: 퀴즈 1 (Index 1) 정답 로드 완료.\n");
    }
}

// -------------------------------------------------------------------------
//  문제 2
// -------------------------------------------------------------------------
void load_quiz_2_answer() {
    FILE* fp = fopen("AI1-2_C_Final.csv", "r");
    if (fp == NULL) {
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
        custom_chomp(line);
        
        get_csv_field(line, 2, slot_field, sizeof(slot_field));

        if (custom_strcmp(slot_field, "W") == 0) {
            
            get_csv_field(line, 7, key_frag_field, sizeof(key_frag_field));

            int index = find_t_index(key_frag_field);
            total_N += index;
        }
    }

    fclose(fp);

    g_quiz[g_quiz_count].index = 2;
    custom_strcpy(g_quiz[g_quiz_count].name, "Quiz 2 Answer", 128);

    char num_str[10];
    custom_itoa(total_N, num_str); 

    custom_strcpy(g_quiz[g_quiz_count].answer, num_str, 128);
    custom_strcat(g_quiz[g_quiz_count].answer, "key", 128);

    g_quiz_count++;
    printf("TEAM-BETA [System]: 퀴즈 2 (Index 2) 정답 로드 완료.\n");
}

// -------------------------------------------------------------------------
// 모션
// -------------------------------------------------------------------------

static int get_dist(const Player* p1, const Player* p2) {
    int dx = abs(get_player_x(p1) - get_player_x(p2));
    int dy = abs(get_player_y(p1) - get_player_y(p2));
    return dx + dy;
}

int student2_ai_logic(const Player* my_info, const Player* opponent_info) {
    int dist = get_dist(my_info, opponent_info);

    
    if (dist <= 1) return CMD_ATTACK;

    int my_x = get_player_x(my_info);
    int opp_x = get_player_x(opponent_info);
    int my_y = get_player_y(my_info);
    int opp_y = get_player_y(opponent_info);

  
    if (my_x != opp_x) return (my_x < opp_x) ? CMD_RIGHT : CMD_LEFT;
    if (my_y != opp_y) return (my_y < opp_y) ? CMD_DOWN : CMD_UP;

    return CMD_ATTACK;
}



int my_secret_key_B;

void student2_ai_entry() {
    my_secret_key_B = register_player_ai("TEAM-BETA", student2_ai_logic);

    load_quiz_1_answer();
    
    load_quiz_2_answer();

    printf("TEAM-BETA: 준비 완료.\n");
    getchar();
}