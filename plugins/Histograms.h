#ifndef HISTOGRAMS_H
#define HISTOGRAMS_H

#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TLatex.h"
#include "TString.h"

const Int_t nMassBins = 60;
Double_t massBins[nMassBins+1] = {61.0, 62.0, 63.0, 64.0, 65.0, 66.0, 67.0, 68.0, 69.0, 70.0, 71.0, 72.0, 73.0, 74.0, 75.0, 76.0, 77.0, 78.0, 79.0, 80.0, 81.0, 82.0, 83.0, 84.0, 85.0, 86.0, 87.0, 88.0, 89.0, 90.0, 91.0, 92.0, 93.0, 94.0, 95.0, 96.0, 97.0, 98.0, 99.0, 100.0, 101.0, 102.0, 103.0, 104.0, 105.0, 106.0, 107.0, 108.0, 109.0, 110.0, 111.0, 112.0, 113.0, 114.0, 115.0, 116.0, 117.0, 118.0, 119.0, 120.0, 121.0};

const Int_t nPtBins = 9;
Double_t ptBins[nPtBins+1] = {0.0, 15.0, 25.0, 35.0, 50.0, 75.0, 100.0, 150.0, 250.0, 500.0};

const Int_t nEtaBins = 10;
Double_t etaBins[nEtaBins+1] = {-2.5, -2.0, -1.6, -1.4, -0.8, 0.0, 0.8, 1.4, 1.6, 2.0, 2.5};

const Int_t nPhiBins = 12;
Double_t phiBins[nPhiBins+1] = {0, 0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5, 5.5, 6};

#endif // HISTOGRAMS_H
