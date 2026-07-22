#include "./../include/bm_correlation.h"

int main(int argc, char* argv[]) {

    const char* input_file = argv[1];
    BmCorrela correla(input_file);
    correla.BeamProfile();

    return 0;
}