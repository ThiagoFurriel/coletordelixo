#include "coletadelixo.h"

int main(int argc, char const *argv[]) {
    int *v = malloc2(sizeof(int));
    *v = 10;
    int *w = malloc2(sizeof(int));
    dump();
    *w = 20;
    atrib2((void**)&v, w);
    dump();
    char *c = malloc2(sizeof(char));
    *c = 'Z';
    dump();
    atrib2((void**)&w, NULL);
    dump();
    return 0;
}
