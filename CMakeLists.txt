cmake_minimum_required(VERSION 3.15)
project(C_Emulator_Assembler C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(C_Emulator_Assembler
        32bitProcess.c
        32bitProcess.h
        armMachine.c
        armMachine.h
        armMachineExec.c
        armMachineExec.h
        armMachineHelper.c
        armMachineHelper.h
        armMachineStructure.h
        assemble.c
        assemblerIO.c
        assemblerIO.h
        emulate.c
        emulateIO.c
        emulateIO.h
        entryParser.c
        entryParser.h
        entryParserBody.c
        entryParserBody.h
        entryParserHelper.c
        entryParserHelper.h
        strIntList.c
        strIntList.h)
