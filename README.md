# Finite State Machine Library

A simple finite state machine library in C.

## Dependancies

Current dependancies are `stdint.h` and `stdio.h`.

## Example

see example folder for `main.c`

## Usage

The following sections are provided as a guideline of how to quickly use and interact with the fsm library.

## Basic Initiallization

This example shows the basic requirements of creating and initiallzing a simple fsm.

```c code

// Create new local buffer, serves as storage for the state machine.
fsmStateEvent_t buffer[NUM_OF_STATE_EVENTS];

// Create new local handle, serves as manager and settings for the state machine.
fsmHandle_t fsm;

// Creates the external callback function that is called by the state machine when an event is proccessed.
fsmStatus_t fooCallback(void* pParam) {
    // do something here.
    // returned status is consumed by the state machine.
    return fsmStatusOk;
}

// Define State/Event
fsmStateEvent_t fooStateEvent = {
    .currentState = 0,
    .event = 1,
    .callback = fooCallback,
    .nextState = 1
}

// Initiallizing the state machine handle structure.
if (fsmInit(&fsm, buffer, NUM_OF_STATE_EVENTS) == fsmStatusOk) {
    
    // Add the state/event to the state machine.
    fsmAddStateEvent(&fsm, fooStateEvent);

    ...

    // Start the state machine in a known state.
    fsmStart(&fsm, 0);

} else {
    
    // Failed to initialize the fsm, determine error and cause.

}

```

## Proccessing State Machine Events

```c code

// Process State Machine Event
fsmProcessEvent(&fsm, 1, NULL);

```

## Get Current State of State Machine

```c code

// Create new local state.
uint32_t state = 0;

// Get State from state machine.
fsmGetState(&fsm, &state);

```

## Enable History Tracking

To enable history tracking, ensure the define below is set. The definition can be found at the top of `fsm.h`
```c code
#define HISTORY
```


```c code

// Create new local history buffer, serves as storage for the state machine history.
fsmStateEvent_t historyBuffer[NUM_OF_STATE_EVENTS_HISTORY];

// Start the state machine history storage.
fsmHistoryStart(&fsm, historyBuffer, NUM_OF_STATE_EVENTS_HISTORY);

```

## Print State Machine History

Prints the current state machine history. History is printed from the newest to oldest.

```c code

// Print history buffer.
fsmHistoryPrint(&fsm, printf);

```