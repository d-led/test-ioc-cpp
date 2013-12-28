#include "intrusive/sauce_decoder.h"
#include "intrusive/sauce_renderer.h"
#include "./common/interfaces/imodel.h"
#include "./common/interfaces/irender.h"

#include <sauce/sauce.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using sauce::Modules;
using sauce::Injector;
using ::testing::AtLeast;
using ::testing::Return;

namespace {

	class SingletonScope {};

	class MockModel : public IModel {
	public:
		MOCK_METHOD0(Get, std::string());
	};

	void model_module(sauce::Binder& b) {
		b.bind<IModel>().in<SingletonScope>().to<MockModel>();
	}
}

TEST(sauce_hello_world,all_together) {

	Modules modules;

	modules.add(render_module);
	modules.add(decoder_module);
	modules.add(model_module);

	// prepare injector
	sauce::shared_ptr<Injector> injector = modules.createInjector();

	// prepare mock
	sauce::shared_ptr<MockModel> mock_model = std::static_pointer_cast<MockModel>(injector->get<IModel>());
	ASSERT_TRUE( mock_model.get() );

	ASSERT_TRUE( mock_model.get() );
	EXPECT_CALL(*mock_model, Get())
		.Times(AtLeast(1))
		.WillRepeatedly(Return("{ \"a\" : 1 , \"b\" : 2 }"));

	// renderer instance
	sauce::shared_ptr<IRender> renderer = injector->get<IRender>();
	ASSERT_TRUE( renderer.get() );

	// render
	ASSERT_EQ( "a,b", renderer->Render() );
}
