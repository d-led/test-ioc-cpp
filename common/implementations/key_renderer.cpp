#include "key_renderer.h"

#include <sstream>

class KeyRenderer : public IRender {
public:
	KeyRenderer(std::shared_ptr< IGetKeyValue > m) :
	model (m) {
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
 	std::shared_ptr< IGetKeyValue > model;
};

std::shared_ptr< IRender > NewKeyRenderer(std::shared_ptr< IGetKeyValue > model) {
	return std::make_shared<KeyRenderer>(model);
};
