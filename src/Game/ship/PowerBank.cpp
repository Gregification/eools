#include "PowerBank.hpp"

using namespace Power;

//overkill but this class is still in dev
enum class DIFF : MsgDiffType {
	cap = DEFAULT_MsgDiff_EVERYTHING + 1,
	val
};

bool Power::PowerBank::isFull() const
{
	return !(value < capacity);
}

const Power::PowerUnit& Power::PowerBank::getCapacity() const
{
	return capacity;
}

const Power::PowerUnit& Power::PowerBank::getValue() const
{
	return value;
}

void Power::PowerBank::setCapacity(PowerUnit a)
{
	capacity = a;
	addPackageTarget(DIFF::cap);
}

void Power::PowerBank::setValue(PowerUnit a)
{
	value = a;
	addPackageTarget(DIFF::val);
}

void Power::PowerBank::packMessage(Message& msg, MsgDiffType t)
{
	switch (t)
	{
	case static_cast<MsgDiffType>(DIFF::cap):
		msg << capacity;	break;

	case static_cast<MsgDiffType>(DIFF::val):
		msg << value;		break;

	case DEFAULT_MsgDiff_EVERYTHING:
	default:
		msg << capacity;
		msg << value;
		break;
	}
}

void Power::PowerBank::unpackMessage(Message& msg, MsgDiffType t)
{
	switch (t)
	{
	case static_cast<MsgDiffType>(DIFF::cap):
		msg >> capacity;	break;

	case static_cast<MsgDiffType>(DIFF::val):
		msg >> value;		break;

	case DEFAULT_MsgDiff_EVERYTHING:
	default:
		msg >> value;
		msg >> capacity;
		break;
	}
}
