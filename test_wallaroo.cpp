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
	MOCK_METHOD0(Count, size_t());
	MOCK_METHOD1(GetKey, std::string(size_t pos));
	MOCK_METHOD1(GetValue, std::string(std::string const&));
};
WALLAROO_REGISTER( MockKeyValue );

class MockModel : public IModel, public wallaroo::Device {
public:
	MOCK_METHOD0(Get, std::string());
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
	catalog.Create("mock data", "MockKeyValue");
	wallaroo_within(catalog)
	{
		use("mock data").as("model").of("renderer");
	}

	std::shared_ptr<MockKeyValue> mock_model = catalog["mock data"];
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

	catalog.Create("mock model","MockModel");

	std::shared_ptr<MockModel> mock_model = catalog["mock model"];
	ASSERT_TRUE( mock_model.get() );

	EXPECT_CALL(*mock_model, Get())
		.Times(AtLeast(1))
		.WillRepeatedly(Return("{}"));

	wallaroo_within(catalog)
	{
		use("mock model").as("data_source").of("decoder");
	}

	ASSERT_EQ( 0, decoder->Count() );
	ASSERT_EQ("", decoder->GetKey(22) );
	ASSERT_EQ("", decoder->GetValue("42") );
	Mock::VerifyAndClearExpectations(mock_model.get());

	// change to invalid data
	EXPECT_CALL(*mock_model, Get())
		.Times(AtLeast(1))
		.WillRepeatedly(Return("0000001230000abc000"));
	ASSERT_EQ( 0, decoder->Count() );
	Mock::VerifyAndClearExpectations(mock_model.get());


	// changed data
	EXPECT_CALL(*mock_model, Get())
		.Times(AtLeast(1))
		.WillRepeatedly(Return("{ \"a\" : 1 , \"b\" : 2 }"));

	ASSERT_EQ( 2, decoder->Count() );
	ASSERT_EQ("", decoder->GetKey(22) );
	ASSERT_EQ("", decoder->GetValue("42") );
	ASSERT_EQ("1", decoder->GetValue("a") );
	ASSERT_EQ("2", decoder->GetValue("b") );
}

TEST_F(wallaroo_hello_world,all_together) {
	
	// model
	catalog.Create("mock model","MockModel");
	std::shared_ptr<MockModel> mock_model = catalog["mock model"];
	ASSERT_TRUE( mock_model.get() );
	EXPECT_CALL(*mock_model, Get())
		.Times(AtLeast(1))
		.WillRepeatedly(Return("{ \"a\" : 1 , \"b\" : 2 }"));

	// configuration
	wallaroo_within(catalog)
	{
		use("decoder").as("model").of("renderer");
		use("mock model").as("data_source").of("decoder");
	}

	// render
	ASSERT_EQ( "a,b", renderer->Render() );
}
