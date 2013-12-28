#include "sauce_renderer.h"
#include "../common/implementations/json_decoder.h"

#include <sauce/sauce.h>

class SauceJsonDecoder : public IGetKeyValue {
public:
	 SauceJsonDecoder(std::shared_ptr< IModel > d):
		pimpl( NewJsonDecoder(d) ) {
	 }

public:
	virtual size_t Count()  {
		return pimpl ? pimpl->Count() : 0;
	}

	virtual std::string GetKey(size_t pos) {
		return pimpl ? pimpl->GetKey(pos) : "";
	}

	virtual std::string GetValue(std::string const& key) {
		return pimpl ? pimpl->GetValue(key) : "";
	}

private:
 	std::shared_ptr< IGetKeyValue > pimpl;
};


void decoder_module(sauce::Binder& b) {
 b.bind<IGetKeyValue>().to<SauceJsonDecoder(IModel&)>();
}
