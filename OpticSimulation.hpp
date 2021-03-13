#include "iostream"
#include "TMath.h"

class environment
{
private:
  TMultiGraph *mg = new TMultiGraph();
  TGraph *gu = new TGraph();
  TGraph *gd = new TGraph();
  Double_t initial[3];
  Double_t x_lens;
  Double_t n2;
  Double_t n_length;
  Double_t width;
  Double_t focus;
  Double_t radius;
  Double_t y_lens_max;
  Double_t y_lens_min;
  Double_t final[3];
  Double_t dx;
  Double_t Xs[100];
  Double_t setLensMax(Double_t x);
  Double_t setLensMin(Double_t x);
  void nextPosition(Double_t dx);

public:
  Double_t path[3];
  void setSource(Double_t x_ini, Double_t y_ini, Double_t theta_ini, Double_t n); //광원(x위치, y위치, 각도, 광선의 굴절률)
  void setCanvas(Double_t x_fi);                                                  //스크린을 세울 x위치
  void setLensCenterAndFocus(Double_t x, Double_t w, Double_t f, Double_t n);     //렌즈(중심, 두께/2, 초점, 렌즈의 굴절률);
  void drawLens();                                                                //렌즈 그리기
  void findFinal(Double_t dx, Int_t colorNumber);                                 //광선 그리기
  void drawGraph(TString str);                                                    //시각화
  ~environment();
};

void environment::setSource(Double_t x_ini, Double_t y_ini, Double_t theta_ini, Double_t n)
{
  initial[0] = x_ini;
  initial[1] = y_ini;
  initial[2] = theta_ini;
  n_length = n;
}

void environment::setCanvas(Double_t x_fi)
{
  final[0] = x_fi;
}

void environment::setLensCenterAndFocus(Double_t x, Double_t w, Double_t f, Double_t n)
{
  x_lens = x;
  width = w;
  focus = f;
  n2 = n;
  radius = 2 * f * (n - 1);
}

Double_t environment::setLensMax(Double_t x)
{
  if (x > x_lens - width && x < x_lens)
  {
    return sqrt(pow(radius, 2) - pow(x - x_lens - radius + width, 2)); //렌즈의 좌상 모양이다.
  }
  else if (x > x_lens && x < x_lens + width)
  {
    return sqrt(pow(radius, 2) - pow(x - x_lens + radius - width, 2)); //렌즈의 우상 모양이다.
  }
  else
  {
    return 0;
  }
}

Double_t environment::setLensMin(Double_t x)
{
  if (x > x_lens - width && x < x_lens)
  {
    return (-1) * sqrt(pow(radius, 2) - pow(x - x_lens - radius + width, 2)); //렌즈의 좌하 모양이다.
  }
  else if (x > x_lens && x < x_lens + width)
  {
    return (-1) * sqrt(pow(radius, 2) - pow(x - x_lens + radius - width, 2)); //렌즈의 우하 모양이다.
  }
  else
  {
    return 0;
  }
}

void environment::drawLens()
{
  for (Int_t i = 0; i < 1000; i++)
  {
    gu->SetPoint(i, x_lens - width + 2 * i * width / 999, setLensMax(x_lens - width + 2 * i * width / 999));
    gd->SetPoint(i, x_lens - width + 2 * i * width / 999, setLensMin(x_lens - width + 2 * i * width / 999));
  }
  gu->SetLineColor(1);
  gd->SetLineColor(1);
  mg->Add(gu);
  mg->Add(gd);
}

