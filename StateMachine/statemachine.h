#ifndef NEZ_STATEMACHINE_H
#define NEZ_STATEMACHINE_H

#include "stdlib.h"

typedef struct{
	void (*StateEnter)(void *data);
	void (*StateExit)(void *data);
	void (*StateUpdate)(void *data);
}State;

#ifndef STATEMACHINE_HISTORY_SIZE
	#define STATEMACHINE_HISTORY_SIZE 5
#endif
typedef struct{
	// List of states.
	State *stateList;
	// Current state pointer
	State *state;
	// ID of current state
	int current;
	// List of previous states (#define STATEMACHINE_HISTORY_SIZE 5)
	// From last to oldest
	int previous[STATEMACHINE_HISTORY_SIZE];
	// Number of states added
	int stateCount;
}StateMachine;

#ifndef NEZSMAPI
    #ifdef NEZ_STATEMACHINE_STATIC
        #define NEZSMAPI static
    #else
        #define NEZSMAPI extern
    #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Create a new StateMachine instance with memory allocated for it.
// Use StateMachineDestroy to free memory.
NEZSMAPI StateMachine*
StateMachineNew();

// Free allocated memory for StateMachine
NEZSMAPI void
StateMachineDestroy(StateMachine *sm);

// Add a state to StateMachine's list.
// Returns states ID in the list for use in StateMachineTransition.
NEZSMAPI int
StateMachineAddState(StateMachine *sm, State state);

// Use it to start with a first state
NEZSMAPI void
StateMachineStart(StateMachine *sm, int id, void *data);

// Transitions out of current state into provided state.
// If state is current transition won't happen.
NEZSMAPI void
StateMachineTransition(StateMachine *sm, int id, void *data);

// Trigger update function for current state.
NEZSMAPI void
StateMachineUpdate(StateMachine *sm, void *data);

#ifdef __cplusplus
}
#endif
#endif // NEZ_STATEMACHINE_H

#ifdef NEZ_STATEMACHINE_IMPLEMENTATION
#undef NEZ_STATEMACHINE_IMPLEMENTATION

void StateMachineUpdateHistory(StateMachine *sm, int id){
	for(int i=1; i<STATEMACHINE_HISTORY_SIZE; i++){
		sm->previous[i] = sm->previous[i-1];
	}
	sm->previous[0] = id;
}

StateMachine*
StateMachineNew(){
	StateMachine *sm = (StateMachine*)malloc(sizeof(StateMachine));
	*sm = (StateMachine){0};
	sm->current = -1;
	
	for(int i=0; i<STATEMACHINE_HISTORY_SIZE; i++){
		sm->previous[i] = -1;
	}
	
	return sm;
}

void
StateMachineDestroy(StateMachine *sm){
	if (sm->stateList){free(sm->stateList);}
	if (sm->state){free(sm->state);}
	free(sm);
}

int
StateMachineAddState(StateMachine *sm, State state){
	State *old = sm->stateList;
	sm->stateList = (State*)malloc(sizeof(State) *(sm->stateCount +1));
	for(int i=0; i< sm->stateCount; i++){
		sm->stateList[i] = old[i];
	}
	sm->stateList[sm->stateCount] = state;
	sm->stateCount++;
	return sm->stateCount -1;
}

void
StateMachineStart(StateMachine *sm, int id, void *data){
	// state doesn't exists
	if (id >= sm->stateCount){return;}
	
	StateMachineUpdateHistory(sm, sm->current);
	sm->current = id;
	sm->state = &(sm->stateList[id]);
	sm->state->StateEnter(data);
}

void
StateMachineTransition(StateMachine *sm, int id, void *data){
	// state doesn't exists
	if (id> sm->stateCount){return;}
	
	sm->state->StateExit(data);
	
	StateMachineUpdateHistory(sm, sm->current);
	sm->current = id;
	sm->state = &(sm->stateList[id]);
	sm->state->StateEnter(data);
}

void
StateMachineUpdate(StateMachine *sm, void *data){
	sm->state->StateUpdate(data);
}

#endif // NEZ_STATEMACHINE_IMPLEMENTATION


