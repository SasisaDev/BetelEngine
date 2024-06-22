#pragma once

#include "../Interface/IPlatform.h"

class LinuxPlatform : public IPlatform
{
public:

	virtual void DebugPrint(const char* string) const override;
};