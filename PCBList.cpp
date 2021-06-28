/*
 * PCBList.cpp
 *
 *  Created on: Jun 23, 2021
 *      Author: OS1
 */

#include <limits.h>
#include "PCBList.h"
#include <IOSTREAM.H>


//zasto je size = 1?
PCBList::PCBList() : first(0), firstFree(0), size(0) { }


PCBList::~PCBList(){
		cout << "Destruktor PCBListe" << endl;
		deleteAll();
	}


int PCBList::isEmpty(){
		if(size > 0){
			return 0;
		}
		else return 1;
	}

int PCBList::isFull(){
		if(size == UINT_MAX){
			return 1;
		}
		else return 0;
	}


ID PCBList::insert(PCB* pcb){
		if(isFull()) return -1;

		Elem* newElem;
		int ret;
#ifndef BCC_BLOCK_IGNORE
			lock;
		//ako ubacujemo PCB na prvo mesto liste
		if(firstFree == 0){
			ret = 0;
			newElem = new Elem(pcb, first, ret);//ako nije jedini element u listi, next pokazuje na prethodno prvi. U suprotnom je next = 0
			first = newElem;
			firstFree = newElem;
		}
		//ako lista nije prazna
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
		cout << "UBACIO: " << ret << endl;
		unlock;
#endif
		return ret;
	}


int PCBList::remove(ID id){
		if(isEmpty()) return -1;

		Elem* cur, *prev = 0;
		//cur postavljamo na firstFree ako je on blizi trazenom elementu, u suprotnom na first
		if(firstFree && firstFree->id > first->id && id > firstFree->id) cur = firstFree;
		else cur = first;


		while(cur){


			//ako je pronadjen element za brisanje
			if(cur->id == id){
#ifndef BCC_BLOCK_IGNORE
		lock;
				cout << "Nasli ID\n";
				if(prev){
					prev->next = cur->next;
					//azuriramo firstFree ako je potrebno
					if(firstFree->id > prev->id) firstFree = prev;
				}
				else{
					cout << "Brisemo prvi\n";
					firstFree = 0;
					first = cur->next;
				}
				delete cur;
				size--;
				cout << "Obrisao: " << id << endl;
				cout << "Preostalo niti: " << this->size << endl;
		unlock;
#endif
				return 0;
			}
			//ako nije pronadjen element za brisanje
			else if(cur->id > id){
				return -2;
			}

			prev = cur;
			cur = cur->next;
		}


		//nije pronadjen element za brisanje
		return -2;
	}

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

Thread* PCBList::getThreadById(ID id){
	if(isEmpty()) return 0;

	Elem* cur;
	//cur postavljamo na firstFree ako je on blizi trazenom elementu, u suprotnom na first
	if(firstFree && firstFree->id > first->id && firstFree->id < id) cur = firstFree;
	else cur = first;

	while(cur){
		if(cur->id == id){
			return cur->data->myThread;
		}
		else if(cur->id > id){
			break;
		}
		cur = cur->next;
	}

	return 0;
}

void PCBList::deleteAll(){
		Elem* cur = first, *next = cur;
		while(cur){
#ifndef BCC_BLOCK_IGNORE
			lock;
			cout << "DELETE ALL id: " << cur->id << endl;
			unlock;
#endif
			next = cur->next;
			delete cur;
			cur = next;
		}
		size = 0;
	}

