#pragma once

#include "../common/interfaces/irender.h"
#include <wallaroo/device.h>

struct IWallarooRender : public wallaroo::Device , public IRender { };
