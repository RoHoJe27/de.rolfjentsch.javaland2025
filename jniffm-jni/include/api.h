/*
 * api.h
 *
 * Extern Deklarationen für die C-Schnittstelle.
 *
 *  Created on: 03.10.2024
 *      Author: rje
 */

#ifndef API_H_
#define API_H_

/* HANDLE for API */
typedef int API_HANDLE;

#define INVALID_API_HANDLE          0

/* Return Codes */
typedef int API_RESULT;
#define API_OK                      0 /* OK */
#define API_ERR_INVALID_HANDLE      1 /* Übergebenes Handle nicht gültig */
#define API_ERR_PARAM               2 /* Irgendein Parameter stimmt nicht */
#define API_ERR_MAX_HANDLE          3 /* Maximal Anzahl von Handles benutzt. Neustart erforderlich */
#define API_ERR_INTERNAL            4 /* Interner Fehler - was auch immer */
#define API_ERR_NO_CB_REGISTERED    5 /* Callbacks nicht registriert. */
#define API_ERR_INVALID_USER_PW     6 /* Ungültiger User oder Password */

/* Callback Status Codes */
#define CB_STATUS_OK                0 /* Operation erfolgreich */
#define CB_STATUS_MAXDATA         100 /* Speicher voll, keine weiteren Daten */
#define CB_STATUS_NODATA          101 /* instId nicht gefunden (read/update/delete) */
#define CB_STATUS_INSTEXIST       102 /* instId bereits vorhanden (insert) */

typedef struct
{
    char const * instId;      /* ID (WKN), muss eindeutig sein ISO-8859-1 */
    char const * instName;    /* Name des Finanzinstruments ISO-8859-1 */
    unsigned int instType;    /* Typ 1-5 */
    double       maxVolume;   /* Maximales Volumen (Stückpreis * Menge) pro Transaktion > 0 */
} InstrumentData;


/* Typedefs for Receive - Callback - functions */
typedef void (*InstrumentCb)( API_HANDLE handle, InstrumentData* msg);
typedef void (*ApiStatusCb)( API_HANDLE handle, int code, char const* msg);

/* Funktionen */
/* Liefert die Version der Schnittstelle */
void getApiVersion (int* major,int* minor,int* release);

/* Liefert den letzten ErrorCode. Relevant insbesondere für createHandle */
int getLastErrorCode(void);

/* Liefert zu einem Errorcode eine Message. Achtung: ISO-8859-1! */
const char* getErrorMessage(int errorCode);

/*
 * Erzeugt ein API-Handle.
 * Muss als erstes aufgerufen werden.
 * user muss "JavaLand" sein.
 * pw muss "ZweiFünf" sein.
 * user und pw sind ISO-8859-1 codiert.
 * Bei Erfolg wird ein Handle >= 1 zurückgeliefert,
 * bei Fehlern der Wert 0
 */
API_HANDLE createHandle(signed char* user,signed char *pw);

/*
 * Registriert die StatusCallback-Funktion.
 */
API_RESULT registerStatusCallback(API_HANDLE handle,ApiStatusCb callback);

/*
 * Registriert den Callback für die Instrumentendaten.
 */
API_RESULT registerInstrumentCallback(API_HANDLE handle,InstrumentCb callback);

/*
 * Legt ein neues Instrument an.
 * h ist das Handle aus createHandle
 * instId, instName, instType und maxVolume siehe InstrumentData
 */
API_RESULT instrumentCreate(
		API_HANDLE h,
		char const* instId,
		char const* instName,
		unsigned int instType,
		double       maxVolume
);

/*
 * Aktualisiert ein Instrument
 * h ist das Handle aus createHandle
 * instId, instName, instType und maxVolume siehe InstrumentData
 * *Filled gibt an, ob der Wert geändert werden soll (1) oder ignoriert wird (0).
 */
API_RESULT instrumentUpdate(
		API_HANDLE h,
		char const* instId,
		char const* instName,  int instNameFilled,
		unsigned int instType,  int instTypeFilled,
		double       maxVolume, int maxVolumeFilled
);


/*
 * Liest die Daten zu einem Instrument
 * h ist das Handle aus createHandle
 * instId ist die ID, für die Daten besorgt werden sollen.
 */
API_RESULT instrumentRead(
		API_HANDLE h,
		char const* instId
);

/*
 * Löscht die Daten zu einem Instrument
 * h ist das Handle aus createHandle
 * instId ist die ID, für die die Daten gelöscht werden sollen.
 */
API_RESULT instrumentDelete(
		API_HANDLE h,
		char const* instId
);

/*
 * Setzt die Bibliothek auf den initialen Status zurück.
 *
 * Wird für UnitTests benötigt, die hintereinander ablaufen.
 */
void reset(void);
#endif /* API_H_ */

