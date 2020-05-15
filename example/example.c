#include <stdio.h>

#include "../fsm.h"

typedef enum {
    testFsmStateInit,
    testFsmStateHello,
} testFsmState_t;

typedef enum {
    testFsmEventInit,
    testFsmEventHello,
} testFsmEvent_t;

fsmStateEvent_t buffer[2];
fsmStateEvent_t historyBuffer[10];

fsmHandle_t fsm;

fsmStatus_t fooStart(void* pParam) {
    printf("fooStart Callback\r\n");
    
    return fsmStatusOk;
}

fsmStatus_t fooDone(void* pParam) {
    printf("fooDone Callback\r\n");
    
    return fsmStatusOk;
}

int main() {

    fsmInit(&fsm, buffer, 2);
    
    fsmAddStateEvent(&fsm, (fsmStateEvent_t){testFsmStateInit, testFsmEventHello, (fsmEventCallback_t)fooStart, testFsmStateHello});
    
    fsmAddStateEvent(&fsm, (fsmStateEvent_t){testFsmStateHello, testFsmEventInit, (fsmEventCallback_t)fooDone, testFsmStateInit});
    
    fsmStart(&fsm, testFsmStateInit);
    
    fsmHistoryStart(&fsm, historyBuffer, 10);
    
    uint32_t state;

    fsmGetState(&fsm, &state);

    printf("Current State: %d\r\n", state);
    
    fsmProcessEvent(&fsm, testFsmEventHello, NULL);
    
    fsmGetState(&fsm, &state);

    printf("Current State: %d\r\n", state);

    fsmProcessEvent(&fsm, testFsmEventInit, NULL);

    fsmGetState(&fsm, &state);

    printf("Current State: %d\r\n", state);
    
    fsmHistoryPrint(&fsm, printf);

    return 0;
}