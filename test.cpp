/*
 * test.cpp
 *
 *  Created on: Jun 23, 2021
 *      Author: OS1
 */

//adresa fajla
//C:\Users\OS1\workspace\javni\BCC

//komanda za izvrsavanje sa ASM fajlom
//bcc.exe -S -mh -v -ejavni.exe -IC:/bc31\include;..\; -LC:/bc31\lib;..\; ..\*.cpp  ..\*.lib


#include <iostream.h>
//#include "pcb.h"
//#include "PCBList.h"
#include "thread.h"
#include "system.h"
#include "general.h"

void f(int k, int j){
	int i;
	i = k - j;
	cout << i << endl;
}

int main(int argc, char** argv) {

/*
	PCB* a = PCB::createPCB(0);
	delete a;
	PCB* b = PCB::createPCB(0);
	int i;
	for(i = 0; i < 8; i++){
		PCB* c = PCB::createPCB(0);
	}

	PCB* d = PCB::createPCB(0);

	for(i = 0; i < 8; i++){
			PCB* c = PCB::createPCB(0);
		}

	delete d;

	delete b;
	d = PCB::createPCB(0);
	PCB* c = PCB::createPCB(0);
	d = PCB::createPCB(0);
	f(5, 7);
*/

/*
	class Nit : public Thread{
		public:
		void run()  {	cout << "Usli smo u nit: " << this->getId() << endl;	}

		Nit(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice) : Thread(stackSize, timeSlice) {}
		~Nit() {}
	};

	Nit *a = new Nit();
	Nit b;
	a->run();
	b.run();
	Nit *c = new Nit();
	int i = 0;
	Nit* nova;
	cout << "PETLJA\n";
	for(i; i < 10; i++ ){
		nova = new Nit();
	}


	c->run();
	delete c;
	cout << "POSLEDNJA 2\n";
	c = new Nit();
	cout << "Cid: "<< c->getId() << endl;
	cout << "a: " << a << endl;
	cout << "aThread: " << Thread::getThreadById(0) << endl;
	c = new Nit();
*/


	//inicijalizacija sistema
	System sys(argc, argv);

	//pokretanje korisnickog main-a
	cout << "Pokrecemo main nit\n";
	userMainThread->start();
	cout << "Pokrenuli main nit\n";
	//dispatch();
	//cout << "Prolazi dispatch()\n";
	userMainThread->waitToComplete();
	cout << "Zavrsila se main nit\n";
	return ((UserThread*)userMainThread)->getRet();
	//automatska restauracija sistema brisanjem sys na kraju main-a

}
