/*
 * api.c
 *
 * Implementierung der Pseudo-Stammdatenschnittstelle.
 *
 *  Created on: 03.10.2024
 *      Author: rje
 */

#include "api.h"
#include <malloc.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>

void getApiVersion(int *major, int *minor, int *release) {
	*major = 2025;
	*minor = 4;
	*release = 1;
}

struct instance {
	InstrumentCb instrumentCb;
	ApiStatusCb apiStatusCb;
} instances[10];

struct data {
	char instId[12 + 1];
	char instName[50 + 1];
	int instType;
	double maxVolume;
};

#define MAX_INST 30

struct data instruments[MAX_INST];

struct status {
	int handle;
	int code;
	char *text;
};

static int lastErrorCode = 0;

static signed char* copyString(signed char *in) {
	int len = strlen((char*) in);
	signed char *out = malloc(len + 1);
	memcpy(in, out, len + 1);
	return out;
}

static int maxhandle = 0;

API_HANDLE createHandle(signed char *user, signed char *pw) {
	if (strcmp(user, "JavaLand") || strcmp(pw, "ZweiF\xFCnf")) {
		lastErrorCode = API_ERR_INVALID_USER_PW;
		return 0;
	}
	if (maxhandle == 10) {
		lastErrorCode = API_ERR_MAX_HANDLE;
		return 0;
	}
	lastErrorCode = API_OK;
	int i = maxhandle++;

	return i + 1;
}

int getLastErrorCode() {
	return lastErrorCode;
}

const char* getErrorMessage(int errorCode) {
	switch (errorCode) {
	case API_OK:
		return "OK";
	case API_ERR_INTERNAL:
		return "internal error";
	case API_ERR_INVALID_HANDLE:
		return "invalid handle";
	case API_ERR_INVALID_USER_PW:
		return "invalid user or password";
	case API_ERR_MAX_HANDLE:
		return "maximum number of handles reached";
	case API_ERR_NO_CB_REGISTERED:
		return "no callback registered";
	case API_ERR_PARAM:
		return "invalid param";
	default:
		return "\xBFunkown code?";
	}
}

static int checkHandle(API_HANDLE handle) {
	return (handle <= 0 || handle > maxhandle) ? 1 : 0;
}

API_RESULT registerStatusCallback(API_HANDLE handle, ApiStatusCb callback) {
	if (checkHandle(handle)) {
		return lastErrorCode = API_ERR_INVALID_HANDLE;
	}
	instances[handle - 1].apiStatusCb = callback;

	return lastErrorCode = API_OK;
}
API_RESULT registerInstrumentCallback(API_HANDLE handle, InstrumentCb callback) {
	if (checkHandle(handle)) {
		return lastErrorCode = API_ERR_INVALID_HANDLE;
	}
	instances[handle - 1].instrumentCb = callback;

	return lastErrorCode = API_OK;
}

static void* sendStatusT(void *args) {
	struct status *status = (struct status*) args;
	int handle = status->handle - 1;
	(instances[handle].apiStatusCb)(status->handle, status->code, status->text);
	free(status);

}
struct dataArg {
	int handle;
	struct data *data;
};

static void* sendDataT(void *args) {
	struct dataArg *dataArg = (struct dataArg*) args;
	int handle = dataArg->handle - 1;
	InstrumentData id;
	id.instId = dataArg->data->instId;
	id.instName = dataArg->data->instName;
	id.instType = dataArg->data->instType;
	id.maxVolume = dataArg->data->maxVolume;
	(instances[handle].instrumentCb)(dataArg->handle, &id);
	free(dataArg);
}
static pthread_t thread;
static int threadRunning = 0;

static void sendStatus(int handle, int code, char *text) {

	if ( threadRunning )
	{
		pthread_join(thread,NULL);
		threadRunning = 0;
	}
	struct status *status = malloc(sizeof(struct status));
	status->handle = handle;
	status->code = code;
	status->text = text;
	pthread_create(&thread, NULL, sendStatusT, status);
	threadRunning = 1;
}

static void sendData(int handle, struct data* data) {

	if ( threadRunning )
	{
		pthread_join(thread,NULL);
		threadRunning = 0;
	}
	struct dataArg *dataArg = malloc(sizeof(struct dataArg));
	dataArg->handle = handle;
	dataArg->data = data;
	pthread_create(&thread, NULL, sendDataT, dataArg);
	threadRunning = 1;
}

