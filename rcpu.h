/*		RCPU-16 specs
 *
 *	CPU operates on 2 byte(16bit) words
 *	CPU reads one word (command) and then (if there are any - it depends on type of command) words with arguments
 *
 *		MAX=65535(two bytes - in C++ unsigned short)
 *
 *	Memory contains 65536 words and is addressed by one word
 *		0		-	CUR - First word of memory is reserved for "runtime pointer", it's the current position of running the code, it can be changed
 *						IMPORTANT:	This is the pointer of word in the program which will be run, so it can point to a value, what would probably cause fatal errors
 *		1		-	if 0 - the program will run from program data, if non-zero it'll run from memory(so you can run some dynamic programs)
 *		2		-	error indicator: normally 0
 *		3-4		-	reserved
 *		5		-	interrupt's first argument
 *		6		-	interrupt's second argument
 *		7		-	interrupt's first returned value
 *		8		-	interrupt's second returned value
 *		9		-	interrupt's id
 *		10-32	-	reserved
 *		33-65535	-	memory for use by user
 *
 *
 *	Supported commands:
 *		Functions from 0-9 defaultly operate on addresses, so first argument is an address in memory and the second is also the address in memory, if you want the second argument to be a value add 10 to the operation id(in language add letter V, eg. SETV), if you want both arguments to be addresses in memory of addresses in memory add 20 to the op id(in language add letter A, eg. SETA)
 *		*	0	-	NOP		-	empty command(nothing, if found, will be omitted, but as normal command will cost one tick)
 *		*	1	-	SET	a,b	-	sets value of a to value of b
 *		*	2	-	ADD a,b	-	a = a+b	if
 *		*	3	-	SUB a,b	-	a = a-b if
 *		*	4	-	MUL a,b	-	a = a*b if
 *		*	5	-	DIV a,b	-	a = a/b	if b==0 then error indicator will have value 1 and a will have value MAX
 *		*	6	-	MOD a,b	-	a = a%b if b==0 then error indicator will have value 1 and a will have value MAX
 *
 *		*	7	-	AND a,b	-	a = a&b
 *		*	8	-	OR	a,b	-	a = a|b
 *		*	9	-	XOR a,b	-	a = a^b
 *
 *		*	10-30	-	operations as 0-9 but on values or addresses to addresses
 *
 *		*	31	-	NOT a	-	a = !a (a has to be memory address)
 *
 *		*	32	-	IFE	a,b	-	next instruction is performed if a==b (a,b has to be memory address)
 *		*	33	-	IFG	a,b	-	next instruction is performed if a>b (a,b has to be memory address)
 *
 *		*	35	-	PUSH a	-	pushes value from memory with address a on the stack, sets error indicator to 6 on stack overflow
 *		*	36	-	PUSHV a	-	pushes value on the stack, sets error indicator to 6 on stack overflow
 *		*	37	-	POP a	-	pops value from stack and sets memory a to it, if stack is empty sets a to 0 and sets error indicator to 7
 *
 *		*	40	-	INT		-	runs the interrupt(see memory)
 *
 *		*	41+(and others)	-	reserved
 *
 *	Interrupts:
 *		*	0-16	-	reserved
 *		*	17-65535	-	for use by implementation
 */

#ifndef RCPU8_H
#define RCPU8_H
#include <stack>

class CPU_Interrupt{
public:
	virtual unsigned short* run(unsigned short id,unsigned short a,unsigned short b);//has to return 2 words, which will be put into memory
	virtual ~CPU_Interrupt();
};
class CPU{
	unsigned short * program;
	CPU_Interrupt* interrupt;
	std::stack<unsigned short> * stack;
public:
	unsigned short memory[65536];
	CPU(CPU_Interrupt* interrupts=0);
	void runOneCycle();
	void reset();
	void load(unsigned short * data);//you should reset before loading
};

#endif