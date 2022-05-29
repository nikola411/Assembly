INC_DIR = ./inc

PARSER_SCANNER_DIR = ./misc

PARSER_INC = ${PARSER_SCANNER_DIR}/parser/
PARSER_SRC = ${PARSER_SCANNER_DIR}/parser/
SCANNER_INC = ${PARSER_SCANNER_DIR}/scanner/
SCANNER_SRC = ${PARSER_SCANNER_DIR}/
LOCATION_INC = ${PARSER_SCANNER_DIR}/parser/

# SOURCE = ./src/*.cpp ./src/scanner/scanner.cpp ./src/parser/parser.cpp
SOURCE = $(wildcard ./src/*.cpp) ${PARSER_SCANNER_DIR}/scanner/scanner.cpp ${PARSER_SCANNER_DIR}/parser/parser.cpp
OUTPUT = -o ./asembler

# INCLUDES = -I${INC_DIR} -iquote${PARSER_INC}
INCLUDES = -I${INC_DIR} -I${PARSER_INC} -I${SCANNER_INC}

FLEX_SOURCE = ${SCANNER_SRC}scanner.l 

BISON_SOURCE = ${PARSER_SCANNER_DIR}/parser.yy
BISON_OUTPUT = ${PARSER_SRC}parser.cpp

all: flex bison
	g++ ${INCLUDES} -g ${SOURCE} ${OUTPUT}

flex:
	flex ${FLEX_SOURCE}

bison:
	bison  -o ${BISON_OUTPUT} ${BISON_SOURCE} -v

clean:
	rm -rf misc/scanner/*.cpp misc/scanner/*.hpp
	rm -rf misc/parser/* misc/parser/*.hpp

run:
	./bin/asembler -o out/izlaz2.dat  ./tests/fajl2.txt