//******************************************************************************************
//* Selfdefine
//******************************************************************************************
#ifndef ORRunH
#define ORRunH

//******************************************************************************************
//* Defines / Macros
//******************************************************************************************
//******************************************************************************************
//* Includes
//******************************************************************************************
// VCL, WIN
#include <Classes.hpp>
#include <vector.h>

// Projekt
#include "ORCal.h"

//******************************************************************************************
//* Typen
//******************************************************************************************
//******************************************************************************************
//* Klassen
//******************************************************************************************
class CORRun
{
   private:

      
      typedef vector<long>       TVecLong;

      struct
      {
         vector<TVecLong*>    vecVecLong;    // MemoryVector
         TPoint               aktPoint;      // aktuelle Position im Speicher
      } m_Memory;

      enum
      {
         OR_EXIT_SUCCESS,
         OR_EXIT_ERROR,
         OR_NO_EXIT
      } m_EAbort;   // Soll beendet werden

      TOrLine           *m_pCmdTab;       // KommandoTabelle
      EMoveDirection    m_Direction;      // Richtung in welche man sich gerade bewegt
      TPoint            m_aktPoint;       // aktuelle Punkt
      bool              m_bDebug;         // Soll debuggt werden
      AnsiString        m_sOutput;

      // HauptInterpretationsFunktion
      void  Run( void );
      // Richtungs Änderung
      bool  CheckDirectionChange( void );
      // Speicher Änderung
      bool  CheckMemoryChange( void );
      // Bedingter Richtungswechsel
      void  ConditionalMove( void );
      // Zur nächsten Position
      void  GotoNextPosition( void );
      // Haupt Vector um 1 vergrößern
      void  PushMainVectorBack( void );
      // Error mit Koordinaten
      void  PrintErrorWithCoordinates( char *pszText, EErrorType errType );
      // Ist das Aktuelles Cmd ein normales CMd
      bool  IsCmdANormalCmd( void );

      //**** Debug Funktionen
      // Debug HauptFunktion
      void  Debug( void );
      // Kmd Tab auf den Screen
      void  DisplayCmdTable( void );
      
   public:
      // Konstruktor
      CORRun( TOrLine *pCmdTab, bool bDebug);
      // Destruktor
      ~CORRun( );

};
//******************************************************************************************
//* FunktionsPrototypen
//******************************************************************************************
//******************************************************************************************
//* Externels
//******************************************************************************************


//******************************************************************************************
//* Ende
//******************************************************************************************
#endif // ORRunH
