//******************************************************************************************
//* CompilerAnweisungen
//******************************************************************************************

//******************************************************************************************
//* Includes
//******************************************************************************************
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "ORCal.h"

#pragma package(smart_init)

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
void PrintError( AnsiString sText, EErrorType errType )
{
   char  *pszErrWarn;

   pszErrWarn = (errType == MSG_ERROR) ? "Error" : "Warning";

   fprintf( stderr, "%s : %s", pszErrWarn, sText.c_str( ));
}

//******************************************************************************************
//* Beschreibung  :  Speicherbewegende Symbole mit entsprechenden Werten besetzen
//* Übergabe      :  [1] po : Komplettes Feld
//* Rückgabe      :
//* Datum         :  17.10.09
//******************************************************************************************
void TreatMemMoveOrders( TOrLine *po )
{
   bool  bFounded;

   for (int y = 0; y < po->nYLen; y++)
   {
      for (int x = 0; x < po->nXLen ; x++)
      {
         // Speicher BewebungKlammer gefunden
         if( (po->ppCmds[y][x] == '(') && (po->ppCmds[y][x+2] == ')') )
         {
            // an die SpeicherStelle des Bewegungssysmboles
            x++;
            bFounded = FALSE;
            // Stimmt auch das Bewegungssymbol
            for (int i = 0; i < MOVE_DIRECTIONS_COUNT ; i++)
            {
               // Stimmt ein Symbol überein
               if( po->ppCmds[y][x] == MEM_MOVE_SYMBOLS[i].chSymbol )
               {
                  po->ppCmds[y][x] = MEM_MOVE_SYMBOLS[i].chValue;  // Wert setzen
                  x++;     // nächster Wert kann auch übergangen werden
                  bFounded =  TRUE;
                  break;   // Abbrechen
               }
            }
            // Symbol nicht gefunden
            if( bFounded == FALSE )
               x--;
         }
      }
   }
}

//******************************************************************************************
//* Beschreibung  :  Ließt das Or File Typisiert ein
//* Übergabe      :  [1] Pfad + Name des Files
//* Rückgabe      :  Typisiertes OR File
//* Datum         :  17.10.09
//******************************************************************************************
TOrLine *ReadOrFile( char *pszFilePath )
{
   TStringList    *psl;
   TOrLine        *po;
   bool           bTmp;
   int            nLineLen;
   int            nBiggestLine;

   po = NULL;

   psl = new TStringList( );

   // Datei einlesen
   try
   {
      bTmp = TRUE;
      psl->LoadFromFile( pszFilePath );
   }
   catch( ... )
   {
      PrintError( (AnsiString)pszFilePath + " kann nicht eingelesen werden.", MSG_ERROR);
      bTmp = FALSE;
   }

   // Konnte das Filet eingelesen werden
   if( bTmp )
   {
      // Speicher erzeugen
      po = new TOrLine;

      // MaxLänge bestimmen
      nBiggestLine = 1;
      for (int i = 0; i < psl->Count; i++)
      {
         if( psl->Strings[i].Length() > nBiggestLine )
            nBiggestLine = psl->Strings[i].Length();
      }
      // Längen setzten
      po->nXLen  = nBiggestLine;
      po->nYLen  = psl->Count;
      po->ppCmds = new char*[po->nYLen];

      // Alle Werte rüber kopieren
      for (int y = 0; y < po->nYLen; y++)
      {
         // Speicher Reservieren und Zeilen vorbesetzten
         po->ppCmds[y] = new char[po->nXLen];
         memset( po->ppCmds[y], ' ', po->nXLen);

         // Zeile rüber kopieren
         strncpy(po->ppCmds[y], psl->Strings[y].c_str( ), psl->Strings[y].Length());
      }
      // Kontrollieren ob es speicher bewegende Symbole gibt
      TreatMemMoveOrders( po );
   }

   return po;
}

//******************************************************************************************
//* Beschreibung  :  Gibt einen Po Struct frei
//* Übergabe      :  [1] po : Zeiger auf den Po Struct
//* Rückgabe      :  
//* Datum         :  17.10.09
//******************************************************************************************
void DeleteCmdTab( TOrLine *po )
{
   for (int y = 0; y < po->nYLen; y++)
      delete[] po->ppCmds[y];

   delete[] po->ppCmds;
   delete   po;
}

//******************************************************************************************
//* Ende
//******************************************************************************************

