#include "../common/interfaces/imodel.h"
#include "../common/interfaces/igetkeyvalue.h"
#include "../common/interfaces/irender.h"

#include "../common/implementations/key_renderer.h"

#include <wallaroo/registered.h>
#include <wallaroo/plug.h>

#include <sstream>

class WallarooKeyRenderer :  public wallaroo::Device , public IRender {
public:
	 WallarooKeyRenderer():
		model( "model", RegistrationToken() )
	 {
	 }

public:
	virtual std::string Render() {
		if (!model)
			return "";

		// delegating the task to a general implementation
	 	auto renderer = NewKeyRenderer(model);
	 	if (!renderer)
	 		return "";

	 	return renderer->Render();
	}

private:
 	wallaroo::Plug< IGetKeyValue > model;
};

WALLAROO_REGISTER( WallarooKeyRenderer );
