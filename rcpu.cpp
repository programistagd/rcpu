#include "rcpu.h"
#include <stack>
//#define DEBUG
#ifdef DEBUG
#include <iostream>
#endif
unsigned short CPU_Interrupt::run(unsigned short a,unsigned short b){
	return 0;
}
CPU_Interrupt::~CPU_Interrupt(){

}

void CPU::reset(){
	for(int i=0;i<65535;i++){
			memory[i]=0;
		}
	stack = std::stack<unsigned short>();
}

void CPU::load(std::vector<unsigned short> program){
    for(int i=0;i<program.size();i++){
        memory[i]=program[i];
    }
}

CPU::CPU(CPU_Interrupt* interrupts){
	reset();
	interrupt=interrupts;
}
void CPU::runOneCycle(){
	unsigned short * pos = &registers[13];
	unsigned short*source = memory;
	unsigned short cmd=memory[(*pos)];
		#ifdef DEBUG
			std::cout<<"Command "<<(int)cmd<<" (*pos) before: "<<(int)(*pos);
		#endif
	switch(cmd){
            
        //SET is not in order because it may be used very often
        case	20:
            (*pos)++;
            registers[source[(*pos)]]=registers[source[(*pos)+1]];
            (*pos)++;
            break;
        case	21:
            (*pos)++;
            registers[source[(*pos)]]=memory[source[(*pos)+1]];
            (*pos)++;
            break;
        case	22:
            (*pos)++;
            memory[source[(*pos)]]=registers[source[(*pos)+1]];
            (*pos)++;
            break;
        case	23:
            (*pos)++;
            registers[source[(*pos)]]=source[(*pos)+1];
            (*pos)++;
            break;
            
        //from now order is kept
	case	2:
		(*pos)++;
		registers[0]=registers[source[(*pos)]]+registers[source[(*pos)+1]];
		(*pos)++;
		break;
	case	3:
			(*pos)++;
			registers[0]=registers[source[(*pos)]]-registers[source[(*pos)+1]];
			(*pos)++;
			break;
	case	4:
			(*pos)++;
			registers[0]=registers[source[(*pos)]]*registers[source[(*pos)+1]];
			(*pos)++;
			break;
	case	5:
			(*pos)++;
			if(registers[source[(*pos)+1]]==0){
				registers[source[(*pos)]]=65535;
				registers[14]=1;
			}
			else
				registers[0]=registers[source[(*pos)]]/registers[source[(*pos)+1]];
			(*pos)++;
			break;
	case	6:
			(*pos)++;
			if(registers[source[(*pos)+1]]==0){
				registers[source[(*pos)]]=65535;
				registers[14]=1;
			}
			else
				registers[0]=registers[source[(*pos)]]%registers[source[(*pos)+1]];
			(*pos)++;
			break;
	case	7:
			(*pos)++;
			registers[0]=registers[source[(*pos)]]&registers[source[(*pos)+1]];
			(*pos)++;
			break;
	case	8:
			(*pos)++;
			registers[0]=registers[source[(*pos)]]|registers[source[(*pos)+1]];
			(*pos)++;
			break;
	case	9:
			(*pos)++;
			registers[0]=registers[source[(*pos)]]^registers[source[(*pos)+1]];
			(*pos)++;
			break;
	case	10:
			(*pos)++;
			registers[0]=!registers[source[(*pos)]];
			break;
	case	16:
		if(registers[source[(*pos)+1]]!=registers[source[(*pos)+2]]){
		if(source[(*pos)+3]==0)
			(*pos)++;
		else if(source[(*pos)+3]==10||source[(*pos)+3]==35||source[(*pos)+3]==37||source[(*pos)+3]==40)//IMPORTANT keep up to specs - skipping cmds sizes
			(*pos)+=2;
		else
			(*pos)+=3;
		}
		(*pos)+=2;
		break;
	case	17:
		if(registers[source[(*pos)+1]]<=registers[source[(*pos)+2]]){
			if(source[(*pos)+3]==0)
				(*pos)++;
			else if(source[(*pos)+3]==10||source[(*pos)+3]==35||source[(*pos)+3]==37||source[(*pos)+3]==40)//IMPORTANT keep up to specs - skipping cmds sizes
				(*pos)+=2;
			else
				(*pos)+=3;
		}
		(*pos)+=2;
		break;
	case 35:
		(*pos)++;
		if(stack.size()<16384)
			stack.push((unsigned short)registers[source[(*pos)]]);
		else
			registers[14]=6;
		break;
	case 37:
		(*pos)++;
		if(!stack.empty()){
			registers[source[(*pos)]]=stack.top();
			stack.pop();
		}
		else{
			registers[14]=7;
			registers[source[(*pos)]]=0;
		}
		break;

	case	40:
		switch(memory[source[(*pos)]]){
		default:
                    if(interrupt!=0 and memory[source[(*pos)]]>=17){
                            registers[0] = interrupt->run(registers[1],registers[2]);
                    }
                    break;
		}
		break;


	default:

break;
	}
	(*pos)++;//go to next cmd
#ifdef DEBUG
	std::cout<<" (*pos) after: "<<(int)(*pos)<<"\n";
	std::cin.get();
#endif
	//memory[0]=(*pos);
        registers[15]++;//increment internal timer
}

