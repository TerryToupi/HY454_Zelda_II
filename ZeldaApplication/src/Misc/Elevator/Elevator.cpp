#include "Elevator.h"

Elevator::Elevator()
{

}

void Elevator::SetSecondary(Sprite _secondary)
{
	m_seconday = _secondary;
}

Sprite Elevator::GetSecondary()
{
	return m_secondary;
}