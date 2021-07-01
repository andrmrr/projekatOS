/*
 * userThread.cpp
 *
 *  Created on: Jun 24, 2021
 *      Author: OS1
 */

#include "userThrd.h"
#include "pcb.h"
#include "thread.h"
#include <IOSTREAM.H>
#include "general.h"
#include "timer.h";
//#include "semaphor.h"
//#include "event.h"

/*
int userMainTestFirst(int argc, char* argv[]){
	class Nit : public Thread{
		public:
			Nit() : Thread(defaultStackSize, 5) {}
			~Nit()  {this->waitToComplete(); }
		protected:
			void run() {
				syncPrintf("Usli u run\n");//cout << "Usli u run\n";
				int i, j, k;
				for ( i =0; i < 10; ++i) {
					//if(i == 50) PCB::running->getThreadById(3)->waitToComplete();
					syncPrintf("Nit: %d; i = %d\n", this->getId(), i);//cout<< "Nit: " << this->getId()  << ";  i = "<<i<<endl;"

					for ( k = 0; k<10000; ++k)
						for ( j = 0; j <30000; ++j);
				}
				syncPrintf("Zavrsen run metod niti%d\n", this->getId());//cout << "Zavrsen run metod niti" << this->getId() << endl;
			}
		};


		Nit kita;
		cout << "Kita id: " << kita.getId() << endl;
		int i;
		int n = 100;
		Nit* niti[100];
		for(i = 0; i < n; i++){
			niti[i] = new Nit();
			niti[i]->start();
		}


		cout << "Pali se kita\n";
	 	kita.start();

	 	int ii;
	 	for(ii = 0; ii < 100; ii ++) cout << "OPA!";

	 	for(i = 0; i < n; i++){
	 		delete niti[i];
	 	}

		Nit prva, druga;
		prva.start();
		druga.start();
		prva.waitToComplete();
		druga.waitToComplete();


	 	cout << "Sljaka toBa\n";
	 	return 0;
}
*/

/*
class Nit : public Thread{
public:
	static Semaphore nitSem[10];
	Nit() : Thread(defaultStackSize, 5) {}
	~Nit()  {this->waitToComplete(); }
protected:
	void run();
};

void Nit::run() {
		//cout << "Usli u run\n";
		//cout << Nit::nitSem.myImpl->waitTimeList;
		nitSem[getId() % 10].wait(getId()*2);
		int i, j, k;
			for ( i =0; i < 10; ++i) {
#ifndef BCC_BLOCK_IGNORE
				lock;
				cout<< "Nit: " << this->getId()  << ";  i = "<<i<<endl;
				unlock;
#endif
				for ( k = 0; k<10000; ++k)
					for ( j = 0; j <30000; ++j);
			}
#ifndef BCC_BLOCK_IGNORE
			lock;
			cout << "Zavrsen run metod niti" << this->getId() << endl;
			unlock;
#endif
	}

Semaphore Nit::nitSem[10];

int userMainTestSecond(int argc, char* argv[]){
	Nit niti[50];
		int i, j, k;
		for(i = 0; i < 50; i++){
			niti[i].start();
		}
		//Nit prva, druga;
		//Semaphore sem;
		//prva.start();
		//druga.start();
		//cout << "Startovali niti\n";
		Nit::nitSem[0].wait(5);
		//cout << "Odradili wait\n";
		for ( i =0; i < 10; ++i) {
	#ifndef BCC_BLOCK_IGNORE
			lock;
			cout<< "Main: ;  i = "<<i<<endl;
			unlock;
	#endif
			for ( k = 0; k<10000; ++k)
				for ( j = 0; j <30000; ++j);
		}

		Nit::nitSem[0].signal();
		Nit::nitSem[0].wait(5);
	#ifndef BCC_BLOCK_IGNORE
		lock;
		cout << "Zavrsen main metod niti\n";
		unlock;
	#endif

		//prva.waitToComplete();
		//druga.waitToComplete();

		return 0;
}
*/

int userMain(int argc, char* argv[]);

int UserThread::getRet(){
	return this->retMain;
}

UserThread::UserThread(int c, char** v, StackSize stackSize, Time timeSlice)  : Thread(stackSize, timeSlice), argc(c), argv(v) {
	//cout << "Konstruktor UserThread-a\n";
	retMain = 0;
}

UserThread::~UserThread(){ this->waitToComplete(); }

void UserThread::run() {
	//int userMain(int argc, char* argv[]);
	char* argvv[4];
	argvv[1] = "2048";
	argvv[2] = "1";
	argvv[3] = "1";

	retMain = userMain(4, argvv);
	//retMain = userMain(argc, argv);
}


