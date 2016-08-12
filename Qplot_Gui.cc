
#include "Qplot_Gui.h"
#include <fstream>
using namespace std;



const char *MyMainFrame::fSaveAsFileTypes[] = { 
  "gif"        , "*.gif" , 
  "ps"         , "*.ps"  ,
  "eps"        , "*.eps" ,
  "C source"   , "*.C"   , 
  "png"        , "*.png" ,
  "root file" , "*.root", 
  //  "ps"         , "*"     , 
  0       , 0};




void MyMainFrame::DoSelect()
{
	Printf("Slot DoSelect()");
}

//  int numberFitBins
	


MyMainFrame::MyMainFrame(const TGWindow *p, UInt_t w, UInt_t h , char *inputfile ) :
TGMainFrame(p, w, h)
{





  fCurrentCanvas =0;
  fNumberCanvases = 1;



  // MENU

  fMenuDock = new TGDockableFrame(this);
  AddFrame(fMenuDock, new TGLayoutHints(kLHintsExpandX, 0, 0, 1, 0));
  fMenuDock->SetWindowName("Menu Dock");

  TGLayoutHints *fMenuBarLayout = new TGLayoutHints(kLHintsTop | kLHintsExpandX);
  TGLayoutHints *fMenuBarItemLayout = new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 4, 0, 0);
  TGLayoutHints *fMenuBarHelpLayout = new TGLayoutHints(kLHintsTop | kLHintsRight);
  
  // File Menu
  TGPopupMenu *fMenuFile = new TGPopupMenu( gClient->GetRoot() );
  fMenuFile->AddEntry("E&xit", M_FILE_EXIT );
  fMenuFile->AddEntry("Close", M_FILE_EXIT );
  fMenuFile->AddSeparator();
  fMenuFile->AddEntry("&Print", M_FILE_PRINT);
  fMenuFile->AddEntry("P&rint setup...", M_FILE_PRINTSETUP);
  
  //Canvas Menu
  TGPopupMenu *fMenuCanvas = new TGPopupMenu( gClient->GetRoot() );
  fMenuCanvas->AddEntry("Save as...", M_CANVAS_SAVEAS);
  fMenuCanvas->AddEntry("&1x1", M_CANVAS_1X1);
  fMenuCanvas->AddEntry("&1x2", M_CANVAS_1X2);
  fMenuCanvas->AddEntry("&1x3", M_CANVAS_1X3);
  fMenuCanvas->AddEntry("&2x2", M_CANVAS_2X2);
  fMenuCanvas->AddEntry("&2x3", M_CANVAS_2X3);
  fMenuCanvas->AddEntry("&3x3", M_CANVAS_3X3);
  fMenuCanvas->AddEntry("&3x2", M_CANVAS_3X2);


  // Help Menu
  TGPopupMenu *fMenuHelp = new TGPopupMenu(gClient->GetRoot());
  fMenuHelp->AddEntry("&Help...", M_HELP_HELP);
  fMenuHelp->AddSeparator();
  fMenuHelp->AddEntry("&About", M_HELP_ABOUT);


  fMenuFile->Connect("Activated(Int_t)", "MyMainFrame", this,"HandleMenu(Int_t)");
  //fMenuFile->Connect("PoppedUp()", "MyMainFrame", this, "HandlePopup()");
  //fMenuFile->Connect("PoppedDown()", "MyMainFrame", this, "HandlePopdown()");
  fMenuCanvas->Connect("Activated(Int_t)", "MyMainFrame", this, "HandleMenu(Int_t)");
  fMenuHelp->Connect("Activated(Int_t)", "MyMainFrame", this, "HandleMenu(Int_t)");
  

  // MenuBar
  fMenuBar = new TGMenuBar(fMenuDock, 1, 1, kHorizontalFrame);
  fMenuBar->AddPopup("&File", fMenuFile, fMenuBarItemLayout);
  fMenuBar->AddPopup("&Canvas", fMenuCanvas, fMenuBarItemLayout);
  fMenuBar->AddPopup("&Help", fMenuHelp, fMenuBarHelpLayout);
  
  fMenuDock->AddFrame(fMenuBar, fMenuBarLayout);


 


  //TGHorizontalFrame *hTopFrame = new TGHorizontalFrame(this, 800, 800, kFixedWidth);
TGHorizontalFrame *hTopFrame = new TGHorizontalFrame(this, 1200, 1000);
	// Create top frame
	
  //
  // listBox
  //

      char IFN[100];
      std::string sinputfile(inputfile); 
      sprintf(IFN,inputfile);
      TFile *file0 = TFile::Open(IFN);


    dir = (TDirectoryFile*)file0->Get("diphotonDumper");
    file0->cd("diphotonDumper");

    trees = (TDirectoryFile*)dir->Get("trees");
