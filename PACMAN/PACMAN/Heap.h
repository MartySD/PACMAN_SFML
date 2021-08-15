#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class HeapElement {					// contains Vector2i as cords to map and pointer to next HeapElement
private:
	Vector2i m_squareCords;
	HeapElement* m_next;

public:
	
	HeapElement* getNext();
	void setNext(HeapElement* newElement);
	Vector2i getCords();

	HeapElement(Vector2i squareCords);
};

class Heap {						// contain pointer to first and last element in Heap
private:
	HeapElement* first;
	HeapElement* last;

public:
	void add(Vector2i squareNo);	// add HeapElement (Vector2i) to Heap
	Vector2i get();					// extract first element from Heap

	Heap::Heap();
};