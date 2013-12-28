#include "intrusive/sauce_decoder.h"
#include "intrusive/sauce_renderer.h"
#include "./common/interfaces/imodel.h"
#include "./common/interfaces/irender.h"

#include <sauce/sauce.h>

#include <gtest/gtest.h>

using sauce::Modules;
using sauce::Injector;

namespace {

	class SingletonScope {};

	class MockModel : public IModel {
	public:
		virtual std::string Get() {
			return "{ \"a\" : 1 , \"b\" : 2 }";
		}
	};

	void model_module(sauce::Binder& b) {
		b.bind<IModel>().to<MockModel>();
	}
}

TEST(sauce_hello_world,all_together) {

	sauce::shared_ptr<Injector> injector = Modules()
		.add(render_module)
		.add(decoder_module)
		.add(model_module)
		.createInjector()
	;

	injector->enter<SingletonScope>();

	// renderer instance
	sauce::shared_ptr<IRender> renderer = injector->get<IRender>();
	ASSERT_TRUE( renderer.get() );

	// render
	ASSERT_EQ( "a,b", renderer->Render() );
}
