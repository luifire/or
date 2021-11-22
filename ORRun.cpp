//******************************************************************************************
//* CompilerAnweisungen
//******************************************************************************************

//******************************************************************************************
//* Includes
//******************************************************************************************

// VCL, Win
#include <vcl.h>
#include <stdio.h>
#include <conio.h>
#pragma hdrstop

// Projekt
#include "ORRun.h"

#pragma package(smart_init)

//******************************************************************************************
//* Defines / Macros
//******************************************************************************************
#define  CURRENT_MEM    (*m_Memory.vecVecLong[m_Memory.aktPoint.y])[m_Memory.aktPoint.x]
#define  CURRENT_CMD    m_pCmdTab->ppCmds[m_aktPoint.y][m_aktPoint.x]

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
//* Beschreibung  :  Konstruktor
//* Übergabe      :  [1] pCmdTab : Kommando Tabelle
//* Datum         :  17.10.09
//******************************************************************************************
CORRun::CORRun( TOrLine *pCmdTab, bool bDebug )
   :  m_pCmdTab(     pCmdTab ),
      m_Direction(   DIR_RIGHT ),
      m_EAbort(      OR_NO_EXIT ),
      m_bDebug(      bDebug )
{
   m_aktPoint.x = 0;
   m_aktPoint.y = 0;

   // Speicher erstellen und initialisieren
   m_Memory.aktPoint = m_aktPoint;
   // 1. Speicherzelle setzten
   PushMainVectorBack( );

   Run( );
}

//******************************************************************************************
//* Beschreibung  :  Destruktor
//* Datum         :  17.10.09
//******************************************************************************************
CORRun::~CORRun( )
{
   // Alle x Vectoren freigeben
   for (UINT i = 0; i < m_Memory.vecVecLong.size() ; i++)
   {
      delete m_Memory.vecVecLong[i];
   }
}

//******************************************************************************************
//* Beschreibung  :  Gibt einen Fehler mit Koordinaten aus
//* Übergabe      :  [1] pszText : Text mit 2 %d drin, hier kommen dann die x,y Koordinaten rein
//*                  [2] errTyp: Error oder Warnung
//* Rückgabe      :
//* Datum         :  17.10.09
//******************************************************************************************
void CORRun::PrintErrorWithCoordinates( char *pszText, EErrorType errType )
{
   AnsiString  sError;

   sError.sprintf( pszText, m_aktPoint.x, m_aktPoint.y);
   sError.cat_printf( "\r\n" );

   PrintError( sError, errType);
}

//******************************************************************************************
//* Beschreibung  :  Eigentliche Interpretation
//* Übergabe      :  
//* Rückgabe      :
//* Datum         :  17.10.09
//******************************************************************************************
void CORRun::Run( void )
{
   AnsiString  sTmp;
   AnsiString  sError;
   bool        bUnknownCmd;
   int         nC;

   nC = 0;
   while( m_EAbort == OR_NO_EXIT )
   {
      bUnknownCmd = TRUE;
      // soll debuggt werdne?
      if( m_bDebug )
         Debug( );

      // Alle Fälle durchgehen
      switch( CURRENT_CMD )
      {
      // mach nix
      case ' ':
      break;

      // Speicher hochzählen
      case '+':
         CURRENT_MEM++;
      break;

      // Speicher runter zählen
      case '-':
         CURRENT_MEM--;
      break;

      // Beenden
      case '@':
         m_EAbort = OR_EXIT_SUCCESS;
      break;

      // Ausgabe
      case ';':   // als ASCII
      case '.':   // als Zahl

         if( CURRENT_CMD == ';' )
            sTmp.sprintf( "%c;",CURRENT_MEM);
         else
            sTmp.sprintf( "%d;",CURRENT_MEM);

         if( m_bDebug )
            m_sOutput += sTmp;
         printf( sTmp.c_str() );
      break;

      // Ausgabe als Hex
      case ':':
         printf( "%x", CURRENT_MEM);
         printf( "\r\n" );
      break;

      // if
      case '?':
         ConditionalMove( );
      break;
      }

      // es wurde beendet
      if( m_EAbort != OR_EXIT_SUCCESS )
      {
         bUnknownCmd &= !IsCmdANormalCmd( );
         // schaut ob ein Richtungsänderung vorliegt oder der Speicher verändert werden soll
         bUnknownCmd &= !CheckDirectionChange( );         
         bUnknownCmd &= !CheckMemoryChange( );

         // Kam ein Unbekannter Befehl
         if( bUnknownCmd )
         {
            AnsiString  sError;

            sError.sprintf( "Unbekannte Operation (%c) an Position (%%d,%%d)", CURRENT_CMD);
            PrintErrorWithCoordinates( sError.c_str( ),MSG_WARNING);
         }

         // In nächste Richtung, bei Fehler beenden
         GotoNextPosition( );
      }

      nC++;
   }
   // und den Run wieder freigeben
   delete this;
}

