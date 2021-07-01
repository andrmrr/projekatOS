/*
 * PCBList.h
 *
 *  Created on: Jun 22, 2021
 *      Author: OS1
 */

#ifndef PCBList_H_
#define PCBList_H_

#include "general.h"

class PCB;
class Thread;

//ulancana lista
class PCBList {

	struct Elem{
		PCB* data;
		ID id;
		Elem* next;
		Elem(PCB* d, Elem* n, ID i) : data(d), next(n), id(i) { }
	};

	Elem* first, *firstFree;
	unsigned size;//maksimum je 65535

public:

	PCBList();
	~PCBList();

	int isEmpty();
	int isFull();

	//vraca -1 ako je neuspesan poziv, u suprotnom vraca ID novog elementa
	ID insert(PCB* pcb);
	//vraca 0 ako je uspsan poziv, <0 ako je neuspesan
	int remove(ID id);// efikasnije je iterirati po listi preko ID-a

	//vraca -1 ako je neuspesan poziv
	//ID getID(Thread* t);

	//vraca 0 ako je neuspesan poziv
	Thread* getThreadById(ID id);

private:
	//brise listu sa sve PCBovima na koje pokazuje
	void deleteAll();

};




#endif /* PCBList_H_ */
