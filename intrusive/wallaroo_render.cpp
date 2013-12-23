#include "../common/interfaces/imodel.h"
#include "../common/interfaces/igetkeyvalue.h"
#include "../common/interfaces/irender.h"

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

	 	static const char* DELIMITER = ",";

		std::stringstream ss;
		for (size_t pos = 0; pos < model->Count(); pos++) {
			if ( pos!= 0 )
				ss << DELIMITER;
			ss << model->GetKey(pos);
		}

		return ss.str();
	}

private:
 	wallaroo::Plug< IGetKeyValue > model;
};

WALLAROO_REGISTER( WallarooKeyRenderer );