void environment::nextPosition(Double_t dx)
{
  if ((path[1] - setLensMax(path[0])) > 0 && (path[1] - setLensMax(path[0] + dx)) < 0)
  {
    path[0] = path[0] + 2 * dx;
    path[1] = path[1] + tan(path[2]) * 2 * dx;
    if (setLensMax(path[0]) != 0)
    {
      path[2] = (3.14159265358979 / 2) + atan((setLensMax(path[0] + dx) - setLensMax(path[0])) / dx) + asin(cos(path[2] - atan((setLensMax(path[0] + dx) - setLensMax(path[0])) / dx)) / n_length);
    }
    else
    {
      if (path[2] < 3.14159265358979 / 2 or (3.14159265358979 < path[2] && 3.14159265358979 * 3 / 2 < path[2]))
      {
        path[2] = asin(sin(path[2] / n_length));
      }
      else
      {
        path[2] = -asin(sin(path[2] / n_length));
      }
    }
  } //렌즈의 좌상부를 통과할 때 스넬의 법칙을 이용해 구한 각도의 변화
  else if ((path[1] - setLensMax(path[0])) < 0 && (path[1] - setLensMax(path[0] + dx)) > 0)
  {
    path[0] = path[0] + 2 * dx;
    path[1] = path[1] + tan(path[2]) * 2 * dx;
    if (setLensMax(path[0] + dx) != 0)
    {
      path[2] = (3.14159265358979 / 2) + atan((setLensMax(path[0] + dx) - setLensMax(path[0])) / dx) + asin(cos(path[2] - atan((setLensMax(path[0] + dx) - setLensMax(path[0])) / dx)) * n_length);
    }
    else
    {
      if (path[2] < 3.14159265358979 / 2 or (3.14159265358979 < path[2] && 3.14159265358979 * 3 / 2 < path[2]))
      {
        path[2] = asin(n_length * sin(path[2]));
      }
      else
      {
        path[2] = -asin(n_length * sin(path[2]));
      }
    }
  } //렌즈의 우상부를 통과할 때 스넬의 법칙을 이용해 구한 각도의 변화
  else if ((path[1] - setLensMin(path[0])) < 0 && (path[1] - setLensMin(path[0] + dx)) > 0)
  {
    path[0] = path[0] + 2 * dx;
    path[1] = path[1] + tan(path[2]) * 2 * dx;
    if (setLensMin(path[0]) != 0)
    {
      path[2] = (3.14159265358979 / 2) + atan((setLensMin(path[0] + dx) - setLensMin(path[0])) / dx) - asin(cos(path[2] - atan((setLensMin(path[0] + dx) - setLensMin(path[0])) / dx)) / n_length);
    }
    else
    {
      if (path[2] < 3.14159265358979 / 2 or (3.14159265358979 < path[2] && 3.14159265358979 * 3 / 2 < path[2]))
      {
        path[2] = asin(sin(path[2]) / n_length);
      }
      else
      {
        path[2] = -asin(sin(path[2]) / n_length);
      }
    }
  } //렌즈의 좌하부를 통과할 때 스넬의 법칙을 이용해 구한 각도의 변화
  else if ((path[1] - setLensMin(path[0])) > 0 && (path[1] - setLensMin(path[0] + dx)) < 0)
  {
    path[0] = path[0] + 2 * dx;
    path[1] = path[1] + tan(path[2]) * 2 * dx;
    if (setLensMax(path[0] + dx) != 0)
    {
      path[2] = (3.14159265358979 / 2) + atan((setLensMin(path[0] + dx) - setLensMin(path[0])) / dx) + asin(cos(path[2] - atan((setLensMin(path[0] + dx) - setLensMin(path[0])) / dx)) * n_length);
    }
    else
    {
      if (path[2] < 3.14159265358979 / 2 or (3.14159265358979 < path[2] && 3.14159265358979 * 3 / 2 < path[2]))
      {
        path[2] = asin(n_length * sin(path[2]));
      }
      else
      {
        path[2] = -asin(n_length * sin(path[2]));
      }
    }
  } //렌즈의 우하부를 통과할 때 스넬의 법칙을 이용해 구한 각도의 변화
  else
  {
    path[0] = path[0] + dx;
    path[1] = path[1] + tan(path[2]) * dx;
    path[2] = path[2]; //렌즈가 어디도 통과하지 않을 때 각도의 변화 = 0
  }
}

void environment::findFinal(Double_t dx, Int_t colorNumber)
{
  path[0] = initial[0];
  path[1] = initial[1];
  path[2] = initial[2];
  TGraph *g1 = new TGraph();
  Int_t i = 0;
  while (1)
  {
    nextPosition(dx);
    g1->SetPoint(i, path[0], path[1]);
    if (path[0] > final[0])
      break;
    i++;
  }
  Int_t color[6] = {632, 800, 400, 416, 600, 880};
  g1->SetLineColor(color[colorNumber]);
  mg->Add(g1);
} //광원의 최종 위치를 찾는다.

void environment::drawGraph(TString str)
{
  mg->SetTitle(str);
  mg->GetXaxis()->SetTitle("x[mm]");
  mg->GetYaxis()->SetTitle("y[mm]");
  mg->Draw("AL");
}

environment::~environment()
{
}
