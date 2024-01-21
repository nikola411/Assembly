ASSEMBLY_INC_DIR = ./inc/assembler

PARSER_INC = ./misc/parser/
SCANNER_INC = ./misc/scanner/
LOCATION_INC = ./misc/parser/

FLEX_SOURCE = ./misc/scanner.l 
BISON_SOURCE = ./misc/parser.yy
BISON_OUTPUT = ./misc/parser/parser.cpp

UTIL = ./inc/util
UTIL_SOURCE = $(wildcard ./src/util/*.cpp)

ASSEMBLY_INCLUDES = -I${ASSEMBLY_INC_DIR} -I${PARSER_INC} -I${SCANNER_INC} -I${UTIL}
ASSEMBLY_SOURCE =
ASSEMBLY_SOURCE += $(wildcard ./src/assembler/*.cpp)
ASSEMBLY_SOURCE += ./misc/parser/parser.cpp
ASSEMBLY_SOURCE += ./misc/scanner/scanner.cpp
ASSEMBLY_SOURCE += ${UTIL_SOURCE}
ASSEMBLY_OUTPUT = -o ./asembler

LINKER_INCLUDE_DIR = ./inc/linker
LINKER_INCLUDES = -I${LINKER_INCLUDE_DIR} -I${UTIL}
LINKER_SOURCE =
LINKER_SOURCE += $(wildcard ./src/linker/*.cpp)
LINKER_SOURCE += ${UTIL_SOURCE}
LINKER_OUTPUT = -o ./linker


EMULATOR_INC_DIR = ./inc/emulator
EMULATOR_INCLUDES = -I${EMULATOR_INC_DIR} -I${UTIL}
EMULATOR_SOURCE = 
EMULATOR_SOURCE += $(wildcard ./src/emulator/*.cpp)
EMULATOR_SOURCE += ${UTIL_SOURCE}
EMULATOR_OUTPUT = -o ./emulator

DEBUG_ENABLED = 0
GPP_FLAGS = 

all: asm lnk
	@echo "$(COLOR_GREEN)Build successful!$(COLOR_END)"

emu:
	@echo "Building emulator..."
	@g++ -g ${EMULATOR_INCLUDES} ${GPP_FLAGS} ${EMULATOR_SOURCE} ${EMULATOR_OUTPUT}
	@echo "$(COLOR_BLUE)Success!$(COLOR_END)"

lnk:
	@echo "Building linker..."
	@g++ -g ${LINKER_INCLUDES} ${GPP_FLAGS} ${LINKER_SOURCE} ${LINKER_OUTPUT}
	@echo "$(COLOR_BLUE)Success!$(COLOR_END)"

asm: flex bison
	@echo "Building assembly..."
	@g++ -g ${ASSEMBLY_INCLUDES} ${GPP_FLAGS} ${ASSEMBLY_SOURCE} ${ASSEMBLY_OUTPUT}
	@echo "$(COLOR_BLUE)Success!$(COLOR_END)"

flex:
	@echo "Building flex..."
	@flex ${FLEX_SOURCE}
	@echo "$(COLOR_BLUE)Success!$(COLOR_END)"

bison:
	@echo "Building bison..."
	@bison  -o ${BISON_OUTPUT} ${BISON_SOURCE} -v
	@echo "$(COLOR_BLUE)Success!$(COLOR_END)"

clean:
	@rm -rf misc/scanner/*.cpp misc/scanner/*.hpp
	@rm -rf misc/parser/* misc/parser/*.hpp
	@rm -rf ./asembler
	@rm -rf ./linker
	@rm -rf ./*.out
	@rm -rf ./*.o
	@rm -rf ./emulator
	@rm -rf ./tests/output*.o
	@echo "Clean finished!"

# coloring util
COLOR_GREEN=\033[0;32m
COLOR_RED=\033[0;31m
COLOR_BLUE=\033[0;34m
COLOR_END=\033[0m

