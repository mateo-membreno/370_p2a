/**
 * Project 2
 * Assembler code fragment for LC-2K
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//Every LC2K file will contain less than 1000 lines of assembly.
#define MAXLINELENGTH 1000

/**
 * Requires: readAndParse is non-static and unmodified from project 1a. 
 *   inFilePtr and outFilePtr must be opened. 
 *   inFilePtr must be rewound before calling this function.
 * Modifies: outFilePtr
 * Effects: Prints the correct machine code for the input file. After 
 *   reading and parsing through inFilePtr, the pointer is rewound.
 *   Most project 1a error checks are done. No undefined labels of any
 *   type are checked, and these are instead resolved to 0.
*/
/**
 * This function will be provided in an instructor object file once the
 * project 1a deadline + late days has passed.
*/
extern void print_inst_machine_code(FILE *inFilePtr, FILE *outFilePtr);

int readAndParse(FILE *, char *, char *, char *, char *, char *);
static void checkForBlankLinesInCode(FILE *inFilePtr);
static inline int isNumber(char *);
static inline void printHexToFile(FILE *, int);



// data structures
// header
typedef struct{
    int text_size;
    int data_size;
    int symbol_table_size;
    int relocation_tablesize;
} Header;

// text? - done by p1a

// data? - done by p1a

// symbol table -- need to pass over again to know what the type is
typedef struct { 
    char label[10]; // i think max is 6 but not sure
    char type;
    int address;
} Symbol;

// relocation table - can add this on first pass
typedef struct {
    int address;
    char opcode[10];
    char label[10];
} Relocation;

// helper functions
void addToSymbolTable(char *label, char type, int address);
void addToRelocationTable(int address, char *opcode, char *label);
void addLabelToAllLabels(char *label, char allLabels[][10], int label_count);
bool isDupeLabel(char *label, char allLabels[][10]);
bool isDupeSymbol(char *label, Symbol *symbolTable);
bool isDupeRelocation(char *label, Relocation *relocationTable);
void printHeader(Header header, FILE *outFilePtr);
void printSymbolTable(Symbol *symbolTable, Header header, FILE *outFilePtr);
void printRelocationTable(Relocation *relocationTable, Header header, FILE *outFilePtr);


int main(int argc, char **argv) {
    char *inFileStr, *outFileStr;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
            arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];
    // my stuff
    Symbol symbolTable[MAXLINELENGTH];
    Relocation relocationTable[MAXLINELENGTH];
    Header header = {0};
    char allLabels[MAXLINELENGTH][10];
    int label_count = 0;
    // my stuff end
    if (argc != 3) {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
            argv[0]);
        exit(1);
    }

    inFileStr = argv[1];
    outFileStr = argv[2];

    inFilePtr = fopen(inFileStr, "r");
    if (inFilePtr == NULL) {
        printf("error in opening %s\n", inFileStr);
        exit(1);
    }
    // Check for blank lines in the middle of the code.
    checkForBlankLinesInCode(inFilePtr);
    
    outFilePtr = fopen(outFileStr, "w");
    if (outFilePtr == NULL) {
        printf("error in opening %s\n", outFileStr);
        exit(1);
    }

    // add stuff to initial data structures (relocation table, and symbol table, update header accordingly)
    while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){
        // if label exists
            // add to label array
            // make sure not duplicate
            // add to symbol table if uppercase cause global
            // ignore relocation table becasue were just looking at the label in the 1st col not 5th
        if (strcmp(label, "")){
            if(!isDupeLabel(label, allLabels)){
                addLabelToAllLabels(label, allLabels, label_count);
                if(!isDupeSymbol(label, symbolTable) && label[0] >= 'A' && label[0] <= 'Z'){
                    addToSymbolTable(label, 'U', 0);
                }
            } else {
                exit(1);
            }
            label_count++;
        }
        // if lw or sw
            // add to relocation table if relative adress (is NOT a number)
            // add to symbol table if uppercase cause global
            //check duplicates right before adding
            // adress kept track of in header.text_size bec this is list of instructions
            // looking at 5th col
        if ((!strcmp(opcode, "lw") || !strcmp(opcode, "sw")) && !isNumber(arg2)) {
            if(!isDupeSymbol(arg2, symbolTable) && arg0[0] >= 'A' && arg0[0] <= 'Z'){
                addToSymbolTable(arg2, 'U', 0); // temporary U and 0
            }
            if(!isDupeRelocation(arg2, relocationTable)){
                addToRelocationTable(header.text_size, opcode, arg2);
            }
        }
        // if .fill
            // add to relocation table if relative adress 
            // add to symbol table if uppercase cause global
            // check duplicates right before adding
            // adress in header.data_size bec it keeps track of variables
            // looking at 3rd col and 1st
        if (!strcmp(opcode, ".fill") && !isNumber(arg0)) {
            if(!isDupeSymbol(arg0, symbolTable) && arg0[0] >= 'A' && arg0[0] <= 'Z'){
                addToSymbolTable(arg0, 'U', 0); // temporary U and 0
            }
            if(!isDupeRelocation(arg0, relocationTable)){
                addToRelocationTable(header.data_size, opcode, arg0);
            }

        }
        // if .fill increment data
        // else increment text bc instructions
        if(!strcmp(opcode, ".fill")){
            header.data_size++;
        } else {
            header.text_size++;
        }
    }

    // for symbol i in symbol table
        // for label j in allLabels
            // if match 
                // found match = true
                // if j >= text_size set sybol table i to curr, D, j - text_size
                // else set symbol table i to curr, T, j
 


    printHeader(header, outFilePtr);
    // generate machine code? from p1a not sure how to use obj file
    // print machine code
    rewind(inFilePtr);
    print_inst_machine_code(inFilePtr, outFilePtr);


    printSymbolTable(symbolTable, header, outFilePtr);
    printRelocationTable(relocationTable, header, outFilePtr);














    /* here is an example for how to use readAndParse to read a line from
        inFilePtr */
    if (! readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) ) {
        /* reached end of file */
    }

    /* after doing a readAndParse, you may want to do the following to test the
        opcode */
    if (!strcmp(opcode, "add")) {
        /* do whatever you need to do for opcode "add" */
    }

    /* this is how to rewind the file ptr so that you start reading from the
        beginning of the file */
    rewind(inFilePtr);

    /* this will print the correct machine code for the file */
    print_inst_machine_code(inFilePtr, outFilePtr);

    /* here is an example of using isNumber. "5" is a number, so this will
       return true */
    if(isNumber("5")) {
        printf("It's a number\n");
    }

    /* here is an example of using printHexToFile. This will print a
       machine code word / number in the proper hex format to the output file */
    printHexToFile(outFilePtr, 123);
    return(0);
}
// my funcs begin
void addToSymbolTable(char *label, char type, int address){
    //FILL

}
void addToRelocationTable(int address, char *opcode, char *label){
    //FILL

}
void addLabelToAllLabels(char *label, char allLabels[][10], int label_count){
    strcpy(allLabels[label_count], label);
}

