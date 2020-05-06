#include "fsm.h"

#include <stdio.h>

#ifdef HISTORY
#define flagHistoryShift 0
#define flagHistoryMask (1 << flagHistoryShift)
#endif

static fsmStatus_t fsmCheckHandle(fsmHandle_t* pHandle) {
    fsmStatus_t status = fsmStatusOk;
    
    if (pHandle == NULL) {
        status = fsmStatusErrorNull;
    } else if (pHandle->buffer == NULL || pHandle->bufferSize < 1) {
        status = fsmStatusErrorMemory;
    }
    
    return status;
}

static fsmStateEvent_t* fsmFindStateEvent(fsmHandle_t* pHandle, uint32_t event) {
    fsmStateEvent_t* pStateEvent = NULL;
    
    for (uint32_t i = 0; i < pHandle->bufferIndex; i++) {
        fsmStateEvent_t* pTemp = &pHandle->buffer[i];
        
        if (pTemp->currentState == pHandle->state) {
            if (pTemp->event == event) {
                pStateEvent = pTemp;
                // end early
                i = pHandle->bufferIndex;
            }
        }
    }
    
    return pStateEvent;
}

fsmStatus_t fsmInit(fsmHandle_t* pHandle, fsmStateEvent_t* buffer, uint32_t bufferSize) {
    fsmStatus_t status = fsmStatusOk;
    
    if (pHandle != NULL && buffer != NULL && bufferSize > 0) {
        pHandle->buffer = buffer;
        pHandle->bufferSize = bufferSize;
        pHandle->bufferIndex = 0;

#ifdef HISTORY
        pHandle->history = NULL;
        pHandle->historySize = 0;
#endif

    } else {
        status = fsmStatusErrorNull;
    }
    
    return status;
}

fsmStatus_t fsmDeInit(fsmHandle_t* pHandle) {
    return fsmStatusOk;
}

fsmStatus_t fsmStart(fsmHandle_t* pHandle, uint32_t state) {
    fsmStatus_t status = fsmStatusOk;
    
    if (fsmCheckHandle(pHandle) == fsmStatusOk) {
        pHandle->state = state;
    }
    
    return status;
}

fsmStatus_t fsmAddStateEvent(fsmHandle_t* pHandle, fsmStateEvent_t stateEvent) {
    fsmStatus_t status = fsmStatusOk;
    
    if (fsmCheckHandle(pHandle) == fsmStatusOk) {
        if (pHandle->bufferIndex < pHandle->bufferSize) {
            pHandle->buffer[pHandle->bufferIndex++] = stateEvent;
        } else {
            status = fsmStatusErrorMemory;
        }
    } else {
        status = fsmStatusErrorNull;
    }
    
    return status;
}

fsmStatus_t fsmProcessEvent(fsmHandle_t* pHandle, uint32_t event, void* pParam) {
    fsmStatus_t status = fsmStatusOk;
    
    if (fsmCheckHandle(pHandle) == fsmStatusOk) {
        fsmStateEvent_t* pStateEvent = fsmFindStateEvent(pHandle, event);
        
        if (pStateEvent != NULL) {
            if (pStateEvent->callback(pParam) == fsmStatusOk) {
                pHandle->state = pStateEvent->nextState;

#ifdef HISTORY
                if (pHandle->flags == flagHistoryMask) {
                    pHandle->history[pHandle->historyIndex++] = *pStateEvent;
                }
#endif
            } else {
                status = fsmStatusErrorEvent;
            }
        } else {
            status = fsmStatusErrorState;
        }
    } else {
        status = fsmStatusErrorNull;
    }
    
    return status;
}

fsmStatus_t fsmGetState(fsmHandle_t* pHandle, uint32_t* state) {
    fsmStatus_t status = fsmStatusOk;
    
    if (fsmCheckHandle(pHandle) == fsmStatusOk) {
        *state = pHandle->state;
    }
    
    return status;
}

#ifdef HISTORY
fsmStatus_t fsmHistoryStart(fsmHandle_t* pHandle, fsmStateEvent_t* historyBuffer, uint32_t historyBufferSize) {
    fsmStatus_t status = fsmStatusOk;
    
    if (fsmCheckHandle(pHandle) == fsmStatusOk) {
        pHandle->flags |= flagHistoryMask;
        pHandle->history = historyBuffer;
        pHandle->historySize = historyBufferSize;
    }
    
    return status;
}

fsmStatus_t fsmHistoryStop(fsmHandle_t* pHandle) {
    fsmStatus_t status = fsmStatusOk;
    
    if (fsmCheckHandle(pHandle) == fsmStatusOk) {
        pHandle->flags &= ~flagHistoryMask;
    }
    
    return status;
}

fsmStatus_t fsmHistoryPrint(fsmHandle_t* pHandle, int32_t (*printf)(const char * format, ...)) {
    fsmStatus_t status = fsmStatusOk;
    
    if (fsmCheckHandle(pHandle) == fsmStatusOk) {
        if (pHandle->flags == flagHistoryMask && printf != NULL) {
            printf("FSM History Count %d: State, Event, New State\r\n", pHandle->historyIndex);
            uint32_t i = pHandle->historyIndex;
            do {
                i--;
                printf("| old state %d >> event %d >> new state %d |\r\n", pHandle->history[i].currentState, pHandle->history[i].event, pHandle->history[i].nextState);
            } while (i != 0);
        }
    }
    
    return status;
}
#endif
