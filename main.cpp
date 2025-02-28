#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * @file main.cpp
 * @brief 包含简单的登录系统和雇员工资计算功能
 *
 * 本程序实现了两个主要功能：
 * 1. 登录系统：验证用户名和密码，根据不同情况给出相应提示
 * 2. 雇员工资计算：根据工作时间和时薪计算工资
 *
 * 登录规则如下：
 * 1. 管理员用户名为"hgzy",密码为"1234"，必须都正确才能正常登录
 * 2. 若用户名和密码输入为空，显示"用户和密码不允许为空"
 * 3. 用户名必须输入字母，否则显示"输入非法"
 * 4. 密码必须输入数字，否则显示"输入非法"
 * 5. 若用户名错误，显示"用户名和密码输入错误"，用户名和密码清空
 * 6. 若用户名正确但密码错误，显示"用户名和密码输入错误"，用户名保留，密码清空
 *
 * 工资计算规则如下：
 * 1. 若雇员周工作小时小于40h(0,40)，则按原小时工资0.7来计算薪水
 * 2. 若雇员周工作小时等于40h，则按原小时工资计算薪水
 * 3. 若雇员周工作介于40~50h(40,50]，超过40部分按照每小时工资的1.5倍来计算薪水
 * 4. 若雇员周工作超过50h(50,60]，则超过50部分按原小时工资的3倍来计算薪水
 * 5. 超过60h或小于0h，提示输入有误，重新输入
 *
 * @author [Bozhou Li]
 * @date 2025-02-28
 */

/* 登录系统常量定义 */
#define USERNAME_MAX_LENGTH 32    /* 用户名最大长度 */
#define PASSWORD_MAX_LENGTH 32    /* 密码最大长度 */
#define VALID_USERNAME "hgzy"     /* 有效的用户名 */
#define VALID_PASSWORD "1234"     /* 有效的密码 */

/* 工资计算常量定义 */
#define MIN_HOURS 0.0          /* 最小工作时间 */
#define STANDARD_HOURS 40.0    /* 标准工作时间 */
#define OVERTIME_THRESHOLD 50.0 /* 加班时间阈值 */
#define MAX_HOURS 60.0         /* 最大工作时间 */
#define REDUCED_RATE 0.7       /* 低于标准时间的费率系数 */
#define OVERTIME_RATE 1.5      /* 加班费率系数 */
#define HEAVY_OVERTIME_RATE 3.0 /* 高强度加班费率系数 */

/* 登录状态码 */
typedef enum {
    LOGIN_SUCCESS = 0,                /* 登录成功 */
    LOGIN_ERROR_EMPTY = 1,            /* 用户名或密码为空 */
    LOGIN_ERROR_INVALID_FORMAT = 2,   /* 输入格式非法 */
    LOGIN_ERROR_WRONG_USERNAME = 3,   /* 用户名错误 */
    LOGIN_ERROR_WRONG_PASSWORD = 4    /* 密码错误 */
} LoginStatus;

/* 系统功能菜单选项 */
typedef enum {
    MENU_EXIT = 0,                    /* 退出系统 */
    MENU_SALARY_CALCULATION = 1,      /* 工资计算 */
    MENU_ABOUT = 2,                   /* 关于系统 */
    MENU_INVALID = 3                  /* 无效选择 */
} MenuOption;

/**
 * @brief 检查字符串是否为空
 *
 * @param str 要检查的字符串
 * @return int 1表示为空，0表示非空
 */
int isEmpty(const char *str) {
    return (str == NULL || str[0] == '\0');
}

/**
 * @brief 检查字符串是否全为字母
 *
 * @param str 要检查的字符串
 * @return int 1表示全为字母，0表示不全为字母
 */
int isAllLetters(const char *str) {
    while (*str) {
        if (!isalpha((unsigned char)*str))
            return 0;
        str++;
    }
    return 1;
}

/**
 * @brief 检查字符串是否全为数字
 *
 * @param str 要检查的字符串
 * @return int 1表示全为数字，0表示不全为数字
 */
int isAllDigits(const char *str) {
    while (*str) {
        if (!isdigit((unsigned char)*str))
            return 0;
        str++;
    }
    return 1;
}

/**
 * @brief 验证登录信息
 *
 * @param username 用户名
 * @param password 密码
 * @return LoginStatus 登录状态码
 */
LoginStatus validateLogin(const char *username, const char *password) {
    /* 检查用户名和密码是否为空 */
    if (isEmpty(username) || isEmpty(password))
        return LOGIN_ERROR_EMPTY;

    /* 检查用户名是否全为字母 */
    if (!isAllLetters(username))
        return LOGIN_ERROR_INVALID_FORMAT;

    /* 检查密码是否全为数字 */
    if (!isAllDigits(password))
        return LOGIN_ERROR_INVALID_FORMAT;

    /* 验证用户名和密码 */
    if (strcmp(username, VALID_USERNAME) != 0)
        return LOGIN_ERROR_WRONG_USERNAME;

    if (strcmp(password, VALID_PASSWORD) != 0)
        return LOGIN_ERROR_WRONG_PASSWORD;

    return LOGIN_SUCCESS;
}

