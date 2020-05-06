#ifndef _FSM_H_
#define _FSM_H_

#include <stdint.h>

// #define HISTORY

typedef enum {
    fsmStatusOk,
    fsmStatusErrorNull,
    fsmStatusErrorHandle,
    fsmStatusErrorEventCallback,
    fsmStatusErrorStateEvent,
    fsmStatusErrorState,
    fsmStatusErrorEvent,
    fsmStatusErrorMemory,
} fsmStatus_t;

typedef fsmStatus_t (*fsmEventCallback_t)(const void* pData);

typedef struct {
    uint32_t currentState;
    uint32_t event;
    fsmEventCallback_t callback;
    uint32_t nextState;
} fsmStateEvent_t;

typedef struct {
    uint32_t state;
    fsmStateEvent_t* buffer;
    uint32_t bufferSize;
    uint32_t bufferIndex;
#ifdef HISTORY
    fsmStateEvent_t* history;
    uint32_t historySize;
    uint32_t historyIndex;
    uint8_t flags;
#endif
} fsmHandle_t;

/**
 * @brief FSM Initialize
 * 
 * @param pHandle FSM Handle
 * @param buffer FSM StateEvent Buffer
 * @param bufferSize FSM StateEvent Buffer Size
 * @return fsmStatus_t Returns FSM Status
 */
fsmStatus_t fsmInit(fsmHandle_t* pHandle, fsmStateEvent_t* buffer, uint32_t bufferSize);

/**
 * @brief FSM DeInitialize
 * 
 * @param pHandle FSM Handle
 * @return fsmStatus_t Returns FSM Status
 */
fsmStatus_t fsmDeInit(fsmHandle_t* pHandle);

/**
 * @brief FSM Start at State
 * 
 * @param pHandle FSM Handle
 * @param state 
 * @return fsmStatus_t Returns FSM Status
 */
fsmStatus_t fsmStart(fsmHandle_t* pHandle, uint32_t state);

/**
 * @brief FSM Add State/Event
 * 
 * @param pHandle FSM Handle
 * @param stateEvent 
 * @return fsmStatus_t Returns FSM Status
 */
fsmStatus_t fsmAddStateEvent(fsmHandle_t* pHandle, fsmStateEvent_t stateEvent);

/**
 * @brief FSM Process Event
 * 
 * @param pHandle FSM Handle
 * @param event 
 * @param pParam 
 * @return fsmStatus_t Returns FSM Status
 */
fsmStatus_t fsmProcessEvent(fsmHandle_t* pHandle, uint32_t event, void* pParam);

/**
 * @brief FSM Gets Current State
 * 
 * @param pHandle FSM Handle
 * @param state 
 * @return fsmStatus_t Returns FSM Status
 */
fsmStatus_t fsmGetState(fsmHandle_t* pHandle, uint32_t* state);

#ifdef HISTORY
/**
 * @brief FSM Start History Tracking
 * 
 * @param pHandle FSM Handle
 * @param historyBuffer 
 * @param historyBufferSize 
 * @return fsmStatus_t Returns FSM Status
 */
fsmStatus_t fsmHistoryStart(fsmHandle_t* pHandle, fsmStateEvent_t* historyBuffer, uint32_t historyBufferSize);

/**
 * @brief FSM Stop History Tracking
 * 
 * @param pHandle FSM Handle
 * @return fsmStatus_t Returns FSM Status
 */
fsmStatus_t fsmHistoryStop(fsmHandle_t* pHandle);

/**
 * @brief FSM Print History
 * 
 * @param pHandle FSM Handle
 * @param printf 
 * @return fsmStatus_t Returns FSM Status
 */
fsmStatus_t fsmHistoryPrint(fsmHandle_t* pHandle, int32_t (*printf)(const char * format, ...));
#endif

#endif /*_FSM_H_*/
