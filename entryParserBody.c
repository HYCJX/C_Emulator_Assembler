#include "entryParserBody.h"

uint32_t parseBranch(char *operand, char ** token, StrIntList *list, uint32_t memAdd) {
    uint32_t binary = 0;
    uint32_t cond = 0;
    uint32_t offset = 0;
    if (strcmp(operand, "beq") == 0){
        cond = 0;
    } else if (strcmp(operand, "bne") == 0){
        cond = 0b0001;
    } else if (strcmp(operand, "bge") == 0){
        cond = 0b1010;
    } else if (strcmp(operand, "blt") == 0){
        cond = 0b1011;
    } else if (strcmp(operand, "bgt") == 0){
        cond = 0b1100;
    } else if (strcmp(operand, "ble") == 0){
        cond = 0b1101;
    } else if (strcmp(operand, "bal") == 0 || strcmp(operand, "b") == 0){
        cond = COND_ALWAYS;
    } else {
        perror("Unknown branch instruction type!");
        return EXIT_FAILURE;
    }
    cond <<= 28;
    //Get target address:
    char *exp = token[1];
    uint32_t targetAdd = getAddress(list, exp);
    //Get actual offset, taking into account of the ARM machine pipeline's effect:
    uint32_t actualOffset = targetAdd - memAdd - 8;
    //Get actual offset and reduce it to 24-bit form:
    offset = (actualOffset >> 2) & (0x00ffffff);
    //Assemble binary and return:
    binary = cond | (0xa << 24) | offset;
    return binary;
}

uint32_t parseDP(char *opcode, char **tokens) {
    uint32_t opCodeBin = 0;
    uint32_t operand2 = 0;
    uint32_t result;
    uint32_t Rd = 0;
    uint32_t Rn = 0;
    bool I = 0;
    bool S = 0;
    int numOfArgs = 0;
    for (numOfArgs = 0; tokens[numOfArgs]; numOfArgs++);
    if (strcmp(opcode, "add") == 0 || strcmp(opcode, "eor") == 0 || strcmp(opcode, "sub") == 0 || strcmp(opcode, "and") == 0
        || strcmp(opcode, "rsb") == 0 || strcmp(opcode, "orr") == 0) {
        Rd = getRegId(tokens[1]);
        Rn = getRegId(tokens[2]);
        if (strcmp(opcode, "and") == 0) {
            opCodeBin = 0;
        } else if (strcmp(opcode, "eor") == 0) {
            opCodeBin = 0b0001;
        } else if (strcmp(opcode, "sub") == 0) {
            opCodeBin = 0b0010;
        } else if (strcmp(opcode, "rsb") == 0) {
            opCodeBin = 0b0011;
        } else if (strcmp(opcode, "add") == 0) {
            opCodeBin = 0b0100;
        } else {
            opCodeBin = 0b1100;
        }
        operand2 = parseOperand2(&tokens[3], &I);
        result = COND_ALWAYS << 28 | I << 25 | opCodeBin << 21 | Rn << 16 | Rd << 12 | operand2;
    } else if (strcmp(opcode, "mov") == 0) {
        Rd = getRegId(tokens[1]);
        //mov Rd <operand2>:
        operand2 = parseOperand2(&tokens[2], &I);
        result = COND_ALWAYS << 28 | I << 25 | 0b1101 << 21 | Rd << 12 | operand2;
    } else {
        //tst teq cmp:
        S = 1;
        Rn = getRegId(tokens[1]);
        operand2 = parseOperand2(&tokens[2], &I);
        if (strcmp(opcode, "tst") == 0) {
            opCodeBin = 0b1000;
        } else if (strcmp(opcode, "teq") == 0) {
            opCodeBin = 0b1001;
        } else {
            opCodeBin = 0b1010;
        }
        result = COND_ALWAYS << 28 | I << 25 | opCodeBin << 21 | S << 20 | Rn << 16 | operand2;
    }
    return result;
}

