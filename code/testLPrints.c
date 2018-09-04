#include <string.h>

#include "LPrints.h"
#include "bool.h"

int main(int argc, char **argv) {
    int foo = 30;
    char *compareString = NULL;

    char* formatString =
        "This should print if __DEBUG__ is defined, but not otherwise 10*3 = %i";
    asprintf(&compareString, formatString, foo);
    
    PRINT(formatString, foo);
    PRINT("\\n is no longer needed!!!!!11one");

    ErrorSDL("testing the error function");
    
    return 0;
}
