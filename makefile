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

DEBUG_ENABLED = 0

GPP_FLAGS = 

TEST_INC = ${ASSEMBLY_INCLUDES}
TEST_SRC = ./tests/tests.cpp
TEST_SRC += ${UTIL_SOURCE}
TEST_OUT = -o ./test

test:
	g++ -g ${TEST_INC} ${GPP_FLAGS} ${TEST_SRC} ${TEST_OUT}

all: asm

asm: flex bison
	g++ -g ${ASSEMBLY_INCLUDES} ${GPP_FLAGS} ${ASSEMBLY_SOURCE} ${ASSEMBLY_OUTPUT}

flex:
	flex ${FLEX_SOURCE}

bison:
	bison  -o ${BISON_OUTPUT} ${BISON_SOURCE} -v

clean:
	rm -rf misc/scanner/*.cpp misc/scanner/*.hpp
	rm -rf misc/parser/* misc/parser/*.hpp
	rm -rf ./asembler
