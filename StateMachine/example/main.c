#include "stdio.h"

#define NEZ_STATEMACHINE_IMPLEMENTATION
#include "statemachine.h"

int testVar = 0;

void IdleEnter(void *data){
	testVar = 1;
}
void IdleExit(void *data){
	printf("Idle state exits\n");
}
void IdleUpdate(void *data){
	float delta = *((float*)data);
	printf("Idle state update: %f\n", delta);
}
void MoveEnter(void *data){
	testVar = 99;
}
void MoveExit(void *data){
	printf("Move state exits\n");
}
void MoveUpdate(void *data){
	float delta = *((float*)data);
	printf("Move state update: %f\n", delta);
}

int main(){
	printf("testVar Begining: %d\n", testVar);
	
	State idle = (State){IdleEnter, IdleExit, IdleUpdate};
	State move = (State){MoveEnter, MoveExit, MoveUpdate};
	printf("States created\n");
	
	StateMachine *sm = StateMachineNew();
	int idleId = StateMachineAddState(sm, idle);
	int moveId = StateMachineAddState(sm, move);
	printf("States Added\n");
	
	StateMachineStart(sm, idleId, NULL);
	printf("testVar post idle enter: %d\n", testVar);
	
	float dt = 1.0/60;
	StateMachineUpdate(sm, (void*)&dt);
	// Idle update printed dt value
	
	StateMachineTransition(sm, moveId, NULL);
	printf("testVar post transition to Move: %d\n", testVar);
	
	dt = 1.0/30;
	StateMachineUpdate(sm, (void*)&dt);
	// Move update printed dt value
	
	StateMachineDestroy(sm);
	return 0;
}