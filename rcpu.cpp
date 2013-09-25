#include "rcpu.h"
#include <stack>
//#define DEBUG
#ifdef DEBUG
#include <iostream>
#endif
unsigned short* CPU_Interrupt::run(unsigned short id,unsigned short a,unsigned short b){
	unsigned short *r =new unsigned short[2];
	r[0]=0;
	r[1]=1;
	return r;
}
CPU_Interrupt::~CPU_Interrupt(){

}

void CPU::reset(){
	if(stack!=0)
		delete stack;
	for(int i=0;i<256;i++){
			memory[i]=0;
		}
	stack=new std::stack<unsigned short>();
}

void CPU::load(unsigned short* data){
	program=data;
}

CPU::CPU(CPU_Interrupt* interrupts){
	reset();
	program=0;
	interrupt=interrupts;
}
void CPU::runOneCycle(){
	unsigned short * pos = &memory[0];
	unsigned short*source;
	if(memory[1]==0)
		source=program;
	else
		source=memory;
	unsigned short cmd=source[(*pos)];
		#ifdef DEBUG
			std::cout<<"Command "<<(int)cmd<<" (*pos) before: "<<(int)(*pos);
		#endif
			bool posfix=false;
	switch(cmd){
	case	1:
		(*pos)++;
		memory[source[(*pos)]]=memory[source[(*pos)+1]];
		(*pos)++;
		break;
	case	2:
		(*pos)++;
		memory[source[(*pos)]]=memory[source[(*pos)]]+memory[source[(*pos)+1]];
		(*pos)++;
		break;
	case	3:
			(*pos)++;
			memory[source[(*pos)]]=memory[source[(*pos)]]-memory[source[(*pos)+1]];
			(*pos)++;
			break;
	case	4:
			(*pos)++;
			memory[source[(*pos)]]=memory[source[(*pos)]]*memory[source[(*pos)+1]];
			(*pos)++;
			break;
	case	5:
			(*pos)++;
			if(memory[source[(*pos)+1]]==0){
				memory[source[(*pos)]]=65535;
				memory[2]=1;
			}
			else
				memory[source[(*pos)]]=memory[source[(*pos)]]/memory[source[(*pos)+1]];
			(*pos)++;
			break;
	case	6:
			(*pos)++;
			if(memory[source[(*pos)+1]]==0){
				memory[source[(*pos)]]=65535;
				memory[2]=1;
			}
			else
				memory[source[(*pos)]]=memory[source[(*pos)]]%memory[source[(*pos)+1]];
			(*pos)++;
			break;
	case	7:
			(*pos)++;
			memory[source[(*pos)]]=memory[source[(*pos)]]&memory[source[(*pos)+1]];
			(*pos)++;
			break;
	case	8:
			(*pos)++;
			memory[source[(*pos)]]=memory[source[(*pos)]]|memory[source[(*pos)+1]];
			(*pos)++;
			break;
	case	9:
			(*pos)++;
			memory[source[(*pos)]]=memory[source[(*pos)]]^memory[source[(*pos)+1]];
			(*pos)++;
			break;
	case	11:
		(*pos)++;
#ifdef DEBUG
		std::cout<<" SETV - "<<source[(*pos)]<<" = "<<source[(*pos)+1]<<"\n";
#endif
		if(source[(*pos)]==0)
			posfix=true;
		memory[source[(*pos)]]=source[(*pos)+1];
		if(!posfix)
			(*pos)++;
		break;
	case	12:
		(*pos)++;
		memory[source[(*pos)]]=memory[source[(*pos)]]+source[(*pos)+1];
		(*pos)++;
		break;
	case	13:
			(*pos)++;
			memory[source[(*pos)]]=memory[source[(*pos)]]-source[(*pos)+1];
			(*pos)++;
			break;
	case	14:
			(*pos)++;
			memory[source[(*pos)]]=memory[source[(*pos)]]*source[(*pos)+1];
			(*pos)++;
			break;
	case	15:
			(*pos)++;
			memory[source[(*pos)]]=memory[source[(*pos)]]/source[(*pos)+1];
			(*pos)++;
			break;
	case	16:
			(*pos)++;
			if(source[(*pos)+1]==0){
				memory[source[(*pos)]]=65535;
				memory[2]=1;
			}
			else
				memory[source[(*pos)]]=memory[source[(*pos)]]%source[(*pos)+1];
			(*pos)++;
			break;
	case	17:
			(*pos)++;
			if(source[(*pos)+1]==0){
				memory[source[(*pos)]]=65535;
				memory[2]=1;
			}
			else
				memory[source[(*pos)]]=memory[source[(*pos)]]&source[(*pos)+1];
			(*pos)++;
			break;
	case	18:
			(*pos)++;
			memory[source[(*pos)]]=memory[source[(*pos)]]|source[(*pos)+1];
			(*pos)++;
			break;
	case	19:
			(*pos)++;
			memory[source[(*pos)]]=memory[source[(*pos)]]^source[(*pos)+1];
			(*pos)++;
			break;
	case	21:
		(*pos)++;
		memory[memory[source[(*pos)]]]=memory[memory[source[(*pos)+1]]];
		(*pos)++;
		break;
	case	22:
		(*pos)++;
		memory[memory[source[(*pos)]]]=memory[memory[source[(*pos)]]]+memory[memory[source[(*pos)+1]]];
		(*pos)++;
		break;
	case	23:
			(*pos)++;
			memory[memory[source[(*pos)]]]=memory[memory[source[(*pos)]]]-memory[memory[source[(*pos)+1]]];
			(*pos)++;
			break;
	case	24:
			(*pos)++;
			memory[memory[source[(*pos)]]]=memory[memory[source[(*pos)]]]*memory[memory[source[(*pos)+1]]];
			(*pos)++;
			break;
	case	25:
			(*pos)++;
			if(memory[memory[source[(*pos)+1]]]==0){
				memory[memory[source[(*pos)]]]=65535;
				memory[2]=1;
			}
			else
				memory[memory[source[(*pos)]]]=memory[memory[source[(*pos)]]]/memory[memory[source[(*pos)+1]]];
			(*pos)++;
			break;
	case	26:
			(*pos)++;
			if(memory[memory[source[(*pos)+1]]]==0){
				memory[memory[source[(*pos)]]]=65535;
				memory[2]=1;
			}
			else
				memory[memory[source[(*pos)]]]=memory[memory[source[(*pos)]]]%memory[memory[source[(*pos)+1]]];
			(*pos)++;
			break;
	case	27:
			(*pos)++;
			memory[memory[source[(*pos)]]]=memory[memory[source[(*pos)]]]&memory[memory[source[(*pos)+1]]];
			(*pos)++;
			break;
	case	28:
			(*pos)++;
			memory[memory[source[(*pos)]]]=memory[memory[source[(*pos)]]]|memory[memory[source[(*pos)+1]]];
			(*pos)++;
			break;
	case	29:
			(*pos)++;
			memory[memory[source[(*pos)]]]=memory[memory[source[(*pos)]]]^memory[memory[source[(*pos)+1]]];
			(*pos)++;
			break;
	case	31:
			(*pos)++;
			memory[source[(*pos)]]=!memory[source[(*pos)]];
			break;
	case	32:
		if(memory[source[(*pos)+1]]!=memory[source[(*pos)+2]]){
		if(source[(*pos)+3]==40||source[(*pos)+3]==0)
			(*pos)++;
		else if(source[(*pos)+3]==31||source[(*pos)+3]==35||source[(*pos)+3]==36||source[(*pos)+3]==37)
			(*pos)+=2;
		else
			(*pos)+=3;
		}
		(*pos)+=2;
		break;
	case	33:
		if(memory[source[(*pos)+1]]<=memory[source[(*pos)+2]]){
			if(source[(*pos)+3]==40||source[(*pos)+3]==0)
				(*pos)++;
			else if(source[(*pos)+3]==31||source[(*pos)+3]==35||source[(*pos)+3]==36||source[(*pos)+3]==37)
				(*pos)+=2;
			else
				(*pos)+=3;
		}
		(*pos)+=2;
		break;
	case 35:
		(*pos)++;
		if(stack->size()<8192)
			stack->push((unsigned short)memory[source[(*pos)]]);
		else
			memory[2]=6;
		break;
	case 36:
		(*pos)++;
		if(stack->size()<8192)
			stack->push((unsigned short)source[(*pos)]);
		else
			memory[2]=6;
		break;
	case 37:
		(*pos)++;
		if(!stack->empty()){
			memory[source[(*pos)]]=stack->top();
			stack->pop();
		}
		else{
			memory[2]=7;
			memory[source[(*pos)]]=0;
		}
		break;

	case	40:
		switch(memory[9]){
		default:
		if(interrupt!=0 and memory[9]>=17){
			unsigned short* ret=interrupt->run(memory[9],memory[5],memory[6]);
			memory[7]=ret[0];
			memory[8]=ret[1];
		}
		break;
		}
		break;


	default:

break;
	}
	if(!posfix)(*pos)++;
#ifdef DEBUG
	std::cout<<" (*pos) after: "<<(int)(*pos)<<"\n";
	std::cin.get();
#endif
	//memory[0]=(*pos);
}

