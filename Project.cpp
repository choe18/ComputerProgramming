#include <stdio.h>
#include <string.h>
#include <time.h>

// ��ǰ ���� �Է¹ޱ� 
struct Food {
    char name[50];
    char purchaseDate[11];
    char expiryDate[11];
};

// ��¥ ���� ���
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

// ���  
void displayMenu() {
    printf("\n=================== �޴� ===================\n");
    printf("1. ��ǰ �߰�\n");
    printf("2. ������� �ӹ��� ������ ����\n");
    printf("3. ��ǰ ���� ����\n");
    printf("4. ��ǰ ����\n");
    printf("5. ������� ���� ��ǰ ����\n");
    printf("6. ����\n");
    printf("===========================================\n");
    printf("����: ");
}

// ��ǰ �߰�
void addFood(struct Food *foodList, int *foodCount) {
    struct Food newFood;
    printf("\n��ǰ��: ");
    scanf("%s", newFood.name);
    printf("������ (YYYY-MM-DD): ");
    scanf("%s", newFood.purchaseDate);
    printf("������� (YYYY-MM-DD): ");
    scanf("%s", newFood.expiryDate);

    foodList[*foodCount] = newFood;
    (*foodCount)++;
    printf("��ǰ�� �߰��Ǿ����ϴ�.\n");
}

// ��ǰ ���� ����
void editFood(struct Food *foodList, int foodCount) {
    char target[50];
    printf("\n������ ��ǰ���� �Է��ϼ���: ");
    scanf("%s", target);

    int found = 0;
    for (int i = 0; i < foodCount; i++) {
        if (strcmp(foodList[i].name, target) == 0) {
            printf("���� ���� - ��ǰ��: %s, ������: %s, �������: %s\n",
                   foodList[i].name, foodList[i].purchaseDate, foodList[i].expiryDate);

            printf("�� ��ǰ��: ");
            scanf("%s", foodList[i].name);
            printf("�� ������ (YYYY-MM-DD): ");
            scanf("%s", foodList[i].purchaseDate);
            printf("�� ������� (YYYY-MM-DD): ");
            scanf("%s", foodList[i].expiryDate);

            printf("��ǰ ������ �����Ǿ����ϴ�.\n");
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("�ش� �̸��� ��ǰ�� �����ϴ�.\n");
    }
}

// ��ǰ ����
void deleteFood(struct Food *foodList, int *foodCount) {
    char target[50];
    printf("\n������ ��ǰ���� �Է��ϼ���: ");
    scanf("%s", target);

    int found = 0;
    for (int i = 0; i < *foodCount; i++) {
        if (strcmp(foodList[i].name, target) == 0) {
            for (int j = i; j < *foodCount - 1; j++) {
                foodList[j] = foodList[j + 1];
            }
            (*foodCount)--;
            printf("��ǰ�� �����Ǿ����ϴ�.\n");
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("�ش� �̸��� ��ǰ�� �����ϴ�.\n");
    }
}

// ������� ���� ��ǰ ����
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
    printf("%d���� ������� ���� ��ǰ�� �����Ǿ����ϴ�.\n", originalCount - *foodCount);
}

// ������� ����� ������ ����
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

    printf("\n%-15s %-12s %-12s %-10s\n", "��ǰ��", "������", "�������", "���� �ϼ�");
    printf("-------------------------------------------------\n");

    for (int i = 0; i < foodCount; i++) {
        int daysLeft = date_diff(foodList[i].expiryDate);
        if (daysLeft < 0) {
            printf("%-15s %-12s %-12s %-10s\n",
                   foodList[i].name, foodList[i].purchaseDate, foodList[i].expiryDate, "������� �ʰ�");
        } else {
            printf("%-15s %-12s %-12s %-10d\n",
                   foodList[i].name, foodList[i].purchaseDate, foodList[i].expiryDate, daysLeft);
        }
    }
    printf("\n");
}

// �Է� ���� ���� ���� 
void saveToFile(struct Food *foodList, int foodCount) {
    FILE *fp = fopen("food_data.txt", "w");
    if (fp == NULL) {
        printf("���� ���� ����!\n");
        return;
    }

    for (int i = 0; i < foodCount; i++) {
        fprintf(fp, "%s %s %s\n",
                foodList[i].name,
                foodList[i].purchaseDate,
                foodList[i].expiryDate);
    }

    fclose(fp);
    printf("��ǰ ������ ���Ͽ� ����Ǿ����ϴ�.\n");
}

// ����� ���� �ҷ�����  
void loadFromFile(struct Food *foodList, int *foodCount) {
    FILE *fp = fopen("food_data.txt", "r");
    if (fp == NULL) {
        printf("����� ������ �����ϴ�. ���� �����մϴ�.\n");
        return;
    }

    while (fscanf(fp, "%s %s %s",
                  foodList[*foodCount].name,
                  foodList[*foodCount].purchaseDate,
                  foodList[*foodCount].expiryDate) == 3) {
        (*foodCount)++;
    }

    fclose(fp);
    printf("%d���� ��ǰ ������ �ҷ��Խ��ϴ�.\n", *foodCount);
}

// ����
int main() {
    struct Food foodList[100];
    int foodCount = 0;
    int choice;

    printf("=== ����� ��ǰ ���� ���α׷� ===\n");

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
                printf("���α׷��� �����մϴ�.\n");
                return 0;
            default:
                printf("�߸��� �����Դϴ�. �ٽ� �Է����ּ���.\n");
        }
    }

    return 0;
}