bool isDuplicateLabel(char *label, char **allLabels){
    //FILL
    return false;
}

bool isDupeSymbol(char *label, Symbol *symbolTable){
    // FILL
    return false;
} 

bool isDupeRelocation(char *label, Relocation *relocationTable){
    // FILL
    return false;
} 
void printHeader(Header header, FILE *outFilePtr){
    fprintf(outFilePtr, "%d %d %d %d\n", header.text_size, header.text_size, header.symbol_table_size, header.relocation_tablesize);
}

void printSymbolTable(Symbol *symbolTable, Header header, FILE *outFilePtr){
    for(int i = 0; i < header.symbol_table_size; i++){
        fprintf(outFilePtr, "%s %c %d\n", symbolTable[i].label, symbolTable[i].type, symbolTable[i].address);
    }
}

void printRelocationTable(Relocation *relocationTable, Header header, FILE *outFilePtr){
    for(int i = 0; i < header.relocation_tablesize; i++){
        fprintf(outFilePtr, "%d %s %s\n", relocationTable[i].address, relocationTable[i].opcode, relocationTable[i].label);
    }
}



// my fucs end

/*
* NOTE: The code defined below is not to be modifed as it is implemented correctly.
*/

// Returns non-zero if the line contains only whitespace.
int lineIsBlank(char *line) {
    char whitespace[4] = {'\t', '\n', '\r', ' '};
    int nonempty_line = 0;
    for(int line_idx=0; line_idx < strlen(line); ++line_idx) {
        int line_char_is_whitespace = 0;
        for(int whitespace_idx = 0; whitespace_idx < 4; ++whitespace_idx) {
            if(line[line_idx] == whitespace[whitespace_idx]) {
                line_char_is_whitespace = 1;
                break;
            }
        }
        if(!line_char_is_whitespace) {
            nonempty_line = 1;
            break;
        }
    }
    return !nonempty_line;
}

// Exits 2 if file contains an empty line anywhere other than at the end of the file.
// Note calling this function rewinds inFilePtr.
static void checkForBlankLinesInCode(FILE *inFilePtr) {
    char line[MAXLINELENGTH];
    int blank_line_encountered = 0;
    int address_of_blank_line = 0;
    rewind(inFilePtr);

    for(int address = 0; fgets(line, MAXLINELENGTH, inFilePtr) != NULL; ++address) {
        // Check for line too long
        if (strlen(line) >= MAXLINELENGTH-1) {
            printf("error: line too long\n");
            exit(1);
        }

        // Check for blank line.
        if(lineIsBlank(line)) {
            if(!blank_line_encountered) {
                blank_line_encountered = 1;
                address_of_blank_line = address;
            }
        } else {
            if(blank_line_encountered) {
                printf("Invalid Assembly: Empty line at address %d\n", address_of_blank_line);
                exit(2);
            }
        }
    }
    rewind(inFilePtr);
}

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 */
int readAndParse(FILE *inFilePtr, char *label,
	char *opcode, char *arg0, char *arg1, char *arg2) {

    char line[MAXLINELENGTH];
    char *ptr = line;

    // delete prior values
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

    // read the line from the assembly-language file
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
		// reached end of file
        return(0);
    }

    // check for line too long
    if (strlen(line) >= MAXLINELENGTH-1) {
		printf("error: line too long\n");
		exit(1);
    }

    // Ignore blank lines at the end of the file.
    if(lineIsBlank(line)) {
        return 0;
    }

    // is there a label?
    ptr = line;
    if (sscanf(ptr, "%[^\t\n ]", label)) {
		// successfully read label; advance pointer over the label */
        ptr += strlen(label);
    }

    // Parse the rest of the line.  Would be nice to have real regular expressions, but scanf will suffice.
    sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
        opcode, arg0, arg1, arg2);
    return(1);
}

static inline int
isNumber(char *string)
{
    int num;
    char c;
    return((sscanf(string, "%d%c",&num, &c)) == 1);
}

// Prints a machine code word in the proper hex format to the file
static inline void 
printHexToFile(FILE *outFilePtr, int word) {
    fprintf(outFilePtr, "0x%08X\n", word);
}
