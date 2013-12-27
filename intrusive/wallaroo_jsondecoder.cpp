#include "wallaroo_jsondecoder.h"

#include "../common/interfaces/igetkeyvalue.h"
#include "../common/interfaces/imodel.h"

#include "../common/implementations/json_decoder.h"

#include <wallaroo/catalog.h>
#include <wallaroo/device.h>
#include <wallaroo/plug.h>
#include <iostream>

class WallarooJsonDecoder : public IGetKeyValue, public wallaroo::Device {
public:
	 WallarooJsonDecoder():
		data_source( "data_source", RegistrationToken() )
	 {
	 }

public:
	virtual size_t Count() {
		reset_if_necessary();
		return last_decoder ? last_decoder->Count() : 0U;
	}

	virtual std::string GetKey(size_t pos) {
		reset_if_necessary();

		if (pos <= this->Count() && last_decoder)
			return last_decoder->GetKey(pos);

		return "";
	}

	virtual std::string GetValue(std::string const& key) {
		reset_if_necessary();

		return last_decoder ? last_decoder->GetValue(key) : "";		
	}

private:

	void clear() {
		last_model.reset();
		last_decoder.reset();
	}

	void reset_if_necessary() {
		if (!data_source) {
			this->clear();
			return;
		}

		if (last_model != std::shared_ptr< IModel >(data_source)) {
			last_model = std::shared_ptr< IModel >(data_source);
			last_decoder = NewJsonDecoder( last_model );
		}
	}

private:
 	wallaroo::Plug< IModel > data_source;
 	
 	// cached values
 	std::shared_ptr< IModel > last_model;
 	std::shared_ptr< IGetKeyValue > last_decoder;
};
WALLAROO_REGISTER( WallarooJsonDecoder );

void CreateDecoder(std::string const& name,wallaroo::Catalog& catalog)
{
	catalog.Create(name, "WallarooJsonDecoder");
}
