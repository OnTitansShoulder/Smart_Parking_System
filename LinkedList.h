#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "Graphics_Car.h"

//Combined the ListNode and LinkedList together for the sake of the program's compilation

//Node object is implemented as double-linkedlist
class Node{
private:
	int m_ufid;
	int m_floorNum;
	int m_slotNum;
	Car* car;
	Node* next;
	Node* previous;
public:
	//constructor
	Node(int id, int fl, int slot, Car* c) {
		m_ufid = id;
		m_slotNum = slot;
		m_floorNum = fl;
		car = c;
		next = NULL;
		previous = NULL;
	}

	int getUFID() { return m_ufid; }
	int getSlotNum() { return m_slotNum; }
	int getFloorNum() { return m_floorNum; }
	Node* getNext();
	Node* getPrevious();
	void setNext(Node* nextNode);
	void setPrevious(Node* previousNode);
	void Leave();
};

//LinkedList, keep track of user's cars information, as well as handling user adding
//Cars and removing the cars
class LinkedList{
private:
    Node* head;
	Node* tail;
	int size;
public:
    LinkedList(){
		size = 0;
		head = new Node(-1, -1, -1, 0);
		tail = head;
    }

	Node* find(int ufid);
	void add(Node* parker);
	void removeUser(int ufid);

};


#endif