//******************************************************************************************
//* Beschreibung  :  Allg Debug infos
//* Übergabe      :
//* Rückgabe      :
//* Datum         :  17.10.09                                                      f
//******************************************************************************************
void CORRun::Debug( void )
{
   char  ch;

   // aktuelle CMDTabl ausgeben
   DisplayCmdTable( );

   // auf next warten
   ch = 0;
   while( (ch != 'N') && (ch != 'F') )
   {
      ch = toupper(getch( ));

      Sleep( 1 );
   }
   // nicht mehr debuggen
   if( ch == 'F' )
      this->m_bDebug = FALSE;

}

//******************************************************************************************
//* Beschreibung  :  Schreibt die Kommando Tabelle auf den Stdout
//* Übergabe      :
//* Rückgabe      :
//* Datum         :  17.10.09
//******************************************************************************************
void CORRun::DisplayCmdTable( void )
{
   bool  bSpezialSymbol;
   char  chAktSymbol;
   char  chDisplaySymbol;
   AnsiString  sRueckLauf;

   // An den Anfang des Debug zurück ( wenn Fehler ? ### )
/*   ###
   for (int y = 0; y <  m_pCmdTab->nYLen; y++)
      for (int x = 0; x <  m_pCmdTab->nXLen; x++)
         sRueckLauf.cat_printf( "\b" );
   printf( sRueckLauf.c_str( ) );
 */
   clrscr( );
   for (int y = 0; y <  m_pCmdTab->nYLen; y++)
   {
      for (int x = 0; x <  m_pCmdTab->nXLen; x++)
      {
         chAktSymbol = m_pCmdTab->ppCmds[y][x];
         // Spezial Symbol
         bSpezialSymbol = FALSE;
         for (int i = 0; i < MOVE_DIRECTIONS_COUNT; i++)
         {
            if( chAktSymbol == MEM_MOVE_SYMBOLS[i].chValue )
            {
               chDisplaySymbol = MEM_MOVE_SYMBOLS[i].chSymbol;
               bSpezialSymbol = TRUE;
               break;
            }
         }

         // Kein SpezialSymbol, normales ausgeben
         if( !bSpezialSymbol )
            chDisplaySymbol = chAktSymbol;
            
         // befinden wir uns am aktuellen Symbol?
         if( (x == m_aktPoint.x) && (y == m_aktPoint.y) )
         {
            // Grün färben
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_GREEN );
            printf( "%c", chDisplaySymbol);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
         }
         else
            printf( "%c", chDisplaySymbol);
      }
      // ZeilenUmbruch ausgeben
      printf( "\r\n" );
   }
   printf( m_sOutput.c_str() );
}

//******************************************************************************************
//* Beschreibung  :  Ist der Aktuelle Befehl ein normaler Befehl ( kein Mem oder Dir Move )
//* Übergabe      :
//* Rückgabe      :
//* Datum         :  17.10.09
//******************************************************************************************
bool CORRun::IsCmdANormalCmd( void )
{
   bool        bReturn;
   const       NORMAL_CMD_COUNT = 10;
   const char  NORMAL_COMMANDS[NORMAL_CMD_COUNT] = { ' ', '+', '-', '@', '.', ';', ':', '(', ')', '?'};

   for (int i = 0; i <  NORMAL_CMD_COUNT; i++)
   {
      if( CURRENT_CMD == NORMAL_COMMANDS[i] )
      {
         bReturn = TRUE;
         break;
      }
   }
   // ist es eine Zahl?
   if( !bReturn )
      bReturn = isdigit( CURRENT_CMD );

   return bReturn;
}

//******************************************************************************************
//* Beschreibung  :  Bedigneter Richtungswechsel
//* Übergabe      :
//* Rückgabe      :
//* Datum         :  18.10.09
//******************************************************************************************
void CORRun::ConditionalMove( void )
{
   /*
   int   nQueryValue;
   char  chCurrentValue;
   char  szValue[32];
   int   nValueIdx;


   // erstmal hinter das ?
   GotoNextPosition( );

   chCurrentValue = CURRENT_CMD;


   // steht kein ZahlenWert hinter dem ? wird 0 genommen
   if( !isdigit( chCurrentValue ) && chCurrentValue != '-' )
   {
      nQueryValue = 0;
      // richtiger Wert hüpft auch auf den 1. Sprung Punkt
      GotoNextPosition( );
   }
   else
   {
      nValueIdx = 0;

      // ist das 1. Zeichen ein Minus?
      if( chCurrentValue == '-' )
      {
         szValue[0] = '-';
         nValueIdx++;
         // hinter das -
         GotoNextPosition( );

         chCurrentValue = CURRENT_CMD;
      }

      // alle Werte rausholen
      while( isdigit( chCurrentValue ) )
      {
         szValue[nValueIdx] = chCurrentValue;
         nValueIdx++;
         // nächster Wert
         GotoNextPosition( );
         chCurrentValue = CURRENT_CMD;
      }

      szValue[nValueIdx] = '\0';

      nQueryValue = atoi( szValue );
   }
   */

   // steht der Mem Wert auf 0 wird gehüpft, danach hüft main nochmal
   if( CURRENT_MEM != 0 )
      GotoNextPosition( );
   GotoNextPosition( );
}

