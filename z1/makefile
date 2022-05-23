INC_DIR = ./inc

PARSER_INC = ./src/parser/
PARSER_SRC = ./src/parser/
SCANNER_INC = ./src/scanner/
SCANNER_SRC = ./src/
LOCATION_INC = ./src/parser/

# SOURCE = ./src/*.cpp ./src/scanner/scanner.cpp ./src/parser/parser.cpp
SOURCE = $(wildcard ./src/*.cpp) ./src/scanner/scanner.cpp ./main.cpp ./src/parser/parser.cpp
OUTPUT = -o ./asembler

# INCLUDES = -I${INC_DIR} -iquote${PARSER_INC}
INCLUDES = -I${INC_DIR} -I${PARSER_INC} -I${SCANNER_INC}

FLEX_SOURCE = ${SCANNER_SRC}scanner.l 

BISON_SOURCE = ./src/parser.yy
BISON_OUTPUT = ${PARSER_SRC}parser.cpp


all: flex bison
	g++ ${INCLUDES} -g ${SOURCE} ${OUTPUT}

flex:
	flex  ${FLEX_SOURCE}

bison:
	bison -o ${BISON_OUTPUT} ${BISON_SOURCE} -v

clean:
	rm -rf src/scanner/*.cpp src/scanner/*.hpp
	rm -rf src/parser/* src/parser/*.hpp

run:
	./bin/asembler -o out/izlaz2.dat  ./tests/fajl2.txt