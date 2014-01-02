#include "dicpp_module.h"
#include "../common/implementations/json_decoder.h"
#include "../common/implementations/key_renderer.h"

#include <di/constructor.hpp>
#include <di/registry.hpp>


// http://stackoverflow.com/a/12605002/847349
namespace {
    template<class SharedPointer> struct Holder {
        SharedPointer p;

        Holder(const SharedPointer &p) : p(p) {}
        Holder(const Holder &other) : p(other.p) {}
        Holder(Holder &&other) : p(std::move(other.p)) {}

        void operator () (...) const {}
    };
}

template<class T> std::shared_ptr<T> to_std_ptr(const boost::shared_ptr<T> &p) {
    typedef Holder<std::shared_ptr<T>> H;
    if(H *h = boost::get_deleter<H, T>(p)) {
        return h->p;
    } else {
        return std::shared_ptr<T>(p.get(), Holder<boost::shared_ptr<T>>(p));
    }
}

template<class T> boost::shared_ptr<T> to_boost_ptr(const std::shared_ptr<T> &p){
    typedef Holder<boost::shared_ptr<T>> H;
    if(H * h = std::get_deleter<H, T>(p)) {
        return h->p;
    } else {
        return boost::shared_ptr<T>(p.get(), Holder<std::shared_ptr<T>>(p));
    }
}


class DicppJsonDecoder : public IGetKeyValue {
public:
	DI_CONSTRUCTOR(DicppJsonDecoder,(boost::shared_ptr< IModel > d));

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
 	boost::shared_ptr< IGetKeyValue > pimpl;
};

DicppJsonDecoder::DicppJsonDecoder(boost::shared_ptr< IModel > d) :
	pimpl( to_boost_ptr<IGetKeyValue>(NewJsonDecoder(to_std_ptr<IModel>(d))) ) { }

class DicppKeyRenderer : public IRender {
public:
	DI_CONSTRUCTOR(DicppKeyRenderer,(boost::shared_ptr< IGetKeyValue > m));

public:
	virtual std::string Render() {
		if (!pimpl)
			return "";

		return pimpl->Render();
	}

private:
 	boost::shared_ptr< IRender > pimpl;
};

DicppKeyRenderer::DicppKeyRenderer(boost::shared_ptr< IGetKeyValue > m) :
	pimpl ( to_boost_ptr<IRender>(NewKeyRenderer(to_std_ptr<IGetKeyValue>(m))) ) { }

void dicpp_module( di::registry& r ) {
	r.add( r.type<IGetKeyValue>().implementation<DicppJsonDecoder>() );
	r.add( r.type<IRender>().implementation<DicppKeyRenderer>() );
}
