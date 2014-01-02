#include "json_decoder.h"

#include "../common/interfaces/igetkeyvalue.h"
#include "../common/interfaces/imodel.h"

#include <picojson.h>
#include <vector>

class JsonDecoder : public IGetKeyValue {
public:
	 JsonDecoder(std::shared_ptr< IModel > d):
		data_source( d )
	 {
	 }

public:
	virtual size_t Count()  {
		parse_if_necessary();

		return keys.size();
	}

	virtual std::string GetKey(size_t pos) {
		parse_if_necessary();

		if (pos <= keys.size())
			return keys[pos];

		return "";
	}

	virtual std::string GetValue(std::string const& key) {
		parse_if_necessary();

		if (obj.find(key) !=  obj.end())
			return obj[key].to_str();
		else
			return "";
	}

private:

	void clear() {
		obj.clear();
		keys.clear();
	}

	void parse_if_necessary() {
		
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
 	std::shared_ptr< IModel > data_source;
 	
 	// cached values
 	std::string last_data;
 	picojson::object obj;
 	std::vector<std::string> keys;
};

std::shared_ptr< IGetKeyValue > NewJsonDecoder(std::shared_ptr< IModel > data_source)
{
	return std::make_shared< JsonDecoder >(data_source);
}
