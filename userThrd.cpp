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
#include "semaphor.h"

int userMainTestFirst(int argc, char* argv[]){
	class Nit : public Thread{
		public:
			Nit() : Thread(defaultStackSize, 0) {}
			~Nit()  {this->waitToComplete(); }
		protected:
			void run() {
				cout << "Usli u run\n";
				int i, j, k;
					for ( i =0; i < 100; ++i) {
						//lock;
						if(i == 50) PCB::running->getThreadById(3)->waitToComplete();
						cout<< "Nit: " << this->getId()  << ";  i = "<<i<<endl;
						//unlock;
						for ( k = 0; k<10000; ++k)
							for ( j = 0; j <30000; ++j);
					}
					cout << "Zavrsen run metod niti" << this->getId() << endl;
			}
		};

		/*
		Nit kita;
		cout << "Kita id: " << kita.getId() << endl;
		int i;
		int n = 30;
		Nit* niti[30];
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
	 	}*/

		Nit prva, druga;
		prva.start();
		druga.start();
		prva.waitToComplete();
		druga.waitToComplete();


	 	cout << "Sljaka toBa\n";
	 	return 0;
}

class Nit : public Thread{
public:
	static Semaphore nitSem;
	Nit() : Thread(defaultStackSize, 5) {}
	~Nit()  {this->waitToComplete(); }
protected:
	void run() {
		cout << "Usli u run\n";
		//cout << Nit::nitSem.myImpl->waitTimeList;
		nitSem.wait(0);
		int i, j, k;
			for ( i =0; i < 10; ++i) {
				//lock;
				cout<< "Nit: " << this->getId()  << ";  i = "<<i<<endl;
				//unlock;
				for ( k = 0; k<10000; ++k)
					for ( j = 0; j <30000; ++j);
			}
			cout << "Zavrsen run metod niti" << this->getId() << endl;
	}
};

Semaphore Nit::nitSem;

int UserThread::userMain(int argc, char* argv[]){


	Nit prva, druga;
	//Semaphore sem;
	prva.start();
	druga.start();
	cout << "Startovali niti\n";
	Nit::nitSem.wait(5);
	cout << "Odradili wait\n";
	int i, j, k;
	for ( i =0; i < 10; ++i) {
		//lock;
		cout<< "Main: ;  i = "<<i<<endl;
		//unlock;
		for ( k = 0; k<10000; ++k)
			for ( j = 0; j <30000; ++j);
	}

	Nit::nitSem.signal();
	Nit::nitSem.wait(5);
	cout << "Zavrsen main metod niti\n";


	prva.waitToComplete();
	druga.waitToComplete();

	return 0;
}

int UserThread::getRet(){
	return this->retMain;
}

UserThread::UserThread(int c, char** v, StackSize stackSize, Time timeSlice)  : Thread(stackSize, timeSlice), argc(c), argv(v) {
	cout << "Konstruktor UserThread-a\n";
	retMain = 0;
}

UserThread::~UserThread(){ }

void UserThread::run() {
	retMain = userMain(argc, argv);
}


