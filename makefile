ASSEMBLY_INC_DIR = ./inc/assembly
LINKER_INC_DIR =  ./inc/linker
ERROR_HANDLER_IND_DIR = ./inc/error_handling
PARSER_SCANNER_DIR = ./misc
PARSER_INC = ${PARSER_SCANNER_DIR}/parser/
PARSER_SRC = ${PARSER_SCANNER_DIR}/parser/
SCANNER_INC = ${PARSER_SCANNER_DIR}/scanner/
SCANNER_SRC = ${PARSER_SCANNER_DIR}/
LOCATION_INC = ${PARSER_SCANNER_DIR}/parser/

FLEX_SOURCE = ${SCANNER_SRC}scanner.l 
BISON_SOURCE = ${PARSER_SCANNER_DIR}/parser.yy
BISON_OUTPUT = ${PARSER_SRC}parser.cpp

ASSEMBLY_INCLUDES = -I${ASSEMBLY_INC_DIR} -I${PARSER_INC} -I${SCANNER_INC} -I${ERROR_HANDLER_IND_DIR}
ASSEMBLY_SOURCE =
ASSEMBLY_SOURCE += $(wildcard ./src/assembly/*.cpp)
ASSEMBLY_SOURCE += ${PARSER_SCANNER_DIR}/scanner/scanner.cpp
ASSEMBLY_SOURCE += ${PARSER_SCANNER_DIR}/parser/parser.cpp
ASSEMBLY_SOURCE += ./src/error_handling/error_handler.cpp

ASSEMBLY_OUTPUT = -o ./asembler

# here we put assembly_inc_dir as include dir since we will be using section and relocation table and possibly even more things
LINKER_INCLUDES = -I${LINKER_INC_DIR} -I${ASSEMBLY_INC_DIR} -I${ERROR_HANDLER_IND_DIR}
LINKER_SOURCE =
LINKER_SOURCE += $(wildcard ./src/linker/*.cpp)
LINKER_SOURCE += ./src/assembly/symbol_table.cpp
LINKER_SOURCE += ./src/assembly/section.cpp
LINKER_SOURCE += ./src/assembly/relocation_table.cpp
LINKER_SOURCE += ./src/error_handling/error_handler.cpp
LINKER_OUTPUT = -o ./linker

DEBUG_ENABLED = 1

lnk:
	g++ ${LINKER_INCLUDES} -g  ${LINKER_SOURCE} ${LINKER_OUTPUT}

asm: flex bison
	g++ ${ASSEMBLY_INCLUDES} -g ${ASSEMBLY_SOURCE} ${ASSEMBLY_OUTPUT}

flex:
	flex ${FLEX_SOURCE}

bison:
	bison  -o ${BISON_OUTPUT} ${BISON_SOURCE} -v

clean:
	rm -rf misc/scanner/*.cpp misc/scanner/*.hpp
	rm -rf misc/parser/* misc/parser/*.hpp
	rm -rf ./asembler
	rm -rf ./linker