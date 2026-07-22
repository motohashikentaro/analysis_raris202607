#include "./../include/gain_calibration.h"

#include <TFile.h>
#include <TTree.h>

int main(int argc, char* argv[]){
    GainCalibration gain_cal(argv[1]);
    for(int layer = 0; layer < 6; layer++){
        for(int strip = 1; strip <= 32; strip++){
            gain_cal.AdcPlot(layer, strip);
        }
    }
    return 0;
}