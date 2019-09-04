#include "armMachineExec.h"

/***** Instruction Execution Functions *****/

bool execDP(uint32_t instruction, ARMmachine *machine)
{
    bool I = getBit(instruction, 25);
    uint32_t opCode = getBitSeq(instruction, 21, 24);
    bool S = getBit(instruction, 20);
    uint32_t *Rn = &machine -> reg[getBitSeq(instruction, 16, 19)];
    uint32_t *Rd = &machine -> reg[getBitSeq(instruction, 12, 15)];
    //Evaluate value for operand2:
    bool carry;
    uint32_t operand2 = evalOperand(getBitSeq(instruction, 0, 11), I, &carry, machine);
    //Compute result:
    uint32_t result;
    switch (opCode) {
        //0000 -- and:
        case 0:
            result = (*Rn) & operand2;
            *Rd = result;
            break;
        //0001 -- eor:
        case 1:
            result = (*Rn) ^ operand2;
            *Rd = result;
            break;
        //0010 -- sub:
        case 2:
            result = (*Rn) - operand2;
            *Rd = result;
            break;
        //0011 -- rsb:
        case 3:
            result = operand2 - *Rn;
            *Rd = result;
            break;
        //0100 -- add:
        case 4:
            result = *Rn + operand2;
            *Rd = result;
            break;
        //1000 -- tst:
        case 8:
            result = *Rn & operand2;
            break;
        //1001 -- teq:
        case 9:
            result = *Rn ^ operand2;
            break;
        //1010 -- cmp:
        case 10:
            result = *Rn - operand2;
            break;
        //1100 -- orr:
        case 12:
            result = *Rn | operand2;
            *Rd = result;
            break;
        //1101 -- mov:
        case 13:
            result = operand2;
            *Rd = result;
            break;
        default:
            perror("Unsupported opcode!");
            break;
    }
    //Update CPSR reg:
    if (S) {
        switch (opCode) {
            //Logical operations:
            case 0:
            case 1:
            case 8:
            case 9:
            case 12:
            case 13:
                setCpsrBit('C', carry, machine);
                break;
            //Arithmetic operations:
            //0010 -- sub:
            case 2:
            //1010 -- cmp:
            case 10:
                //ALU carry = NOT borrow:
                setCpsrBit('C', operand2 >= *Rn, machine);
            //0100 -- add:
            case 4:
                //ALU carry = overflow:
                setCpsrBit('C', (result < *Rn || result < operand2), machine);
            //0011 -- rsb:
            case 3:
                //ALU carry = borrow:
                setCpsrBit('C', *Rn >= operand2, machine);
            default:
                break;
        }
        setCpsrBit('Z', (result == 0), machine);
        setCpsrBit('N', getBit(result, 31), machine);
    }
    return true;
}

bool execMul(uint32_t instruction, ARMmachine *machine)
{
    bool A = getBit(instruction, 21);
    bool S = getBit(instruction, 20);
    uint32_t *Rd = &machine -> reg[getBitSeq(instruction, 16, 19)];
    uint32_t *Rn = &machine -> reg[getBitSeq(instruction, 12, 15)];
    uint32_t *Rs = &machine -> reg[getBitSeq(instruction, 8, 11)];
    uint32_t *Rm = &machine -> reg[getBitSeq(instruction, 0, 3)];
    uint32_t result;
    if (A) {
        result = (*Rm) * (*Rs) + *Rn;
    } else {
        result = (*Rm) * (*Rs);
    }
    *Rd = result;
    //Update CPSR reg:
    if (S) {
        setCpsrBit('Z', (result == 0), machine);
        setCpsrBit('N', getBit(result, 31), machine);
    }
    return true;
}

bool execSDT(uint32_t instruction, ARMmachine *machine)
{
    bool I = getBit(instruction, 25);
    bool P = getBit(instruction, 24);
    bool U = getBit(instruction, 23);
    bool L = getBit(instruction, 20);
    uint32_t *Rn = &machine -> reg[getBitSeq(instruction, 16, 19)];
    uint32_t *Rd = &machine -> reg[getBitSeq(instruction, 12, 15)];
    uint32_t offset = getBitSeq(instruction, 0, 11);
    //Compute operand2:
    bool carry;
    uint32_t operand2 = evalOperand(offset, !I, &carry, machine);
    operand2 = U ? operand2 : 0 - operand2;
    //Compute memAdd:
    uint32_t memAdd = P ? (*Rn + operand2) : *Rn;
    memAdd += 8 * (Rn == machine -> pc);
    *Rn = P ? *Rn : (*Rn + operand2);
    //Execution:
    if (L) {
        *Rd = get32bitIns(memAdd, machine);
    } else {
        if (!store32BitToMem(*Rd, memAdd, machine)) return false;
    }
    return true;
}

bool execBranch(uint32_t instruction, ARMmachine * machine)
{
    uint32_t offset = getBitSeq(instruction, 0, 23);
    offset = signExtend(offset << 2, 26);
    *(machine -> pc) += offset;
    *(machine -> pc) += 4;
    return true;
}

/***** Helper Evaluation Functions *****/

bool evalCond(uint32_t instruction, ARMmachine *machine)
{
    uint32_t condCode = getBitSeq(instruction, 28, 31);
    switch (condCode) {
        //0000 -- eq;
        case 0: return getCpsrBit('Z', machine);
        //0001 -- ne;
        case 1: return !getCpsrBit('Z', machine);
        //1010 -- ge;
        case 10: return getCpsrBit('N', machine) == getCpsrBit('V', machine);
        //1011 -- lt;
        case 11: return getCpsrBit('N', machine) != getCpsrBit('V', machine);
        //1100 -- gt;
        case 12: return !getCpsrBit('Z', machine) && (getCpsrBit('N', machine) == getCpsrBit('V', machine));
        //1101 -- le;
        case 13: return getCpsrBit('Z', machine) || (getCpsrBit('N', machine) != getCpsrBit('V', machine));
        //1110 -- al;
        case 14: return true;
        default: perror("Wrong condition code!"); return false;
    }
}

uint32_t evalOperand(uint32_t operand, bool I, bool *carry, ARMmachine *machine)
{
    uint32_t result;
    if (I) {
        //Operand2 is an immediate value:
        uint32_t rotate = getBitSeq(operand, 8, 11) * 2;
        uint32_t imm = getBitSeq(operand, 0, 7);
        result = rotate32Bit('R', imm, rotate, carry);
    } else {
        //Operand2 as register:
        uint32_t *Rm = &machine -> reg[getBitSeq(operand, 0, 3)];
        uint32_t shiftAmount;
        if (getBit(operand, 4) == 0) {
            shiftAmount = getBitSeq(operand, 7, 11);
        } else if (getBit(operand, 7) == 0) {
            uint32_t *Rs = &machine -> reg[getBitSeq(operand, 8, 11)];
            //Get the bottom byte from Rs register:
            shiftAmount = getBitSeq(*Rs, 0, 7);
        }
        uint32_t shiftType = getBitSeq(operand, 5, 6);
        switch(shiftType) {
            //00 -- logical left;
            case 0: result = logicalShift32Bit('L', *Rm, shiftAmount, carry); break;
            //01 -- logical right:
            case 1: result = logicalShift32Bit('R', *Rm, shiftAmount, carry); break;
            //10 -- arithmetic right;
            case 2: result = arithShift32Bit('R', *Rm, shiftAmount, carry); break;
            //11 -- rotate right;
            case 3: result = rotate32Bit('R', *Rm, shiftAmount, carry); break;
            default: break;
        }
    }
    return result;
}