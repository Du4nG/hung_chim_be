#include "emitter.h"

// uint8_t init_emitter(FILE fp) {

// }

uint8_t emit(FILE *fp, char* code) {
    fprintf(fp, "%s", code);
}