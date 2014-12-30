#pragma once

#include "../../common/interfaces/imodel.h"
#include "../../common/interfaces/igetkeyvalue.h"
#include <memory>

std::shared_ptr< IGetKeyValue > NewJsonDecoder(std::shared_ptr< IModel >);
