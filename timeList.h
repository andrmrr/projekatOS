/*
 * timeList.h
 *
 *  Created on: Jun 29, 2021
 *      Author: OS1
 */

#ifndef TIMELIST_H_
#define TIMELIST_H_

#include "baseList.h"
#include "thread.h"

class TimeList : public BaseList<void*> {

protected:
	struct TimeElem : public Elem{
		Time waitTime;
		TimeElem(void* const &p, Time wt, TimeElem* n = 0) : Elem(p){
			waitTime = wt;
			next = n;
		}
	};

	virtual void deleteAll() {
		cout << "deleteAll TimeList\n";
		Elem* elem = first;
		while (first) {
			elem = first->next;
			delete (TimeElem*)first;
			first = elem;
		}
		last = current = beforeCurrent = 0;
	}


public:
	/*
	//testiraj jako
	TimeList& addd(void* const& p, Time wt){
		Time prevTime;
		for(goToFirst(), prevTime = 0; current && (prevTime += ((TimeElem*)current)->waitTime) < wt; goToNext()) {cout << "OPA!\n"; }
		//broj koji se upisuje za cekanje novog elementa
		Time newTime;
		if(current){
			//menja se waitTime sledece niti
			Time nextTime = prevTime - wt;
			((TimeElem*)current)->waitTime = nextTime;

			newTime = wt - (prevTime - ((TimeElem*)current)->waitTime);
		}
		else{
			newTime = wt - prevTime;
		}
		cout << "newTime: " << newTime << endl;
		current = (beforeCurrent ? beforeCurrent->next : first) = new TimeElem(p, newTime, (TimeElem*)current);
		if(!first->next || !current->next) last = first;
		return *this;
	}
*/
	TimeList& add(void* const& p, Time wt){
		Time prevTime = 0;
		for(goToFirst(); isCurrent(); goToNext()){
			prevTime += ((TimeElem*)current)->waitTime;
			if(prevTime > wt){
				//menja se waittime narednog elementa
				Time tmp = ((TimeElem*)current)->waitTime;
				((TimeElem*)current)->waitTime = prevTime - wt;

				//prevTime sada pokazuje waittime svih prethodnih elemenata
				prevTime -= tmp;
				cout << "Prev time: " << prevTime << "; Vreme sledeceg: " << ((TimeElem*)current)->waitTime << endl;
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
		cout << "Wait lista nakon novog elementa: " << *this;
		return *this;
	}


	Time getCurrentTime(){
			if (!current) cout << "Ne postoji element gettime\n";
			return ((TimeElem*)current)->waitTime;
		}

	void decTime(){
			if (!current) cout << "Ne postoji element dectime\n";
			((TimeElem*)current)->waitTime--;
		}

	friend ostream& operator<<(ostream& os, const TimeList& l1) {
			//cout << "Wait times: ";
			for (Elem *cur = l1.first; cur; cur = cur->next) {
				os << ((TimeElem*)cur)->waitTime << " ";
			}
			cout << endl;
			return os;
		}


private:
	//sakrivamo staru funkciju
	BaseList<void*>& add(void* const &t){
		return *this;
	}

};




#endif /* TIMELIST_H_ */