uint32_t parseMul(char* operand, char** token) {
    bool ismla = (bool) strcmp(operand, "mul");
    bool A = ismla;
    uint32_t binary = 0;
    uint32_t Cond = (uint32_t) 0xe << 28;
    uint32_t Rd = getRegId(token[1]) << 16;
    uint32_t Rm = getRegId(token[2]);
    uint32_t Rn = ismla ? (getRegId(token[4]) << 12) : 0;
    uint32_t Rs = getRegId(token[3]) << 8;
    binary = Cond | A << 21 | Rd | Rn | Rs | (0b1001 << 4) | Rm;
    return binary;
}

uint32_t parseSDT(char *opcode, char **tokens, uint32_t memAdd, uint32_t *target, uint32_t output[]) {
    //Setup bit seq:
    uint32_t header = 0b111001;
    uint32_t immediate = 0;
    uint32_t instruction = 0;
    uint32_t offset = 0;
    uint32_t Rd = getRegId(tokens[1]);
    uint32_t Rn = 0;
    bool I = 0;
    bool L = (bool) (strcmp(opcode, "ldr") == 0 ? 1 : 0);
    bool P = 0;
    bool U = 1; //U = 1 by default.
    int numOfArgs = 0;
    for (int i = 0; tokens[i] != NULL; i++, numOfArgs++);
    if (tokens[2][0] == '=') {
        //Immediate:
        P = 1;
        char *str;
        immediate = (uint32_t) strtol(&tokens[2][3], &str, 16);
        if (str == NULL || strlen(str) > 0) {
            exit(EXIT_FAILURE);
        }
        if (immediate <= 0xff) {
            tokens[2][0] = '#';
            return parseDP("mov", tokens);
        }
        output[*target / 4] = immediate;
        //PC:
        offset = *target - memAdd - 8;
        *target += 4;
        Rn = PC_VALUE;
    } else if (strEndChar(tokens[numOfArgs - 1]) == ']') {
        //Pre:
        P = 1;
        if (numOfArgs == 3) {
            Rn = getRegId(removeFrontRear(tokens[2]));
        } else if (tokens[3][0] == '#') {
            //[Rn <#expression>]
            Rn = getRegId(&tokens[2][1]);
            offset = evalExpression(removeFrontRear(tokens[3]), &U);
        } else {
            I = 1;
            parseShift(&tokens[2], &Rn, &offset, &U);
        }
    } else {
        //Post:
        P = 0;
        if (tokens[3][0] == '#') {
            //[Rn] <#expression>:
            Rn = getRegId(&tokens[2][1]);
            offset = evalExpression(&tokens[3][1], &U);
        } else {
            I = 1;
            parseShift(&tokens[2], &Rn, &offset, &U);
        }
    }
    instruction = header << 26 | I << 25 | P << 24 | U << 23 | L << 20 | Rn << 16 | Rd << 12 | offset;
    return instruction;
}

uint32_t parseSpecial(char *opcode, char** tokens){
    if (strcmp(opcode, "andeq") == 0) {
        //andeq:
        for (int i = 1; i < 4; i++) {
            if (strcmp(tokens[i], "r0") != 0) {
                perror("Invalid andeq format!");
                return 0;
            }
        }
        return 0;
    } else {
        //lsl:
        //Alias each token:
        char *Rn = tokens[1];
        char *exp = tokens[2];
        //Build new tokens:
        char **newTokens = (char **)malloc(sizeof(char *) * 6);
        for (int i = 0; i < 5; i++) {
            newTokens[i] = (char *)malloc(sizeof(char) * DEFAULT_STRLEN_SHORT);
        }
        strcpy(newTokens[0], "mov");
        strcpy(newTokens[1], Rn);
        strcpy(newTokens[2], Rn);
        strcpy(newTokens[3], "lsl");
        strcpy(newTokens[4], exp);
        newTokens[5] = NULL;
        //Parse it using parseDP:
        uint32_t binary = parseDP("mov", newTokens);
        //Free new tokens before return:
        for (int i = 0; newTokens[i]; i++) {
            free(newTokens[i]);
        }
        free(newTokens);
        //Return binary:
        return binary;
    }
}