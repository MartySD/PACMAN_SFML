#include "stdafx.h"
#include "PacmanPlayer.h"


void Pacman::setPosition(Vector2f newPosition) {
	m_prevPosition = m_position;
	m_position = newPosition;
}

Vector2f Pacman::getPosition() {
	return m_position;
}

Vector2f Pacman::getPrevPosition() {
	return m_prevPosition;
}

float Pacman::getSpeed() {
	return m_speed;
}

int Pacman::getPacmanSize() {
	return m_pacmanSize;
}

float Pacman::getRotation() {
	return m_rotation;
}

void Pacman::setRotation(float newRotation) {
	m_rotation = newRotation;
}

void Pacman::updateSheetSelection(float dtAsSeconds) {
	m_lastSwap += dtAsSeconds;

	if (m_lastSwap > m_sheetSwapTime) {
		m_lastSwap = 0;
		m_sheetSelection += 1;
	}
	m_sheetSelection = m_sheetSelection % 6;		// 6 is number of animated frames of Pacman
}

IntRect Pacman::getSheetSelection() {
	IntRect sheetSelection;

	sheetSelection.height = 30;
	sheetSelection.width = 30;
	sheetSelection.top = 0;
	sheetSelection.left = m_sheetSelection * 30;

	return sheetSelection;
}