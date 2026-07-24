#include "./../include/dut_ana.h"

int main(int argc, char* argv[]) {

    DutAna dut_ana(argv[1]);
    dut_ana.ExtractFeatures();

    return 0;
}