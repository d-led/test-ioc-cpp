#include "sauce_renderer.h"
#include "../common/implementations/key_renderer.h"

#include <sauce/sauce.h>

class SauceKeyRenderer : public IRender {
public:
	SauceKeyRenderer(std::shared_ptr< IGetKeyValue > m) :
	pimpl ( NewKeyRenderer(m) ) {
	}

public:
	virtual std::string Render() {
		if (!pimpl)
			return "";

		return pimpl->Render();
	}

private:
 	std::shared_ptr< IRender > pimpl;
};


void render_module(sauce::Binder& b) {
	b.bind<IRender>().to<SauceKeyRenderer(IGetKeyValue&)>();
}
