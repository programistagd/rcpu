#include "rcpu.h"
#include <iostream>
#include <fstream>

using namespace std;
CPU cpu;
int ret=0;
string filen="";
fstream file;
class ConsoleInterrupt : public CPU_Interrupt{
public:
	void memdump(int start,int end){
		cout<<"Memory dump:\n";
	for(int i=start;i<end;i++){
		cout<<i<<" - "<<cpu.memory[i]<<"\n";
	}
	}
	bool running;
	ConsoleInterrupt(){
		running=true;
	}
	unsigned short*run(unsigned short id,unsigned short a,unsigned short b){
		unsigned short*r=new unsigned short[2];
		r[0]=0;
		r[1]=0;
		if(id==100){
			running=false;
			ret=a;
			cout<<"Stopped\n";
		}
		else if(id==101){
			memdump(a,b);
		}
		else if(id==90){
			cout<<(char)a;
		}
		else if(id==91){
			r[0]=cin.get();
		}
		else if(id==93){
			filen="";
		}
		else if(id==94){
			filen+=(char)a;
		}
		else if(id==95){
			if(a==0)
				file.open(filen.c_str(),ios::in|ios::out|ios::binary);
			else
				file.open(filen.c_str(),ios::out|ios::binary);
		}
		else if(id==96){
			file.close();
		}
		else if(id==97){
			file.read((char*)&r[0],sizeof(unsigned short));
		}
		else if(id==98){
			file.write((char*)&a,sizeof(unsigned short));
		}
		return r;
	}
	~ConsoleInterrupt(){

	}
};

int main(int argc,char* argv[]){
	if(argc!=2){
		cout<<"Bad syntax!\nUsage: rcpu file.bin\n\n";
		return 1;
	}
	cout<<"Started\n";
	ConsoleInterrupt interrupts=ConsoleInterrupt();
	cpu = CPU(&interrupts);
	cout<<"Loading file "<<argv[1]<<" \n";
	ifstream input(argv[1],ios::in|ios::binary);
	unsigned short * program;
	input.seekg (0, ios::end);
	int end = input.tellg();
	input.seekg(0,ios::beg);
	int begin = input.tellg();
	program = new unsigned short[end-begin];
	input.read((char*)program,(end-begin));
	cout<<"Loaded\nProgram is "<<(end-begin)/2<<" words long\nLoading CPU...\n";
	cpu.load(program);
	cout<<"Program dump:\n";
	for(int i=0;i<(end-begin)/2;i++){
		cout<<i<<" - "<<program[i]<<"\n";
	}
	cout<<"\nStarting program...\n";
	while(interrupts.running){
		cpu.runOneCycle();
	}
	cout<<"Finished\n";
	return ret;
}
