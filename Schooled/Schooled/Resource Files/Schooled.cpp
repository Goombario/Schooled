#include "../Header Files/Schooled.h"

///////////////////////////////////////////////////////////////////////////////
// Operators

bool operator ==(COORD a, COORD b)
{
	return (a.X == b.X && a.Y == b.Y);
}

COORD operator +(COORD a, COORD b)
{
	return{ a.X + b.X, a.Y + b.Y };
}