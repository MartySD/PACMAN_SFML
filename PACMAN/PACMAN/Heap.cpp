#include "stdafx.h"
#include "Heap.h"

HeapElement::HeapElement(Vector2i squareNumber) {
	m_squareCords = squareNumber;
}

Vector2i HeapElement::getCords() {
	return m_squareCords;
}

HeapElement* HeapElement::getNext() {
	return m_next;
}

void HeapElement::setNext(HeapElement* newElement) {
	m_next = newElement;
}

Heap::Heap() {
	first = NULL;
	last = NULL;
}


void Heap::add(Vector2i squareCords) {
	if (first == NULL) {	// Empty Heap
		first = new HeapElement(squareCords);
		last = first;
		return;
	}
	else {
		last[0].setNext(new HeapElement(squareCords));
		last = last[0].getNext();
		return;
	}
}

Vector2i Heap::get() {
	if (first == NULL) {		// Empty Heap
		return Vector2i(-1, -1);	// invalid / end of heap
	}
	else {
		Vector2i returnCords = first[0].getCords();
		HeapElement* removeElement = first;

		first = first[0].getNext();		// move pointer to next (second) Element
		delete removeElement;
		return returnCords;
	}
}
