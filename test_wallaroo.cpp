#include <wallaroo/catalog.h>
#include <wallaroo/device.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "common/interfaces/irender.h"
#include "common/interfaces/imodel.h"
#include "common/interfaces/igetkeyvalue.h"

#include "intrusive/wallaroo_jsondecoder.h"

class wallaroo_hello_world : public ::testing::Test {
protected:
	virtual void SetUp() {
		catalog.Create("renderer", "WallarooKeyRenderer");
		CreateDecoder("decoder",catalog);

		renderer = catalog["renderer"];
		ASSERT_TRUE( renderer.get() );

		decoder = catalog["decoder"];
		ASSERT_TRUE( decoder.get() );
	}

protected:
	wallaroo::Catalog catalog;
	std::shared_ptr<IRender> renderer;
	std::shared_ptr<IGetKeyValue> decoder;
};


class MockKeyValue : public IGetKeyValue, public wallaroo::Device {
public:
	MOCK_CONST_METHOD0(Count, size_t());
	MOCK_CONST_METHOD1(GetKey, std::string(size_t pos));
	MOCK_CONST_METHOD1(GetValue, std::string(std::string const&));
};
WALLAROO_REGISTER( MockKeyValue );

class MockModel : public IModel, public wallaroo::Device {
public:
	MOCK_CONST_METHOD0(Get, std::string());
};
WALLAROO_REGISTER( MockModel );


using namespace wallaroo;
using ::testing::Return;
using ::testing::AtLeast;
using ::testing::Mock;
using ::testing::_;


TEST_F(wallaroo_hello_world,just_renderer) {
	ASSERT_EQ( "", renderer->Render() );

	// rewire
	catalog.Create("mock model", "MockKeyValue");
	wallaroo_within(catalog)
	{
		use("mock model").as("model").of("renderer");
	}

	std::shared_ptr<MockKeyValue> mock_model = catalog["mock model"];
	ASSERT_TRUE( mock_model.get() );

	// set expectations for 1 key
	EXPECT_CALL(*mock_model, Count())
		.Times(AtLeast(1))
		.WillRepeatedly(Return(1));
	EXPECT_CALL(*mock_model, GetKey(0))
		.Times(AtLeast(1))
		.WillRepeatedly(Return("a"));

	ASSERT_EQ( "a", renderer->Render() )<<"should show just one key";
	Mock::VerifyAndClearExpectations(mock_model.get());

	EXPECT_CALL(*mock_model, Count())
		.Times(AtLeast(1))
		.WillRepeatedly(Return(2));
	EXPECT_CALL(*mock_model, GetKey(_))
		.WillOnce(Return("a"))
		.WillOnce(Return("b"));
	
	ASSERT_EQ( "a,b", renderer->Render() )<<"should render a comma-separated key list";
}

TEST_F(wallaroo_hello_world,just_decoder) {

	ASSERT_EQ(0, decoder->Count() );
	ASSERT_EQ("", decoder->GetKey(22) );
	ASSERT_EQ("", decoder->GetValue("42") );

	catalog.Create("mock data","MockModel");

	std::shared_ptr<MockModel> mock_data = catalog["mock data"];
	ASSERT_TRUE( mock_data.get() );

	EXPECT_CALL(*mock_data, Get())
		.Times(AtLeast(1))
		.WillRepeatedly(Return(""));

	wallaroo_within(catalog)
	{
		use("mock data").as("data_source").of("decoder");
	}

	ASSERT_EQ( 0, decoder->Count() );
	Mock::VerifyAndClearExpectations(mock_data.get());
}
