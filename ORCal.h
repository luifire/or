//******************************************************************************************
//* Selfdefine
//******************************************************************************************
#ifndef C1CalH
#define C1CalH

//******************************************************************************************
//* Defines / Macros
//******************************************************************************************
#define  MOVE_DIRECTIONS_COUNT   4     // Anzahl der Richtungen in welche man sich Bewegen kann

//******************************************************************************************
//* Typen
//******************************************************************************************
typedef enum
{
   MSG_ERROR,
   MSG_WARNING
}EErrorType;

typedef enum
{
   DIR_LEFT  = 0,
   DIR_RIGHT = 1,
   DIR_UP    = 2,
   DIR_DOWN  = 3,
} EMoveDirection; // Richtung in die man gehen kann, die Werte sind die Indizes für den MemMoveSymbol Struct

typedef enum
{
   MEM_LEFT  = 0x0001,
   MEM_RIGHT = 0x0002,
   MEM_UP    = 0x0003,
   MEM_DOWN  = 0x0004,
} EMoveMemory; // Richtung in die man gehen kann, die Werte sind die Indizes für den MemMoveSymbol Struct


typedef struct
{
   char     **ppCmds;       // KommandoTabelle
   int      nXLen;
   int      nYLen;
}TOrLine;

const struct
{
   char              chSymbol;
   EMoveDirection    eDirection;
   char              chValue;
} MEM_MOVE_SYMBOLS[MOVE_DIRECTIONS_COUNT] = {   {'<', DIR_LEFT, MEM_LEFT} ,{ '>', DIR_RIGHT, MEM_RIGHT},
                                                {'^', DIR_UP,  MEM_UP},    {'V',  DIR_DOWN,  MEM_DOWN}};

//******************************************************************************************
//* Klassen
//******************************************************************************************
//******************************************************************************************
//* FunktionsPrototypen
//******************************************************************************************
TOrLine *ReadOrFile( char *pszFilePath );
void     DeleteCmdTab( TOrLine *po );
void     PrintError( AnsiString sText, EErrorType errType );

//******************************************************************************************
//* Externels
//******************************************************************************************

#endif // C1CalH
//******************************************************************************************
//* Ende
//******************************************************************************************
