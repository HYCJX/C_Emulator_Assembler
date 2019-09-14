#include "entryParserHelper.h"

/***** Functions *****/

uint32_t evalExpression(char *exp, bool *U)
{
    if (exp[0] == '-') {
        *U = 0;
        exp = &exp[1];
    } else {
        *U = 1;
    }
    char *tmp;
    uint32_t result = 0;
    if (exp[0] == '0' && exp[1] == 'x') {
        //hex
        result = (uint32_t) strtol(exp, &tmp, 16);
    } else {
        //dec
        result = (uint32_t) strtol(exp, &tmp, 10);
    }
    if (strlen(tmp) > 0) {
        exit(EXIT_FAILURE);
    }
    return result;
}

uint32_t getOp2Numeric(uint32_t operand2)
{
    uint32_t encode = operand2;
    int rotate;
    uint32_t result = 0xffffffff;
    bool canConvert = false;
    for (rotate = 0; rotate < 32; rotate += 2) {
        if (!(encode & ~0xffU)) {
            canConvert = true;
            uint32_t tmp = (rotate / 2) << 8 | encode;
            result = (tmp < result) ? tmp : result;

        }
        //Rotate:
        encode = (encode << 2) | (encode >> 30);
    }
    if (canConvert) {
        return result;
    }
    perror("Cannot convert!");
    return 0;
}

uint32_t getShiftCode(char *shiftType)
{
    uint32_t typeCode = 0;
    if (strcmp(shiftType, "lsl") == 0) {
        typeCode = 0;
    } else if (strcmp(shiftType, "lsr") == 0) {
        typeCode = 1;
    } else if (strcmp(shiftType, "asr") == 0) {
        typeCode = 2;
    } else if (strcmp(shiftType, "ror") == 0) {
        typeCode = 3;
    } else {
        exit(EXIT_FAILURE);
    }
    return typeCode;
}

uint32_t parseOperand2(char **tokensFrag, bool *I)
{
    uint32_t operand2 = 0;
    if (tokensFrag[0][0] == '#') {
        bool U;
        *I = 1;
        operand2 = evalExpression(&tokensFrag[0][1], &U);
        if (operand2 > 0xff) {
            //Determine rotate + imm:
            operand2 = getOp2Numeric(operand2);
        }
    } else {
        *I = 0;
        uint32_t Rm = getRegId(tokensFrag[0]);
        if (tokensFrag[1] != NULL) {
            //operand 2 = Rm, shift:
            char *shiftType = tokensFrag[1];
            uint32_t type = getShiftCode(shiftType);
            if (tokensFrag[2][0] == '#') {
                //Shift by a constant:
                bool U;
                uint32_t integer = evalExpression(&tokensFrag[2][1], &U);
                operand2 = integer << 7 | type << 5 | Rm;
            } else {
                //Shift by a reg specified value:
                uint32_t Rs = getRegId(tokensFrag[2]);
                operand2 = Rs << 8 | type << 5 | 1 << 4 | Rm;
            }
        } else {
            //operand 2 = rm, shift part = 0:
            operand2 = Rm;
        }
    }
    return operand2;
}

void parseShift(char **tokensFrag, uint32_t *Rn, uint32_t *offset, bool *U)
{
    //[Rn] {+/-}Rm {<shift>}:
    uint32_t Rm = 0;
    //Remove possible final right bracket:
    for (int i = 0; tokensFrag[i]; i++) {
        if (strEndChar(tokensFrag[i]) == ']') {
            removeLastChar(tokensFrag[i]);
        }
    }
    *Rn = getRegId(&tokensFrag[0][1]);
    *U = 1;
    //+/- Rm handling:
    if (tokensFrag[1][0] != 'r') {
        switch (tokensFrag[1][0]) {
            case '-':
                //When '-', set U = 0:
                *U = 0;
            case '+':
                //When '+', same as default, U = 1:
                break;
            default: exit(EXIT_FAILURE);
        }
        Rm = getRegId(&tokensFrag[1][1]);
    } else {
        Rm = getRegId(tokensFrag[1]);
    }
    if (tokensFrag[2] == NULL) {
        //[Rn] {+/-}Rm:
        *offset = Rm;
    } else {
        uint32_t shift = 0;
        uint32_t typeCode = 0;
        //[Rn] {+/-}Rm <shift>:
        char *shiftType = tokensFrag[2];
        char *tail = tokensFrag[3];
        //Determine shift code:
        typeCode = getShiftCode(shiftType);
        //Evaluate tail:
        if (tail[0] == '#') {
            //tail is an expression:
            uint32_t integer = evalExpression(&tail[1], U);
            *offset = integer << 7 | typeCode << 5 | Rm;
        } else {
            *U = 1;
            uint32_t reg = getRegId(tail);
            *offset = reg << 8 | typeCode << 5 | 1 << 4 | Rm;
        }
    }
}

/***** Helper Functions *****/

static char strEndChar(char *str) {
    return str[strlen(str) - 1];
}

static char *removeFrontRear(char *str) {
    return removeLastChar(&str[1]);
}

static char *removeLastChar(char *str) {
    str[strlen(str) - 1] = '\0';
    return str;
}

static uint32_t getRegId(char *str) {
    if (str[0] == 'r') {
        if (str[1] == '0' && !str[2]) {
            return 0;
        }
        char *tmp;
        uint32_t id = (uint32_t) strtol(&str[1], &tmp, 10);
        if (id > 0 && id < 17) {
            return id;
        }
    }
    perror("wrong register format!\n");
    return ERROR_CODE;
}