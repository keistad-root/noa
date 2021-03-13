#include "iostream"
#include "OpticSimulation.hpp"

void visualize()
{
    TCanvas *c1 = new TCanvas("c1", "c1", 800, 800);
    Int_t s = 100;
    Int_t screen = 200;
    Double_t width = 10;
    Double_t focus = 50;
    Double_t index[6] = {1.5142, 1.5163, 1.5168, 1.5183, 1.5250, 1.5296};
    environment p1;

    for (Int_t i = 0; i < 6; i++)
    {
        p1.setSource(0, 7 * sqrt(pow(width, 2) + 2 * width * focus) / 10, 0, index[i]); //광원(x위치, y위치, 각도, 광선의 굴절률);
        p1.setCanvas(screen);                                                           //스크린을 세울 x위치
        p1.setLensCenterAndFocus(s, width, focus, 1.51);                                //렌즈(중심, 두께/2, 초점, 렌즈의 굴절률);
        p1.drawLens();                                                                  //렌즈 그리기
        p1.findFinal(0.0001, i);                                                        //광선 그리기
        p1.setSource(0, -7 * sqrt(pow(width, 2) + 2 * width * focus) / 10, 0, index[i]);
        p1.findFinal(0.0001, i);
        p1.~environment();
    }
    c1->cd();
    p1.drawGraph("Color Aberration");
    c1->SaveAs("Color Aberration.png");
}