//     trees.cd();
//     trees.ls();


	TIter next(trees->GetListOfKeys());
	nkeys = trees->GetNkeys();
	int counter=0;
	while ((key=(TKey*)next())) {
		printf("key: %s points to an object of class: %s at %dn",
		key->GetName(),
		key->GetClassName(),key->GetSeekKey());
		Categories[counter]=key->GetName();
		counter=counter+1;
					}

        char* c;
	for (int i=0 ; i< nkeys ; i++){
		cout<<"array try "<< i << " this is the name "<<Categories[i]<<endl;
					}

	for (int i=0 ; i< nkeys ; i++){
        c = new char[Categories[i].length() + 1];
        strcpy(c, Categories[i].c_str());
        ntp[i]= (TTree*)trees->Get(c);

	myarray[i] = (TObjArray*)ntp[i]->GetListOfBranches()->Clone();
	myarray[i]->SetOwner(kFALSE);
	myarray[i]->Sort();

}





  fListCBox = new TGListBox(hTopFrame, 100);
  fCSelected = new TList;
  fListCBox->Resize(150,100);
  for (int i = 0; i < nkeys ; ++i) {
  char* c = new char[Categories[i].length() + 1];
  strcpy(c, Categories[i].c_str());
    fListCBox->AddEntry(c,i+1);
  }

//  fListCBox->SetMultipleSelections();
  hTopFrame->AddFrame(fListCBox, new TGLayoutHints(kLHintsTop | kLHintsLeft |
						  kLHintsExpandY, 
						  5, 5, 5, 5));
	

  fListVBox = new TGListBox(hTopFrame, 100);
  fVSelected = new TList;
  char tmp[20];
    
  fListVBox->Resize(150,150);
  //fListVBox->SetMultipleSelections();
 hTopFrame->AddFrame(fListVBox, new TGLayoutHints(kLHintsTop | kLHintsLeft |
						  kLHintsExpandY, 
						  5, 5, 5, 5));


 
  // embedded canvas
  //  fECanvas = new TRootEmbeddedCanvas("Ecanvas",hTopFrame,416,368);
	if(nkeys<=3){
  fECanvas = new TRootEmbeddedCanvas("Ecanvas",hTopFrame,nkeys*400,350);
  fECanvas->GetCanvas()->SetFillColor(kWhite);  
  fECanvas->GetCanvas()->Divide(nkeys,1);
		}
	if(nkeys>3){
		if(nkeys%2 != 0){
  fECanvas = new TRootEmbeddedCanvas("Ecanvas",hTopFrame,((nkeys+1)/2)*400,700);
  fECanvas->GetCanvas()->SetFillColor(kWhite);  
  fECanvas->GetCanvas()->Divide((nkeys+1)/2,2); 
				}
		if(nkeys%2 ==0){
  fECanvas = new TRootEmbeddedCanvas("Ecanvas",hTopFrame,(nkeys/2)*400,700);
  fECanvas->GetCanvas()->SetFillColor(kWhite);  
  fECanvas->GetCanvas()->Divide(nkeys/2,2);
				}
			}

fECanvas->GetCanvas()->SetFillColor(kWhite);
  fECanvas->GetCanvas()->SetBorderSize(0);


  hTopFrame->AddFrame(fECanvas, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY,2,2,2,2));
  //fECanvas->MoveResize(240,16,416,368);
  
  AddFrame(hTopFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY|kLHintsRight, 2, 2, 5, 1));
  
  
	
  // Create a horizontal frame containing button(s)
  TGHorizontalFrame *hBottomFrame = new TGHorizontalFrame(this, 180, 70, kFixedWidth);
  // Reset Button
  

////////////////// *********************************************************////////////  
  //Reset Fit files button
  TGTextButton *fResetCButton = new TGTextButton(hBottomFrame, "&Reset Category Selections");
  fResetCButton->SetToolTipText("Click here to clear category selections");
  fResetCButton->Connect("Pressed()", "MyMainFrame", this, "ClearSelectedCategories()");
  hBottomFrame->AddFrame(fResetCButton, new TGLayoutHints(kLHintsExpandX, 5, 5, 3, 4));


  TGTextButton *fResetVButton = new TGTextButton(hBottomFrame, "&Reset Variable Selections");
  fResetVButton->SetToolTipText("Click here to clear Variable selections");
  fResetVButton->Connect("Pressed()", "MyMainFrame", this, "ClearSelectedVariables()");
  hBottomFrame->AddFrame(fResetVButton, new TGLayoutHints(kLHintsExpandX, 5, 5, 3, 4));


  // Add waves of corresponding file
  TGTextButton *fAddButton = new TGTextButton(hBottomFrame, "&Add Variableses");
  fAddButton->SetToolTipText("Click here to Add the Corresponding Waves");
  fAddButton->Connect("Pressed()", "MyMainFrame", this, "AddVariables()");
  hBottomFrame->AddFrame(fAddButton, new TGLayoutHints(kLHintsExpandX, 5, 5, 3, 4));