/**
 * @brief 处理登录状态并显示相应消息
 *
 * @param status 登录状态码
 * @param username 用户名
 * @param password 密码
 * @return int 1表示需要继续登录，0表示登录成功或退出
 */
int handleLoginStatus(LoginStatus status, char *username, char *password) {
    switch (status) {
        case LOGIN_SUCCESS:
            printf("\n登录成功！欢迎管理员 %s\n", username);
            return 0;

        case LOGIN_ERROR_EMPTY:
            printf("\n错误：用户和密码不允许为空\n");
            return 1;

        case LOGIN_ERROR_INVALID_FORMAT:
            printf("\n错误：输入非法\n");
            printf("用户名必须全部为字母，密码必须全部为数字\n");
            return 1;

        case LOGIN_ERROR_WRONG_USERNAME:
            printf("\n错误：用户名和密码输入错误\n");
            printf("用户名和密码已清空\n");
            username[0] = '\0';
            password[0] = '\0';
            return 1;

        case LOGIN_ERROR_WRONG_PASSWORD:
            printf("\n错误：用户名和密码输入错误\n");
            printf("用户名已保留，密码已清空\n");
            password[0] = '\0';
            return 1;

        default:
            printf("\n未知错误\n");
            return 1;
    }
}

/**
 * @brief 从标准输入安全地读取一行
 *
 * @param buffer 目标缓冲区
 * @param size 缓冲区大小
 * @return char* 成功时返回buffer，失败时返回NULL
 */
char *safeGetLine(char *buffer, size_t size) {
    if (fgets(buffer, size, stdin) == NULL)
        return NULL;

    /* 移除换行符 */
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n')
        buffer[len - 1] = '\0';

    return buffer;
}

/**
 * @brief 验证工作小时数是否在有效范围内
 *
 * @param hours 工作小时数
 * @return int 1表示有效，0表示无效
 */
int validateHours(double hours) {
    return (hours >= MIN_HOURS && hours <= MAX_HOURS);
}

/**
 * @brief 计算雇员工资
 *
 * @param hours 工作小时数
 * @param rate 每小时工资率
 * @return double 计算得出的工资
 */
double calculateSalary(double hours, double rate) {
    double salary = 0.0;

    if (hours < STANDARD_HOURS)
        salary = hours * rate * REDUCED_RATE;   /* 工作时间小于标准时间 */
    else if (hours == STANDARD_HOURS)
        salary = hours * rate;                  /* 工作时间等于标准时间 */
    else if (hours <= OVERTIME_THRESHOLD)
        salary = STANDARD_HOURS * rate +        /* 工作时间介于标准时间和加班阈值之间 */
                (hours - STANDARD_HOURS) * rate * OVERTIME_RATE;
    else
        salary = STANDARD_HOURS * rate +        /* 工作时间介于加班阈值和最大工作时间之间 */
                (OVERTIME_THRESHOLD - STANDARD_HOURS) * rate * OVERTIME_RATE +
                (hours - OVERTIME_THRESHOLD) * rate * HEAVY_OVERTIME_RATE;

    return salary;
}

/**
 * @brief 执行工资计算功能
 */
void runSalaryCalculation() {
    double hours = 0.0;     /* 工作小时数 */
    double hourlyRate = 0.0; /* 每小时工资率 */
    double salary = 0.0;     /* 计算得出的工资 */
    char buffer[100];        /* 输入缓冲区 */
    int valid_input = 0;     /* 输入是否有效 */

    printf("\n=== 雇员工资计算 ===\n\n");

    /* 获取并验证工作小时数 */
    do {
        printf("请输入周工作小时数 (0-60): ");
        if (safeGetLine(buffer, sizeof(buffer)) == NULL) {
            printf("输入错误！\n");
            continue;
        }

        if (sscanf(buffer, "%lf", &hours) != 1) {
            printf("输入必须是数字！\n");
            continue;
        }

        if (!validateHours(hours))
            printf("输入有误，工作小时数必须在0到60之间！\n");
        else
            valid_input = 1;
    } while (!valid_input);
    /* 重置标志，获取每小时工资率 */
    valid_input = 0;
    do {
        printf("请输入每小时工资率: ");
        if (safeGetLine(buffer, sizeof(buffer)) == NULL) {
            printf("输入错误！\n");
            continue;
        }

        if (sscanf(buffer, "%lf", &hourlyRate) != 1) {
            printf("输入必须是数字！\n");
            continue;
        }

        if (hourlyRate <= 0)
            printf("每小时工资率必须为正数！\n");
        else
            valid_input = 1;
    } while (!valid_input);
    /* 计算工资并显示结果 */
    salary = calculateSalary(hours, hourlyRate);
    printf("\n计算结果\n");
    printf("-------------------------\n");
    printf("周工作时间: %.2f 小时\n", hours);
    printf("每小时工资: %.2f 元\n", hourlyRate);
    printf("应付工资总额: %.2f 元\n\n", salary);

    printf("按回车键返回主菜单...");
    getchar(); /* 等待用户按回车 */
}

