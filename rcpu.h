/*		RCPU-16 specs
 *
 *	CPU operates on 2 byte(16bit) words
 *	CPU reads one word (command) and then (if there are any - it depends on type of command) words with arguments
 *
 *		MAX=65535(two bytes - in C++ unsigned short)
 *
 *	Memory contains 65536 words and is addressed by one word
 *		The whole range is meant for the user.
 *              The memory is shared for program and general-use (RAM) memory
 *
 *      Registers (0-16):
 *      0       - common op result (can be used as general register)
 *      1-12     - general use registers (12)
 *      13      - current cursor position (incremented with each step, can be changed manually or with JMP)
 *      14      - error indicator (0=no error)
 *      15      - reserved
 * 
 *	Supported commands:
 *              a,b - registers id's (1-8)
 *              c - register 0
 *              m - place in memory
 *		*	0	-	NOP		-	empty command(nothing, if found, will be omitted, but as normal command will cost one tick)
 *		*	1	-	reserved?
 *		*	2	-	ADD a,b	-	c = a+b	if
 *		*	3	-	SUB a,b	-	c = a-b if
 *		*	4	-	MUL a,b	-	c = a*b if
 *		*	5	-	DIV a,b	-	c = a/b	if b==0 then error indicator will have value 1 and a will have value MAX
 *		*	6	-	MOD a,b	-	c = a%b if b==0 then error indicator will have value 1 and a will have value MAX
 *
 *		*	7	-	AND a,b	-	c = a&b
 *		*	8	-	OR  a,b	-	c = a|b
 *		*	9	-	XOR a,b	-	c = a^b
 *
 *		*	10	-	NOT a	-	c = !a
 *
 *		*	16	-	IFE	a,b	-	next instruction is performed if a==b
 *		*	17	-	IFG	a,b	-	next instruction is performed if a>b
 *
 *              *       20      -       SET     a,b     ]
 *              *       21      -       SET     a,m     ]       those sets first argument to value of second argument
 *              *       22      -       SET     m,a     ]
 * 
 *		*	35	-	PUSH a	-	pushes value from register a on the stack, sets error indicator to 6 on stack overflow
 *		*	37	-	POP a	-	pops value from stack and sets register a to it, if stack is empty sets a to 0 and sets error indicator to 7
 *
 *		*	40	-	CALL a		-	runs the special function (id a) with arguments from registers 1 and 2, result is stored in 0
 *
 *		*	41+(and others)	-	reserved
 *
 *	Special functions:
 *		*	0-16	-	reserved
 *		*	17-65535	-	for use by implementation
 */

#ifndef RCPU8_H
#define RCPU8_H
#include <stack>
#include <vector>
#define MAX 65535

class CPU_Interrupt{
public:
	virtual unsigned short run(unsigned short a,unsigned short b);//has to return 2 words, which will be put into memory
	virtual ~CPU_Interrupt();
};
class CPU{
	CPU_Interrupt* interrupt;
	std::stack<unsigned short> stack;
public:
	unsigned short memory[65536];
        unsigned short registers[16];
	CPU(CPU_Interrupt* interrupts=0);
	void runOneCycle();
	void reset();
	void load(std::vector<unsigned short> program);
};

#endif