/////////////////////////////*************************************************///////////////


  // Plot Button
  TGTextButton *fPlotButton = new TGTextButton(hBottomFrame, "&Plot");
  fPlotButton->SetToolTipText("Click here to generate plot");
  fPlotButton->Connect("Pressed()", "MyMainFrame", this, "Plot()");
  hBottomFrame->AddFrame(fPlotButton, new TGLayoutHints(kLHintsExpandX, 5, 5, 3, 4));


  TGTextButton *fSaveButton = new TGTextButton(hBottomFrame, "&Save");
  fSaveButton->SetToolTipText("Click here to save plot");
  fSaveButton->Connect("Pressed()", "MyMainFrame", this, "SaveSelected()");
  hBottomFrame->AddFrame(fSaveButton, new TGLayoutHints(kLHintsExpandX, 5, 5, 3, 4));


  // Exit Button
  TGTextButton *exit = new TGTextButton(hBottomFrame, "&Exit ");
  exit->Connect("Pressed()", "MyMainFrame", this, "DoExit()");
  hBottomFrame->AddFrame(exit, new TGLayoutHints(kLHintsExpandX, 5, 5, 3, 4));
  AddFrame(hBottomFrame, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
	
  // Set a name to the main frame   
 //std::string fitin = std::string(fitfile);
  SetWindowName("Qplot");
  MapSubwindows();
  
  // Initialize the layout algorithm via Resize()
  Resize(GetDefaultSize());
  
  // Map main frame
  MapWindow();
  fListVBox->Select(1);
  fListCBox->Select(1);

}

MyMainFrame::~MyMainFrame()
{
	// Clean up main frame...
	Cleanup();
	if (fVSelected) {
		fVSelected->Delete();
		delete fVSelected;
	}
}



void MyMainFrame::AddVariables()
	{

  fListVBox->RemoveAll();
  fListVBox->Resize(100,350);
  int j=  fListCBox->GetSelected();
  for (int i = 0; i < ntp[j-1]->GetNbranches() ; ++i) {

      fListVBox->AddEntry(myarray[j-1]->At(i)->GetName(), i+1);
			  }
  fListVBox->Resize(150,400);
//  fListVBox->SetMultipleSelections();

	}


void MyMainFrame::Plot(){
  TCanvas *fcc = fECanvas->GetCanvas(); 
  int j= 0;
  j = fListCBox->GetSelected();
  int k= 0;
  k = fListVBox->GetSelected();
  fcc->cd( j );  
  char padname[30];
  sprintf(padname,"Ecanvas_%d",j);
  TPad *pad = (TPad*)fcc->FindObject(padname);
//  pad->SetFillColor(kWhite);
  TH1F *h[ntp[j-1]->GetNbranches()][nkeys];

  Double_t MinBin = 0.0;
  Double_t MaxBin = 0.0;
 
  Double_t binSize = 0;


  std::string st(myarray[j-1]->At(k-1)->GetTitle()); 
  const std::size_t sz = st.size() ;
  std::string st_new = st.substr(0,sz-2); //trim off the last two characters 
  char *variable = new char[st_new.length()+1];
  strcpy(variable, st_new.c_str());
 
  MinBin = ntp[j-1]->GetMinimum(variable);  
  MaxBin = ntp[j-1]->GetMaximum(variable);

  cout<<"variable plotting: "<< variable<<endl;
  cout<<"MaxBin "<< MinBin<<endl;
  cout<<"MaxBin "<< MaxBin<<endl;
  
  binSize = (MaxBin - MinBin) / 100;

// draw a frame to define the range
//    TH1F *hr = fcc->DrawFrame( fFitResults->minBin(0) - binSize, 0 , fFitResults->maxBin(1) + binSize, maxY);
    char name[10];
    sprintf(name,"h[%d][%d]",k,j-1); 
    char draw[1000]; 
    sprintf(draw,"%s>>h[%d][%d]",variable,k,j-1);
    h[k][j-1]= new TH1F(name,"",100, MinBin-10*binSize, MaxBin+binSize);
    h[k][j-1]->SetTitle(variable);  
  if(st_new == "diphoton_mass"){
      pad->SetLogy();
      ntp[j-1]->Draw(draw);
      h[k][j-1]->Draw("E1");
      pad->Update();}
    else{
      pad->SetLogy(0);
      ntp[j-1]->Draw(draw);
      h[k][j-1]->Draw("E1");
      pad->Update();}      

}










