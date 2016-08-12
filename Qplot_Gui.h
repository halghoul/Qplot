#ifndef _PWA_FIT_VIEW_GUI
#define _PWA_FIT_VIEW_GUI

#include <TApplication.h>
#include <TGClient.h>
#include <TGMenu.h>
#include <TGFrame.h>
#include <TGDockableFrame.h>
#include <TGButton.h>
#include <TGDoubleSlider.h>
#include <TGListBox.h>
#include <TList.h>
#include "TRootEmbeddedCanvas.h"
#include "TCanvas.h"
#include <TGButtonGroup.h>
#include <TGFileDialog.h>
#include <TGMsgBox.h>
#include <TH1F.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TMultiGraph.h>
#include <TRandom.h>
#include <TMath.h>
#include <math.h>
#include <TGTextEntry.h>
#include <TVirtualPad.h>
#include "TFrame.h"
#include "TStyle.h"
#include "TPaveText.h"
#include "TPaveStats.h"
#include <TFile.h>
#include <TDirectoryFile.h>
#include <TTree.h>
#include <TKey.h>


enum MenuItems {
  M_FILE_EXIT,
  M_FILE_PRINT,
  M_FILE_PRINTSETUP,

  M_HELP_HELP,
  M_HELP_SEARCH,
  M_HELP_ABOUT,
  

  M_CANVAS_CLEAR,
  M_CANVAS_SAVEAS,
  M_CANVAS_1X1,
  M_CANVAS_1X2,
  M_CANVAS_1X3,
  M_CANVAS_2X2,
  M_CANVAS_2X3,
  M_CANVAS_3X3,
  M_CANVAS_3X2
};
/*
class tgrapherrors : public TGraph{

private:

public:
  tgrapherrors();
  tgrapherrors(int n, float const *x, float const *y, float const *ex=0, float const *ey=0);
  virtual ~tgrapherrors();
  void custom(const TCanvas *c1);
};*/

class MyMainFrame : public TGMainFrame {
	
private:
  TGMenuBar *fMenuBar;
  TGDockableFrame *fMenuDock;
  TRootEmbeddedCanvas *fECanvas;
  TGListBox           *fListVBox;
  TGListBox           *fListCBox;
  TList               *fVSelected; 
  TList               *fCSelected; 

    TDirectoryFile *dir ;
    TDirectoryFile *trees;
    TKey *key;
    TTree *ntp[20];
    TObjArray *myarray[20];
    int nkeys;
    std::string Categories[10];

  TGFileInfo SaveAsFile;
  static const char *fSaveAsFileTypes[14];
  int fCurrentCanvas;
  int fNumberCanvases ;
    
public:
  MyMainFrame(const TGWindow *p, UInt_t w, UInt_t h ,char *inputfile);
  virtual ~MyMainFrame();
  void DoExit();
  void DoSelect();
  void PrintSelected();
  void PlotSelected();
  void Plot();
  void AddVariables();
  void SaveSelected();
  void Save();
  void ClearSelectedVariables();
  void ClearSelectedCategories();
  void HandleMenu( int );
  void SetCanvasDivision( int aRows, int aCols);
  int CurrentCanvas();
  void SendMsg(char*Title, char *Msg);
  ClassDef(MyMainFrame, 0)
};


#endif