/**
 * @brief 显示关于系统的信息
 */
void showAbout() {
    printf("\n=== 关于系统 ===\n\n");
    printf("综合系统 v1.0\n");
    printf("包含登录系统和雇员工资计算功能\n\n");
    printf("登录系统：验证用户名和密码，根据不同情况给出相应提示\n");
    printf("雇员工资计算：根据工作时间和时薪计算工资\n\n");
    printf("作者：Bozhou Li\n");
    printf("日期：2025-02-28\n\n");

    printf("按回车键返回主菜单...");
    getchar(); /* 等待用户按回车 */
}

/**
 * @brief 显示并处理主菜单
 *
 * @return MenuOption 用户选择的菜单选项
 */
MenuOption showMainMenu() {
    char buffer[10];
    int choice = -1;

    printf("\n=== 主菜单 ===\n\n");
    printf("1. 工资计算\n");
    printf("2. 关于系统\n");
    printf("0. 退出系统\n\n");
    printf("请选择（0-2）: ");

    if (safeGetLine(buffer, sizeof(buffer)) == NULL)
        return MENU_EXIT;

    if (sscanf(buffer, "%d", &choice) != 1) {
        printf("输入无效，请重新选择\n");
        return MENU_INVALID; /* 返回无效选择状态 */
    }

    if (choice < 0 || choice > 2) {
        printf("选择超出范围，请重新选择\n");
        return MENU_INVALID; /* 返回无效选择状态 */
    }

    return (MenuOption)choice;
}

/**
 * @brief 处理登录
 *
 * @return int 登录是否成功，1表示成功，0表示失败
 */
int performLogin() {
    char username[USERNAME_MAX_LENGTH] = {0}; /* 用户名 */
    char password[PASSWORD_MAX_LENGTH] = {0}; /* 密码 */
    LoginStatus status;                       /* 登录状态 */
    int continue_login = 1;                   /* 是否继续登录 */
    int max_attempts = 3;                     /* 最大尝试次数 */
    int attempts = 0;                         /* 当前尝试次数 */

    printf("=== 系统登录界面 ===\n\n");
    printf("提示：管理员登录需要正确的用户名和密码\n");
    printf("用户名必须为字母，密码必须为数字\n\n");

    while (continue_login && attempts < max_attempts) {
        /* 获取用户名 */
        printf("请输入用户名: ");
        if (safeGetLine(username, sizeof(username)) == NULL) {
            printf("输入错误！\n");
            continue;
        }

        /* 获取密码 */
        printf("请输入密码: ");
        if (safeGetLine(password, sizeof(password)) == NULL) {
            printf("输入错误！\n");
            continue;
        }

        /* 验证登录并处理结果 */
        status = validateLogin(username, password);
        continue_login = handleLoginStatus(status, username, password);

        if (continue_login) {
            attempts++;
            if (attempts < max_attempts)
                printf("\n还有 %d 次尝试机会\n\n", max_attempts - attempts);
            else {
                printf("\n尝试次数过多，程序将退出\n");
                return 0; /* 登录失败 */
            }
        } else
            return 1; /* 登录成功 */
    }

    // return 0; /* 登录失败 */
}

/**
 * @brief 主函数
 *
 * @return int 程序执行状态
 */
int main(void) {
    MenuOption option;
    int login_success;

    /* 系统登录 */
    login_success = performLogin();

    if (!login_success) {
        printf("\n=== 程序结束 ===\n");
        return EXIT_FAILURE;
    }

    /* 主事件循环 */
    do {
        option = showMainMenu();

        switch (option) {
            case MENU_SALARY_CALCULATION:
                runSalaryCalculation();
                break;

            case MENU_ABOUT:
                showAbout();
                break;

            case MENU_EXIT:
                printf("\n感谢使用本系统！\n");
                break;

            case MENU_INVALID:
                /* 无效选择，继续循环 */
                break;

            default:
                printf("\n未知选项，请重新选择\n");
                break;
        }
    } while (option != MENU_EXIT);

    printf("\n=== 程序结束 ===\n");
    return EXIT_SUCCESS;
}