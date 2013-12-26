#include "wallaroo_jsondecoder.h"

#include "../common/interfaces/igetkeyvalue.h"
#include "../common/interfaces/imodel.h"

#include <wallaroo/catalog.h>
#include <wallaroo/device.h>
#include <wallaroo/plug.h>
#include <picojson.h>
#include <vector>

class WallarooJsonDecoder : public IGetKeyValue, public wallaroo::Device {
public:
	 WallarooJsonDecoder():
		data_source( "data_source", RegistrationToken() )
	 {
	 }

public:
	virtual size_t Count() const  {
		parse_if_necessary();

		return keys.size();
	}

	virtual std::string GetKey(size_t pos) const {
		parse_if_necessary();

		if (pos <= keys.size())
			return keys[pos];

		return "";
	}

	virtual std::string GetValue(std::string const& key) const {
		parse_if_necessary();

		if (obj.find(key) !=  obj.end())
			return obj[key].to_str();
		else
			return "";
	}

private:

	void clear() const {
		obj.clear();
		keys.clear();
	}

	void parse_if_necessary() const {
		if (!data_source) {
			last_data = "";
			this->clear();
			return;
		}

		if (data_source->Get() == last_data) {
			return;
		}

		last_data = data_source->Get();
		this->clear();

		std::string err;
		picojson::value v;
		picojson::parse(v,last_data.begin(),last_data.end(),&err);
		
		if (!err.empty() || !v.is<picojson::object>()) {
			return;
		}

		obj = v.get<picojson::object>();

		for( auto& key: obj) {
			keys.push_back(key.first);
		}
	}

private:
 	wallaroo::Plug< IModel > data_source;
 	
 	// cached values
 	mutable std::string last_data;
 	mutable picojson::object obj;
 	mutable std::vector<std::string> keys;
};
WALLAROO_REGISTER( WallarooJsonDecoder );

void CreateDecoder(std::string const& name,wallaroo::Catalog& catalog)
{
	catalog.Create(name, "WallarooJsonDecoder");
}