//******************************************************************************************
//* Beschreibung  :  Hüpft im HauptVector eins nach unten
//* Übergabe      :
//* Rückgabe      :
//* Datum         :  17.10.09
//******************************************************************************************
void CORRun::PushMainVectorBack( void )
{
   TVecLong    *pVecLong;
   
   pVecLong = new TVecLong( );
   pVecLong->push_back( 0 );
   
   m_Memory.vecVecLong.push_back( pVecLong );
}

//******************************************************************************************
//* Beschreibung  :  Zählt den Aktuellen Speicher hoch oder runter
//* Übergabe      :  [1] chOperation : Operation welche ausgeführt werden soll
//* Rückgabe      :
//* Datum         :  17.10.09
//******************************************************************************************
bool CORRun::CheckMemoryChange( void )
{
   TVecLong    *pVecLong;
   bool        bReturn;

   bReturn = TRUE;
   switch( CURRENT_CMD )
   {
   // Links
   case MEM_LEFT:
      m_Memory.aktPoint.x--;
   break;

   // Rechts
   case MEM_RIGHT:
      m_Memory.aktPoint.x++;
   break;

   // Hoch
   case MEM_UP:
      m_Memory.aktPoint.y++;
   break;

   // Runter
   case MEM_DOWN:
      m_Memory.aktPoint.y--;
   break;

   default:
      bReturn = FALSE;
   break;
   }

   // Fand eine Veränderung statt
   if( bReturn )
   {
      // Ins Negative?
      if( (m_Memory.aktPoint.y < 0) || (m_Memory.aktPoint.x < 0) )
      {
         PrintErrorWithCoordinates( "An Position (%d,%d) wird negativ in den Speicher gegangen!", MSG_ERROR);
         m_EAbort = OR_EXIT_ERROR;
      }
      else
      {
         if( m_Memory.aktPoint.y > ((int)m_Memory.vecVecLong.size( ) - 1) )
         {
            // Main Vector um 1. vergrößern
            PushMainVectorBack( );
         }
         else if( m_Memory.aktPoint.x > ((int)m_Memory.vecVecLong[m_Memory.aktPoint.y]->size() - 1) )
         {
            // x Vector um 1 vergrößern
            m_Memory.vecVecLong[m_Memory.aktPoint.y]->push_back( 0 );
         }
      }
   }

   return bReturn;
}

//******************************************************************************************
//* Beschreibung  :  Zählt den Aktuellen Speicher hoch oder runter
//* Übergabe      :  [1] chOperation : Operation welche ausgeführt werden soll
//* Rückgabe      :
//* Datum         :  17.10.09
//******************************************************************************************
bool CORRun::CheckDirectionChange( void )
{
   const char     chAktCmd = CURRENT_CMD;
   bool  bDirectionChanged;

   bDirectionChanged = FALSE;
   for (int i = 0; i < MOVE_DIRECTIONS_COUNT; i++)
   {
      // Richtungswechsel
      if( chAktCmd == MEM_MOVE_SYMBOLS[i].chSymbol )
      {
         bDirectionChanged = TRUE;
         if( m_Direction == MEM_MOVE_SYMBOLS[i].eDirection )
         {
            PrintErrorWithCoordinates( "An Position (%d,%d) findet ein redundanter Richtungswechsel statt", MSG_WARNING);
         }
         else
         {
            // Richtung ändern
            m_Direction = MEM_MOVE_SYMBOLS[i].eDirection;
         }
         break;
      }
   }

   return bDirectionChanged;
}

//******************************************************************************************
//* Beschreibung  :  In die nächste Richtung gehen
//* Übergabe      :  FALSE bei Fehler
//* Rückgabe      :
//* Datum         :  17.10.09
//******************************************************************************************
void CORRun::GotoNextPosition( void )
{
   bool  bFirst;

   bFirst = TRUE;
   // nicht abgeschalten und kein aktuelles Kmdn außer beim 1. mal 
   while( (m_EAbort == OR_NO_EXIT) && ((CURRENT_CMD == ' ') || bFirst) )
   {
      bFirst = FALSE;
      
      switch( m_Direction )
      {
      // Links
      case DIR_LEFT:
         m_aktPoint.x--;
      break;

      // Rechts
      case DIR_RIGHT:
         m_aktPoint.x++;
      break;

      // Hoch
      case DIR_UP:
         m_aktPoint.y--;
      break;

      // Runter
      case DIR_DOWN:
         m_aktPoint.y++;
      break;
      }

      // befinden wir uns noch in zulässigem Bereich
      if( (m_aktPoint.x < 0) || (m_aktPoint.x > m_pCmdTab->nXLen) ||
          (m_aktPoint.y < 0) || (m_aktPoint.y > m_pCmdTab->nYLen) )
      {
         PrintErrorWithCoordinates( "Zugriff in undefiniertem Bereich Position: (%d,%d)", MSG_ERROR);
         m_EAbort = OR_EXIT_ERROR;
      }
   }
}

//******************************************************************************************
//* Ende
//******************************************************************************************
