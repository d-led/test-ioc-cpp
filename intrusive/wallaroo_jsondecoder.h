#pragma once

#include <string>

namespace wallaroo { class Catalog; }

void CreateDecoder(std::string const& name,wallaroo::Catalog& catalog);
