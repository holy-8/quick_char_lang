#pragma once

#include <stdlib.h>
#include <stdbool.h>

#include "DataTypes.h"
#include "ShortStack.h"
#include "ProcedureStack.h"

VMState*
VMState_new(void);

void
VMState_free(VMState* this);