API_RESULT instrumentCreate(API_HANDLE h, char const *instId,
		char const *instName, unsigned int instType, double maxVolume) {
	int slot = -1;
	if (checkHandle(h)) {
		return lastErrorCode = API_ERR_INVALID_HANDLE;
	}
	if (instId == NULL || *instId == '\0')
		return lastErrorCode = API_ERR_PARAM;
	if (instName == NULL || *instName == '\0')
		return lastErrorCode = API_ERR_PARAM;
	if (instType < 1 || instType > 5)
		return lastErrorCode = API_ERR_PARAM;
	if (maxVolume <= 0.0)
		return lastErrorCode = API_ERR_PARAM;

	for (int i = 0; i < MAX_INST; i++) {
		if (strcmp(instId, instruments[i].instId) == 0) {
			sendStatus(h, CB_STATUS_INSTEXIST, "Instrument ID existiert");
			return API_OK;
		}
	}
	for (int i = 0; i < MAX_INST; i++) {
		if (instruments[i].instId[0] == '\0') {
			slot = i;
			break;
		}
	}
	if (slot < 0 || slot >= MAX_INST) {
		sendStatus(h, CB_STATUS_MAXDATA, "Speicher voll");
		return API_OK;
	}
	strncpy((char*) (instruments[slot].instId), instId, 12);
	instruments[slot].instId[12] = '\0';
	strncpy(instruments[slot].instName, instName, 50);
	instruments[slot].instName[50] = '\0';
	instruments[slot].instType = instType;
	instruments[slot].maxVolume = maxVolume;

	sendStatus(h, CB_STATUS_OK, "");
	return API_OK;
}

API_RESULT instrumentUpdate(
		API_HANDLE h,
		char const* instId,
		char const* instName,  int instNameFilled,
		unsigned int instType,  int instTypeFilled,
		double       maxVolume, int maxVolumeFilled
) {
	int slot = -1;
	if (checkHandle(h)) {
		return lastErrorCode = API_ERR_INVALID_HANDLE;
	}
	if (instId == NULL || *instId == '\0')
		return lastErrorCode = API_ERR_PARAM;
	if (instNameFilled && (instName == NULL || *instName == '\0'))
		return lastErrorCode = API_ERR_PARAM;
	if (instTypeFilled && (instType < 1 || instType > 5))
		return lastErrorCode = API_ERR_PARAM;
	if (maxVolumeFilled && maxVolume <= 0.0)
		return lastErrorCode = API_ERR_PARAM;

	for (int i = 0; i < MAX_INST; i++) {
		if (strcmp(instId, instruments[i].instId) == 0) {
			slot = i;
		}
	}
	if (slot < 0)
	{
		sendStatus(h, CB_STATUS_NODATA, "Nicht vorhanden");
		return API_OK;
	}
	if ( instNameFilled ) {
		strncpy(instruments[slot].instName, instName, 50);
		instruments[slot].instName[50] = '\0';
	}
	if (instTypeFilled) {
		instruments[slot].instType = instType;
	}
	if (maxVolumeFilled) {
		instruments[slot].maxVolume = maxVolume;
	}

	sendStatus(h, CB_STATUS_OK, "");
	return API_OK;
}

API_RESULT instrumentDelete(
		API_HANDLE h,
		char const* instId
) {
	int slot = -1;
	if (checkHandle(h)) {
		return lastErrorCode = API_ERR_INVALID_HANDLE;
	}
	if (instId == NULL || *instId == '\0')
		return lastErrorCode = API_ERR_PARAM;

	for (int i = 0; i < MAX_INST; i++) {
		if (strcmp(instId, instruments[i].instId) == 0) {
			slot = i;
		}
	}
	if (slot < 0)
	{
		sendStatus(h, CB_STATUS_NODATA, "Nicht vorhanden");
		return API_OK;
	}
	instruments[slot].instId[0] = '\0';

	sendStatus(h, CB_STATUS_OK, "");
	return API_OK;
}

API_RESULT instrumentRead(
		API_HANDLE h,
		char const* instId
) {
	int slot = -1;
	if (checkHandle(h)) {
		return lastErrorCode = API_ERR_INVALID_HANDLE;
	}
	if (instId == NULL || *instId == '\0')
		return lastErrorCode = API_ERR_PARAM;

	if (strcmp("CRASH!",instId) == 0)
	{
		return API_OK;
	}

	for (int i = 0; i < MAX_INST; i++) {
		if (strcmp(instId, instruments[i].instId) == 0) {
			slot = i;
		}
	}
	if (slot < 0)
	{
		sendStatus(h, CB_STATUS_NODATA, "Nicht vorhanden");
		return API_OK;
	}

	sendData(h,instruments+slot);
	return API_OK;
}

void reset()
{
	maxhandle = 0;
	memset(instruments,0,sizeof instruments);
}



