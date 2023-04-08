#pragma once
#include "IDescriberCl.h"

class IDescriberClPattern : public IDescriberCl {
public:
	virtual bool Input1IsSyn() = 0;
	virtual bool Input1IsAny() = 0;
};