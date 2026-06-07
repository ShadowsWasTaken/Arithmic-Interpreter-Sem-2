#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define INPUT_LENGTH 1000

typedef enum {
    TOKEN_INT,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_END
} TokenType;

typedef struct {
    TokenType type;
    int value;
} Token;

int pos = 0;
Token current_token;

void skip_whitespace(char *text) {
    while (text[pos] == ' ' || text[pos] == '\n')
        pos++;
}

int get_number(char *text) {
    int result = 0;
    while (isdigit(text[pos])) {
        result = result * 10 + (text[pos] - '0');
        pos++;
    }
    return result;
}

void get_next_token(char *text) {
    skip_whitespace(text);

    char c = text[pos];

    if (c == '\0') {
        current_token.type = TOKEN_END;
        return;
    }

    if (isdigit(c)) {
        current_token.type = TOKEN_INT;
        current_token.value = get_number(text);
        return;
    }

    if (c == '+')
        current_token.type = TOKEN_PLUS;
    else if (c == '-')
        current_token.type = TOKEN_MINUS;
    else if (c == '*')
        current_token.type = TOKEN_MUL;
    else if (c == '/')
        current_token.type = TOKEN_DIV;
    else if (c == '(')
        current_token.type = TOKEN_LPAREN;
    else if (c == ')')
        current_token.type = TOKEN_RPAREN;
    else {
        printf("Invalid character: %c\n", c);
        exit(1);
    }

    pos++;
}

void eat(TokenType type, char *text) {
    if (current_token.type == type) {
        get_next_token(text);
    } else {
        printf("Syntax error\n");
        exit(1);
    }
}

int expr(char *text);

int factor(char *text) {
    if (current_token.type == TOKEN_INT) {
        int value = current_token.value;
        eat(TOKEN_INT, text);
        return value;
    }

    if (current_token.type == TOKEN_LPAREN) {
        eat(TOKEN_LPAREN, text);
        int result = expr(text);
        eat(TOKEN_RPAREN, text);
        return result;
    }

    printf("Invalid syntax in factor\n");
    exit(1);
}

int term(char *text) {
    int result = factor(text);

    while (current_token.type == TOKEN_MUL ||
           current_token.type == TOKEN_DIV) {

        TokenType op = current_token.type;
        eat(op, text);
        int right = factor(text);

        if (op == TOKEN_MUL) {
            result *= right;
        } else {
            if (right == 0) {
                printf("Division by zero\n");
                exit(1);
            }
            result /= right;
        }
    }

    return result;
}

int expr(char *text) {
    int result = term(text);

    while (current_token.type == TOKEN_PLUS ||
           current_token.type == TOKEN_MINUS) {

        TokenType op = current_token.type;
        eat(op, text);
        int right = term(text);

        if (op == TOKEN_PLUS)
            result += right;
        else
            result -= right;
    }

    return result;
}

int interpret(char *text) {
    pos = 0;
    get_next_token(text);

    int result = expr(text);

    if (current_token.type != TOKEN_END) {
        printf("Unexpected input at end\n");
        exit(1);
    }

    return result;
}

int main() {
    printf("Interpreter running...\n");

    while (1) {
        char input[INPUT_LENGTH];

        printf(">>> ");
        fgets(input, INPUT_LENGTH, stdin);

        if (strncmp(input, "exit", 4) == 0)
            break;

        int result = interpret(input);
        printf("%d\n", result);
    }

    return 0;
}
