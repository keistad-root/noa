#include "iostream"
#include "OpticSimulation.hpp"

Double_t stdev(Double_t data[], Int_t j)
{
  Double_t avg = 0;
  Double_t avg2 = 0;
  Double_t stdev;

  for (Int_t i = 0; i < j; i++)
  {
    avg += data[i] / j;
    avg2 += pow(data[i], 2) / j;
  }

  stdev = sqrt(avg2 - pow(avg, 2));

  return stdev;
}

void simulation()
{
  TCanvas *c1 = new TCanvas("c1", "c1", 800, 800);
  TCanvas *c2 = new TCanvas("c2", "c2", 1000, 800);
  Int_t s = 110;
  TString svalue = TString::Format("s=%d", s);
  TString svaluepng = TString::Format("s=%d.png", s);
  TString svaluepngsimulation = TString::Format("s=%d simulation.png", s);
  environment p1;
  Double_t Y_value[13];
  TGraph *gY = new TGraph();
  for (Int_t j = 0; j < 30; j++)
  {
    for (Int_t i = 0; i < 13; i++)
    {
      p1.setSource(0, 3, 0.05 - 0.01 * i, 1.51); //광원(x위치, y위치, 각도, 광선의 굴절률);
      p1.setCanvas(190 + j);                     //스크린을 세울 x위치
      p1.setLensCenterAndFocus(s, 2, 50, 1.51);  //렌즈(중심, 두께/2, 초점, 렌즈의 굴절률);
      p1.drawLens();                             //렌즈 그리기
      p1.findFinal();                            //광선 그리기
      p1.~environment();
      Y_value[i] = p1.path[1];
    }
    gY->SetPoint(j, 190 - s + j, stdev(Y_value, 13));
  }
  gY->SetTitle(svalue);
  gY->GetXaxis()->SetTitle("s*[mm]");
  gY->GetYaxis()->SetTitle("stdev of Y[mm]");
  c2->cd();
  gY->Draw();
  c2->SaveAs(svaluepng);
  c1->cd();
  p1.drawGraph();
  c1->SaveAs(svaluepngsimulation); //시각화
}
