

	/*********************************************
 * Qplot for Exclusive Diphoton Analysis
 * Hussein Al Ghoul
 * August 2016
 * The Uniuversity of Kansas


 **********************************************/

#include <iostream>
#include <fstream>
#include <complex>
#include <vector>

using namespace std;
#include "TApplication.h"

#include "Qplot_Gui.h"


/***********************
 * PROTOTYPE FUNCTIONS
 ***********************/

int printUsage(char *processName)
{
  
  fprintf(stderr,"%s usage: [switches]   \n",processName);
  cerr << "\t-N \t\t Fit results did not fix any fit parameters" << endl;
  fprintf(stderr,"\t-h \t\t Print this help message\n\n");
}


/*
 ***********************
 *                     *
 *  Main()             *
 *                     *
 ***********************
 */

int main(int argc,char **argv){
  int argcc = argc;
 char *argptr; 
 cerr << "argc is " << argc << endl;
  for (int i = 0; i < argc; i++ ){
    argptr = argv[i];
    cerr << "argv[" << i << "] is " << argptr << endl;
  }

 
  char fitFile[128];
  char fitlistfile[128];
  ofstream outputFp;
  ifstream fitFp;
  ifstream fitlistf;
  bool useDebug = false;  
  bool useFixedInputParms = true;
       // PwaFitResults *PwaResults = new PwaFitResults;
 	// PwaResults->numberBins = numberBins;

  cerr.precision(10);
  cerr << "argc is " << argc << endl;
  for (int i = 0; i < argcc; i++ ){
    argptr = argv[i];
    cerr << "argv[" << i << "] is " << argptr << endl;
  }

  if (argc == 1){
    printUsage(argv[0]);
    exit (0);
  }

  for (int i = 1; i < argc; i++ ){
    argptr = argv[i];
    if( useDebug ) cerr << "argv[" << i << "] is " << argptr << endl;
    if ( *argptr == '-' ) {
      argptr++;
      switch (*argptr) {
      case 'N':
	useFixedInputParms = false;
	break;
      case 'd':
	useDebug = true;
	cerr << "Printing Degub Info" << endl;;
	break;
      case 'h':
	printUsage(argv[0]);
	exit(0);
	break;
      case 'l':
	sprintf(fitlistfile,"%s",++argptr);
	cerr << "fits list file: " << fitlistfile << endl;
	fitlistf.open( fitlistfile, ios::out );
	break;

      default:
	fprintf(stderr,"Unrecognized argument -%s\n\n",argptr);
	printUsage(argv[0]);    
	exit(-1);
	break;
      }
   
    }
  
}    
      if( useDebug ) cerr << "opening input file: " << argptr << endl;


	//cout<<"maxbins "<<PwaResults->fitResult[1][21]->massBin();
  if( useDebug ) cerr << "Done reading fit file" << endl; 
  TApplication theApp("App",&argc,argv);
  MyMainFrame *PwaViewerMainFrame =  new MyMainFrame(gClient->GetRoot(), 1200, 1000, argptr);
  theApp.Run();

}// end of main()







