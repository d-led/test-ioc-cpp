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

class DicppJsonDecoder : public IGetKeyValue {
public:
    DI_CONSTRUCTOR(DicppJsonDecoder,(std::shared_ptr< IModel > d));

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

DicppJsonDecoder::DicppJsonDecoder(std::shared_ptr< IModel > d) :
    pimpl( NewJsonDecoder(d) ) { }

class DicppKeyRenderer : public IRender {
public:
    DI_CONSTRUCTOR(DicppKeyRenderer,(std::shared_ptr< IGetKeyValue > m));

public:
    virtual std::string Render() {
        if (!pimpl)
            return "";

        return pimpl->Render();
    }

private:
    std::shared_ptr< IRender > pimpl;
};

DicppKeyRenderer::DicppKeyRenderer(std::shared_ptr< IGetKeyValue > m) :
    pimpl ( NewKeyRenderer(m) ) { }

void dicpp_module( di::registry& r ) {
    r.add( r.type<IGetKeyValue>().implementation<DicppJsonDecoder>() );
    r.add( r.type<IRender>().implementation<DicppKeyRenderer>() );
}
