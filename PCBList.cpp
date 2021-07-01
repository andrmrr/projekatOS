/*
 * PCBList.cpp
 *
 *  Created on: Jun 23, 2021
 *      Author: OS1
 */

#include <limits.h>
#include "PCBList.h"
#include <IOSTREAM.H>



PCBList::PCBList() : first(0), firstFree(0), size(0) { }


PCBList::~PCBList(){
		//cout << "Destruktor PCBListe" << endl;
		deleteAll();
	}


int PCBList::isEmpty(){
		if(size == 0){
			return 1;
		}
		else return 0;
	}

int PCBList::isFull(){
		if(size == UINT_MAX){
			return 1;
		}
		else return 0;
	}


ID PCBList::insert(PCB* pcb){
#ifndef BCC_BLOCK_IGNORE
		int ret = -10;
		lock;
		if(isFull()) ret = -1;
		if(ret == -1)
		{
			unlock;
			return ret;
		}

		Elem* newElem;
		//ako ubacujemo PCB na prvo mesto liste
		if(firstFree == 0){
			ret = 0;
			newElem = new Elem(pcb, first, ret);//ako nije jedini element u listi, next pokazuje na prethodno prvi. U suprotnom je next = 0
			first = newElem;
			firstFree = newElem;
		}
		else{
			ret = firstFree->id + 1;
			newElem = new Elem(pcb, firstFree->next, ret);
			firstFree->next = newElem;
			firstFree = newElem;
		}


		//trazimo prvi sledeci slobodan element
		while(firstFree->next != 0 && firstFree->next->id - firstFree->id <= 1) {
			firstFree = firstFree->next;
		}

		size++;
		//cout << "UBACIO: " << ret << endl;
		unlock;
		return ret;
#endif
	}


int PCBList::remove(ID id){
#ifndef BCC_BLOCK_IGNORE
		int ret = -2;
		lock;
		if(isEmpty()) ret = -1;
		if(ret == -1)
		{
			unlock;
			return ret;
		}


		Elem* cur, *prev = 0;
		//cur postavljamo na firstFree ako je on blizi trazenom elementu, u suprotnom na first
		if(firstFree && firstFree->id > first->id && id > firstFree->id) cur = firstFree;
		else cur = first;


		while(cur){

			//ako je pronadjen element za brisanje
			if(cur->id == id){
				//cout << "Nasli ID\n";
				if(prev){
					prev->next = cur->next;
					//azuriramo firstFree ako je potrebno
					if(firstFree && firstFree->id > prev->id) firstFree = prev;
				}
				else{
				//	cout << "Brisemo prvi\n";
					firstFree = 0;
					first = cur->next;
				}
				delete cur;
				size--;
				//cout << "Obrisao: " << id << endl;
				//cout << "Preostalo niti: " << this->size << endl;
				ret = 0;
				break;
			}
			//ako nije pronadjen element za brisanje
			else if(cur->id > id){
				ret = -2;
				break;
			}

			prev = cur;
			cur = cur->next;
		}

		//nije pronadjen element za brisanje
		unlock;
		return ret;
#endif
	}
/*
ID PCBList::getID(Thread* t){
	if(isEmpty()) return -1;

	Elem* cur = first;
	while(cur){
		if(cur->data->myThread == t){
			return cur->id;
		}
		cur = cur->next;
	}

	return -1;
}
*/

Thread* PCBList::getThreadById(ID id){
	Elem* cur;
	Thread* ret = 0;
#ifndef BCC_BLOCK_IGNORE
	lock;
	//cur postavljamo na firstFree ako je on blizi trazenom elementu, u suprotnom na first
	if(firstFree && firstFree->id > first->id && firstFree->id < id) cur = firstFree;
	else cur = first;

	while(cur){
		if(cur->id == id){
			ret = cur->data->myThread;
			break;
		}
		else if(cur->id > id){
			break;
		}
		cur = cur->next;
	}
	unlock;
#endif
	return ret;
}

void PCBList::deleteAll(){
#ifndef BCC_BLOCK_IGNORE
		lock;
		Elem* cur = first, *nextt = 0;
		while(cur){
			//syncPrintf("DELETE ALL id: %d\n", cur->id);
			nextt = cur->next;
			delete cur;
			cur = nextt;
		}
		size = 0;
		first = firstFree = 0;
		unlock;
#endif
	}

