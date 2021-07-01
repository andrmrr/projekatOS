/*
 * timeList.h
 *
 *  Created on: Jun 29, 2021
 *      Author: OS1
 */

#ifndef TIMELIST_H_
#define TIMELIST_H_

#include "baseList.h"

typedef unsigned int Time;

class TimeList : public BaseList<void*> {

public:
	struct TimeElem : public Elem{
		Time waitTime;
		TimeElem(void* const &p, Time wt, TimeElem* n = 0) : Elem(p){
			waitTime = wt;
			next = n;
		}
	};

	virtual void deleteAll();
	TimeList& add(void* const& p, Time wt);
	Time TimeList::getCurrentTime();
	void decTime();

	friend ostream& operator<<(ostream& os, const TimeList& l1) {
				//cout << "Wait times: ";
				for (Elem *cur = l1.first; cur; cur = cur->next) {
					os << ((TimeElem*)cur)->waitTime << " ";
				}
				cout << endl;
				return os;
			}
private:

	BaseList<void*>& add(void* const &t);

};




#endif /* TIMELIST_H_ */
