#pragma once

#include "../common/interfaces/irender.h"
#include "../common/interfaces/igetkeyvalue.h"

#include <memory>

std::shared_ptr< IRender > NewKeyRenderer(std::shared_ptr< IGetKeyValue >);
