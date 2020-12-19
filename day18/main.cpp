#include <cstdio>
#include <cinttypes>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>

const int PLUS = -1;
const int TIMES = -2;
const int LPAREN = -3;

size_t nextToken(const std::string& str, const std::string& delims, size_t pos, std::string& token)
{
    size_t newpos = str.find_first_of(delims, pos);
    if (newpos == std::string::npos) {
        token = str.substr(pos);
    } else {
        token = str.substr(pos, newpos - pos);
        ++newpos;
    }

    return newpos;
}

void evaluate(std::stack<int64_t>& e, int64_t value)
{
    if (e.empty() || e.top() == LPAREN) {
        e.push(value);
    } else {
        int64_t opcode = e.top(); e.pop();
        int64_t operand = e.top(); e.pop();
        switch (opcode) {
        case PLUS: e.push(operand + value); break;
        case TIMES: e.push(operand * value); break;
        default: printf("uh oh!\n"); break;
        }
    }
}

void evaluate_rparen(std::stack<int64_t>& e, int rparen)
{
    while (rparen--) {
        int64_t value = e.top(); e.pop();
        int64_t lp = e.top(); e.pop();
        if (lp != LPAREN) {
            printf("mis-matched parens?\n");
        }
        evaluate(e, value);
    }
}

void evaluate2(std::stack<int64_t>& e, int64_t value)
{
    if (e.empty() || e.top() == LPAREN || e.top() == TIMES) {
        e.push(value);
    } else if (e.top() == PLUS) {
        int64_t opcode = e.top(); e.pop();
        int64_t operand = e.top(); e.pop();
        switch (opcode) {
        case PLUS: e.push(operand + value); break;
        default: printf("uh oh!\n"); break;
        }
    }
}

void evaluate_rparen2(std::stack<int64_t>& e, int rparen)
{
    while (rparen) {
        int64_t value = e.top(); e.pop();
        if (e.top() == LPAREN) {
            e.pop();
            evaluate2(e, value);
            rparen--;
        } else if (e.top() == TIMES) {
            evaluate(e, value);
        }
    }
}

int main(int argc, char** argv)
{
    std::ifstream is(argv[1]);
    std::string str;

    uint64_t sum = 0;
    while (std::getline(is, str)) {
        if (str.front() == ';') {
            continue;
        }

        std::stack<int64_t> e;
        e.push(LPAREN);

        std::string token;
        size_t pos = 0;
        do {
            int number = 0;
            pos = nextToken(str, " \n", pos, token);

            while (token.front() == '(') {
                printf(" LPAREN ");
                token = token.substr(1);
                e.push(LPAREN);
            }
            int rparen = 0;
            while (token.back() == ')') {
                token.pop_back();
                ++rparen;
            }

            if (token.front() == '+') {
                printf(" PLUS ");
                e.push(PLUS);
            } else if (token.front() == '*') {
                printf(" TIMES ");
                e.push(TIMES);
            } else {
                sscanf(token.c_str(), "%d", &number);
                printf(" NUM=%d ", number);
                evaluate2(e, number);
            }

            evaluate_rparen2(e, rparen);
        }
        while (pos != std::string::npos);

        evaluate_rparen2(e, 1);

        printf("\n\tevaluates to: %" PRIu64 "\n", e.top());
        sum += e.top();

        e.pop();
        if (!e.empty()) {
            printf("stack is not empty!!\n");
        }
    }

    printf("Sum of results: %" PRIu64 "\n", sum);

    return 0;
}
