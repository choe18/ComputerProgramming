#include <stdio.h>
#include <string.h>
#include <time.h>

// 식품 정보 입력받기 
struct Food {
    char name[50];
    char purchaseDate[11];
    char expiryDate[11];
};

// 날짜 차이 계산
int date_diff(char *expiryDate) {
    struct tm expiry = {0};
    struct tm today = {0};
    time_t now;

    time(&now);
    struct tm *localTime = localtime(&now);

    today.tm_year = localTime->tm_year;
    today.tm_mon = localTime->tm_mon;
    today.tm_mday = localTime->tm_mday;
    today.tm_hour = 0;
    today.tm_min = 0;
    today.tm_sec = 0;
    time_t todayTime = mktime(&today);

    int y, m, d;
    sscanf(expiryDate, "%d-%d-%d", &y, &m, &d);
    expiry.tm_year = y - 1900;
    expiry.tm_mon = m - 1;
    expiry.tm_mday = d;
    expiry.tm_hour = 0;
    expiry.tm_min = 0;
    expiry.tm_sec = 0;
    time_t expiryTime = mktime(&expiry);

    double seconds = difftime(expiryTime, todayTime);
    int days = (int)(seconds / (60 * 60 * 24));

    return days;
}

// 기능  
void displayMenu() {
    printf("\n=================== 메뉴 ===================\n");
    printf("1. 식품 추가\n");
    printf("2. 유통기한 임박한 순으로 보기\n");
    printf("3. 식품 정보 수정\n");
    printf("4. 식품 삭제\n");
    printf("5. 유통기한 지난 식품 삭제\n");
    printf("6. 종료\n");
    printf("===========================================\n");
    printf("선택: ");
}

// 식품 추가
void addFood(struct Food *foodList, int *foodCount) {
    struct Food newFood;
    printf("\n식품명: ");
    scanf("%s", newFood.name);
    printf("구매일 (YYYY-MM-DD): ");
    scanf("%s", newFood.purchaseDate);
    printf("유통기한 (YYYY-MM-DD): ");
    scanf("%s", newFood.expiryDate);

    foodList[*foodCount] = newFood;
    (*foodCount)++;
    printf("식품이 추가되었습니다.\n");
}

// 식품 정보 수정
void editFood(struct Food *foodList, int foodCount) {
    char target[50];
    printf("\n수정할 식품명을 입력하세요: ");
    scanf("%s", target);

    int found = 0;
    for (int i = 0; i < foodCount; i++) {
        if (strcmp(foodList[i].name, target) == 0) {
            printf("기존 정보 - 식품명: %s, 구매일: %s, 유통기한: %s\n",
                   foodList[i].name, foodList[i].purchaseDate, foodList[i].expiryDate);

            printf("새 식품명: ");
            scanf("%s", foodList[i].name);
            printf("새 구매일 (YYYY-MM-DD): ");
            scanf("%s", foodList[i].purchaseDate);
            printf("새 유통기한 (YYYY-MM-DD): ");
            scanf("%s", foodList[i].expiryDate);

            printf("식품 정보가 수정되었습니다.\n");
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("해당 이름의 식품이 없습니다.\n");
    }
}

// 식품 삭제
void deleteFood(struct Food *foodList, int *foodCount) {
    char target[50];
    printf("\n삭제할 식품명을 입력하세요: ");
    scanf("%s", target);

    int found = 0;
    for (int i = 0; i < *foodCount; i++) {
        if (strcmp(foodList[i].name, target) == 0) {
            for (int j = i; j < *foodCount - 1; j++) {
                foodList[j] = foodList[j + 1];
            }
            (*foodCount)--;
            printf("식품이 삭제되었습니다.\n");
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("해당 이름의 식품이 없습니다.\n");
    }
}

// 유통기한 지난 식품 삭제
void deleteExpiredFoods(struct Food *foodList, int *foodCount) {
    int originalCount = *foodCount;
    for (int i = 0; i < *foodCount; ) {
        if (date_diff(foodList[i].expiryDate) < 0) {
            for (int j = i; j < *foodCount - 1; j++) {
                foodList[j] = foodList[j + 1];
            }
            (*foodCount)--;
        } else {
            i++;
        }
    }
    printf("%d개의 유통기한 지난 식품이 삭제되었습니다.\n", originalCount - *foodCount);
}

// 유통기한 가까운 순으로 보기
void sortByExpiryDate(struct Food *foodList, int foodCount) {
    for (int i = 0; i < foodCount - 1; i++) {
        for (int j = 0; j < foodCount - 1 - i; j++) {
            if (date_diff(foodList[j].expiryDate) > date_diff(foodList[j + 1].expiryDate)) {
                struct Food temp = foodList[j];
                foodList[j] = foodList[j + 1];
                foodList[j + 1] = temp;
            }
        }
    }

    printf("\n%-15s %-12s %-12s %-10s\n", "식품명", "구매일", "유통기한", "남은 일수");
    printf("-------------------------------------------------\n");

    for (int i = 0; i < foodCount; i++) {
        int daysLeft = date_diff(foodList[i].expiryDate);
        if (daysLeft < 0) {
            printf("%-15s %-12s %-12s %-10s\n",
                   foodList[i].name, foodList[i].purchaseDate, foodList[i].expiryDate, "유통기한 초과");
        } else {
            printf("%-15s %-12s %-12s %-10d\n",
                   foodList[i].name, foodList[i].purchaseDate, foodList[i].expiryDate, daysLeft);
        }
    }
    printf("\n");
}

// 입력 받은 내용 저장 
void saveToFile(struct Food *foodList, int foodCount) {
    FILE *fp = fopen("food_data.txt", "w");
    if (fp == NULL) {
        printf("내용 저장 실패!\n");
        return;
    }

    for (int i = 0; i < foodCount; i++) {
        fprintf(fp, "%s %s %s\n",
                foodList[i].name,
                foodList[i].purchaseDate,
                foodList[i].expiryDate);
    }

    fclose(fp);
    printf("식품 정보가 파일에 저장되었습니다.\n");
}

// 저장된 내용 불러오기  
void loadFromFile(struct Food *foodList, int *foodCount) {
    FILE *fp = fopen("food_data.txt", "r");
    if (fp == NULL) {
        printf("저장된 내용이 없습니다. 새로 시작합니다.\n");
        return;
    }

    while (fscanf(fp, "%s %s %s",
                  foodList[*foodCount].name,
                  foodList[*foodCount].purchaseDate,
                  foodList[*foodCount].expiryDate) == 3) {
        (*foodCount)++;
    }

    fclose(fp);
    printf("%d개의 식품 정보를 불러왔습니다.\n", *foodCount);
}

// 메인
int main() {
    struct Food foodList[100];
    int foodCount = 0;
    int choice;

    printf("=== 냉장고 식품 관리 프로그램 ===\n");

    loadFromFile(foodList, &foodCount);

    while (1) {
        displayMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addFood(foodList, &foodCount);
                break;
            case 2:
                sortByExpiryDate(foodList, foodCount);
                break;
            case 3:
                editFood(foodList, foodCount);
                break;
            case 4:
                deleteFood(foodList, &foodCount);
                break;
            case 5:
                deleteExpiredFoods(foodList, &foodCount);
                break;
            case 6:
                saveToFile(foodList, foodCount);
                printf("프로그램을 종료합니다.\n");
                return 0;
            default:
                printf("잘못된 선택입니다. 다시 입력해주세요.\n");
        }
    }

    return 0;
}


