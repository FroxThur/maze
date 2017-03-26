#include "includes.h" 



void InitStack (stack *s){

	s->top = 0;
}

int StackEmpty(stack *s){
	if (s->top == 0)
		return 1;
	else
		return 0;
}

int StackFull(stack *s){
	if (s->top == S_STACK)
		return 1;
	else
		return 0;
}

void PushStack(stack *s, stackElmt e){
	if(!StackFull(s))
		s->ElemArr[(s->top)++] = e;
	else{
		fprintf(stderr, "%s\n", "Stack Full !");
		exit(EXIT_FAILURE);
	}
}

stackElmt PullStack(stack *s){
	if(!StackEmpty(s))
		return s->ElemArr[--(s->top)];
	else{
		fprintf(stderr, "%s\n", "Stack Empty !");	
		exit(EXIT_FAILURE);
	}
}

