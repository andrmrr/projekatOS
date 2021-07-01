/*
 * timeList.cpp
 *
 *  Created on: Jun 30, 2021
 *      Author: OS1
 */

#include "general.h"
#include "timeList.h"

void TimeList::deleteAll() {
	cout << "deleteAll TimeList\n";
	Elem* elem = first;
	while (first) {
		elem = first->next;
		delete (TimeElem*)first;
		first = elem;
	}
	last = current = beforeCurrent = 0;
	}

TimeList& TimeList::add(void* const& p, Time wt){
#ifndef BCC_BLOCK_IGNORE
		lock;
		Time prevTime = 0;
		for(goToFirst(); isCurrent(); goToNext()){
			prevTime += ((TimeElem*)current)->waitTime;
			if(prevTime > wt){
				//menja se waittime narednog elementa
				Time tmp = ((TimeElem*)current)->waitTime;
				((TimeElem*)current)->waitTime = prevTime - wt;

				//prevTime sada pokazuje waittime svih prethodnih elemenata
				prevTime -= tmp;
				//cout << "Prev time: " << prevTime << "; Vreme sledeceg: " << ((TimeElem*)current)->waitTime << endl;
				break;
			}
		}
		//waittime novog elementa
		Time newTime = wt - prevTime;
		//prevezujemo pokazivace
		(beforeCurrent ? beforeCurrent->next : first) = current = new TimeElem(p, newTime, (TimeElem*)current);

		//ako je lista prazna ili ako novi element ima najveci waittime(na kraju liste)
		if(!current){
			last = current;
		}
		//cout << "Wait lista nakon novog elementa: " << *this;
		unlock;
#endif
		return *this;
	}


Time TimeList::getCurrentTime(){
		if (!current) cout << "Ne postoji element gettime\n";
		return ((TimeElem*)current)->waitTime;
	}

void TimeList::setCurrentTime(Time newTime){
	if (!current) cout << "Ne postoji element dectime\n";
			((TimeElem*)current)->waitTime = newTime;
}

void TimeList::decTime(){
		if (!current) cout << "Ne postoji element dectime\n";
		((TimeElem*)current)->waitTime--;
	}


	//sakrivamo staru funkciju
	BaseList<void*>& TimeList::add(void* const &t){
		return *this;
	}