void MyMainFrame::PlotSelected(){
	// Writes selected entries in TList if multiselection.
  bool isPlotting = false;
  fVSelected->Clear();
  

}





/*void MyMainFrame::AddWaveFiles(){

  TCanvas *fcc = fECanvas->GetCanvas(); 
  int j=  fListCBox->GetSelected();
  fcc->cd( j );   
  fListCBox->SetMultipleSelections();
  fListCBox->GetSelectedEntries(fFSelected);
  int k=  fFSelected->GetEntries();
  fcc->Divide(k,k);
}
*/




void MyMainFrame::PrintSelected(){
	// Writes selected entries in TList if multiselection.
  bool isPlotting = false;
  fVSelected->Clear();
  
}





void MyMainFrame::ClearSelectedVariables(){
  fListVBox->SetMultipleSelections( kFALSE );
  //fListVBox->SetMultipleSelections( kTRUE );
}

void MyMainFrame::ClearSelectedCategories(){
  fListCBox->SetMultipleSelections( kFALSE );
 // fListCBox->SetMultipleSelections( kTRUE );
}




void MyMainFrame::SaveSelected(){


  TCanvas *fcc = fECanvas->GetCanvas();
  int j= 0;
  j = fListCBox->GetSelected();
  int k= 0;
  k = fListVBox->GetSelected();

  //get the recently plotted pad
  char padname[100];
  sprintf(padname,"Ecanvas_%d",j);
  TPad *pad = (TPad*)fcc->FindObject(padname);

  // append the title string
  std::string st(myarray[j-1]->At(k-1)->GetTitle());
  const std::size_t sz = st.size() ;
  std::string st_new = st.substr(0,sz-2); //trim off the last two characters
  char *variable = new char[st_new.length()+1];
  std::string cTitle;
  cTitle.append(Categories[j-1]);
  std::string separator("-");
  cTitle.append(separator);
  cTitle.append(st_new);
  std::string ext(".png");
  cTitle.append(ext);
  pad->SaveAs(cTitle.c_str());
//  fECanvas->GetCanvas()->SaveAs(cTitle.c_str());
}




void MyMainFrame::Save(){

  bool isPlotting = false;
  fVSelected->Clear();
//  fWSelected->ls();
//  SaveSelected( fWSelected );

  
}






void MyMainFrame::HandleMenu( int id ){
  
  switch( id ) {
  case  M_FILE_EXIT:
    Printf("M_FILE_EXIT\n");
    DoExit();
    break;


  case M_HELP_ABOUT:
    {
      char aTitle[] = "ABOUT";
      char aMsg[] = 
	"Qplot - View Diphoton analysis plots \n" 
	"authored by Hussein Al Ghoul \n"
	"Experimental Particle Physics\n"
	"University of Kansas\n"
	"Lawrence, KS, 66045, USA\n"
	"(C) Copyright 2016";
      int j;
            
      new TGMsgBox(gClient->GetRoot(), this,
		   aTitle, aMsg,
		   kMBIconAsterisk, kMBOk, &j);
    }
    break;
  default:
    Printf("Item not yet implimented\n");
    
  }
  
}

void MyMainFrame::SetCanvasDivision( int aRows, int aCols){
  fNumberCanvases = aRows * aCols;
  
  TCanvas * canvas = fECanvas->GetCanvas();
  canvas->cd();
  canvas->SetFillColor(0);
  canvas->SetBorderMode(kFALSE);
  canvas->Clear("");
  canvas->Divide( aCols, aRows );
  //ClearTGraphStack();
  for (int i = 1; i <= fNumberCanvases; i++){
    canvas->cd(i);
    gPad->SetFillColor(0);
    gPad->SetBorderMode(kFALSE);
    gPad->SetFrameBorderMode(kFALSE);
    gPad->Clear("");
  }
  
  if( fNumberCanvases > 1 )
    fCurrentCanvas = 3;
  else 
    fCurrentCanvas = 0;
  canvas->Update();
}


int MyMainFrame::CurrentCanvas(){
  
    int val  =   fCurrentCanvas;
    if( fNumberCanvases > 1){
      fCurrentCanvas++;
      if( fCurrentCanvas > fNumberCanvases )
	fCurrentCanvas = 1;
    }else fCurrentCanvas = 0;
    cout << "current canvas is " << fCurrentCanvas;
    return val;
}

void MyMainFrame::DoExit()
{
	Printf("Slot DoExit()");
	gApplication->Terminate(0);
}


void MyMainFrame::SendMsg(char *aTitle, char *aMsg){
  char tTitle[] = "test Title";
  char tMsg[] = "test Messsage";
  int retval;

 new TGMsgBox(gClient->GetRoot(), this,
                aTitle, aMsg,
                kMBIconStop, 0, &retval);
  cout << "Sending MsGBox" << endl;

}


