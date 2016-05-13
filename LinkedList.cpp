#include "LinkedList.h"

//get the next node
Node* Node::getNext() {
	return next;
}

//get the previous node
Node* Node::getPrevious() {
	return previous;
}

//set the next node with the node passed in
void Node::setNext(Node* nextNode) {
	next = nextNode;
}

//set the previous node with the node passed in
void Node::setPrevious(Node* previousNode){
	previous = previousNode;
}

//leave the garage and delete this user from the userlist
void Node::Leave()
{
	car->exit(m_floorNum, m_slotNum);
	previous->setNext(next);
}

//add user to the end of the userlist
void LinkedList::add(Node* parker) {

	//check if the user if already in the list
    if(head->getNext() == NULL) {
        head->setNext(parker);
		parker->setPrevious(head); 
		tail = parker;
    }
    else {
		tail->setNext(parker);
		parker->setPrevious(tail);
		tail = parker;
    }
}

//find the node matches the ufid passed in
Node* LinkedList::find(int ufid)
{
	Node* traverse = head->getNext();
	bool success = false;
	while (traverse != NULL)
	{
		if (traverse->getUFID() == ufid)
			return traverse;
		else
			traverse = traverse->getNext();
	}
	return NULL;
}

//remove the user from the userlist if the list is not empty
void LinkedList::removeUser(int ufid){
	Node* temp = find(ufid);
	if (temp == NULL)
		printf("\nThe user with UFID %d was not found on the system. \n", ufid);
	else
		temp->Leave();
}
