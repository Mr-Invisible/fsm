#include <stdio.h>

#include "../fsm.h"

typedef enum {
    exampleFsmStateInit,
    exampleFsmStateStarted,
    exampleFsmStateStopped,
} testFsmState_t;

typedef enum {
    exampleFsmEventStarting,
    exampleFsmEventStopping,
    exampleFsmEventRestarting,
    exampleFsmEventEnd,
} testFsmEvent_t;

fsmStateEvent_t buffer[10];
fsmStateEvent_t historyBuffer[10];

fsmHandle_t fsm;

fsmStatus_t fsmCallbackStarting(void* pParam) {
    printf("Starting Callback\r\n");

    return fsmStatusOk;
}

fsmStatus_t fsmCallbackRestarting(void* pParam) {
    printf("Restarting Callback\r\n");

    return fsmStatusOk;
}

fsmStatus_t fsmCallbackStopping(void* pParam) {
    printf("Stopping Callback\r\n");
    
    return fsmStatusOk;
}

fsmStatus_t fsmCallbackEnd(void* pParam) {
    printf("End Callback\r\n");

    return fsmStatusOk;
}

int main() {

    uint32_t state;
    fsmInit(&fsm, buffer, 10);

    fsmAddStateEvent(&fsm, (fsmStateEvent_t){exampleFsmStateInit, exampleFsmEventStarting, (fsmEventCallback_t)fsmCallbackStarting, exampleFsmStateStarted});
    fsmAddStateEvent(&fsm, (fsmStateEvent_t){exampleFsmStateStarted, exampleFsmEventStopping, (fsmEventCallback_t)fsmCallbackStopping, exampleFsmStateStopped});
    fsmAddStateEvent(&fsm, (fsmStateEvent_t){exampleFsmStateStopped, exampleFsmEventRestarting, (fsmEventCallback_t)fsmCallbackRestarting, exampleFsmStateStarted});
    fsmAddStateEvent(&fsm, (fsmStateEvent_t){exampleFsmStateStopped, exampleFsmEventEnd, (fsmEventCallback_t)fsmCallbackEnd, exampleFsmStateInit});
    
    fsmStart(&fsm, exampleFsmStateInit);
    fsmHistoryStart(&fsm, historyBuffer, 10);
    fsmGetState(&fsm, &state);
    printf("Current State: %d\r\n", state);

    fsmProcessEvent(&fsm, exampleFsmEventStarting, NULL);
    fsmGetState(&fsm, &state);
    printf("Current State: %d\r\n", state);

    fsmProcessEvent(&fsm, exampleFsmEventStopping, NULL);
    fsmGetState(&fsm, &state);
    printf("Current State: %d\r\n", state);

    fsmProcessEvent(&fsm, exampleFsmEventRestarting, NULL);
    fsmGetState(&fsm, &state);
    printf("Current State: %d\r\n", state);

    fsmProcessEvent(&fsm, exampleFsmEventStopping, NULL);
    fsmGetState(&fsm, &state);
    printf("Current State: %d\r\n", state);

    fsmProcessEvent(&fsm, exampleFsmEventEnd, NULL);
    fsmGetState(&fsm, &state);
    printf("Current State: %d\r\n", state);
    
    fsmHistoryPrint(&fsm, printf);

    return 0;
}