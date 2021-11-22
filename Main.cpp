//******************************************************************************************
//* CompilerAnweisungen
//******************************************************************************************

//******************************************************************************************
//* Includes
//******************************************************************************************
#include <vcl.h>
#include <conio.h>
#pragma hdrstop

// Projekt
#include "ORRun.h"
#include "ORCal.h"
#include "lib.h"

//******************************************************************************************
//* Defines / Macros
//******************************************************************************************

//******************************************************************************************
//* Typen
//******************************************************************************************

//******************************************************************************************
//* Globale Varibalen
//******************************************************************************************

//******************************************************************************************
//* Ende
//******************************************************************************************

//******************************************************************************************
//* Funktionen
//******************************************************************************************

//******************************************************************************************
//* Beschreibung  :  Initialisiert
//* �bergabe      :
//* R�ckgabe      :
//* Datum         :  14.10.09
//******************************************************************************************
TOrLine *Init( int argc, char* argv[], bool &bDebug)
{
   bDebug = FALSE;
   
   // debug?
   if( argc > 2 )
   {
      if( (argv[2][1] == 'D') || (argv[2][1] == 'd') )
         bDebug = TRUE;
   }

   // KommandoTabelle einlesen und Typisieren
   return ReadOrFile( argv[1] );
}

//******************************************************************************************
//* Beschreibung  :  Exit
//* �bergabe      :
//* R�ckgabe      :
//* Datum         :  14.10.09
//******************************************************************************************
void Exit( TOrLine *po )
{
   // KommandoTabelle freigeben
   DeleteCmdTab( po );
}

//******************************************************************************************
//* Beschreibung  :  Main
//* �bergabe      :
//* R�ckgabe      :
//* Datum         :  14.10.09
//******************************************************************************************
#pragma argsused
int main(int argc, char* argv[])
{
   TOrLine  *po;
   bool     bDebug;

   po = Init( argc, argv, bDebug);

   // Init
   if( po )
   {
      // Interpreter
      new CORRun( po, bDebug);

      Exit( po );
   }
   getch( );
   return 0;

}

//******************************************************************************************
//* Ende
//******************************************************************************************


