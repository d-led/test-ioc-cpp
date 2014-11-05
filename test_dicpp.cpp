#include "intrusive/dicpp_module.h"
#include "./common/interfaces/imodel.h"
#include "./common/interfaces/irender.h"
#include "./common/interfaces/igetkeyvalue.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <di/injector.hpp>
#include <di/scopes/singleton.hpp>

using ::testing::Return;
using ::testing::AtLeast;

namespace {
    class MockModel : public IModel {
    public:
        MOCK_METHOD0(Get, std::string());
    };

    void mock_module( di::registry& r ) {
        r.add( r
            .type<IModel>()
            .implementation<MockModel>()
            .in_scope<di::scopes::singleton>() )
        ;
    }
}

TEST(dicpp_hello_world,example) {
    di::injector inj;
    inj.install( dicpp_module );
    inj.install( mock_module );

    auto mock_model = inj.construct_ptr< IModel >();
    ASSERT_TRUE( !!mock_model )<<"the singleton";

    auto mock_model_2 = inj.construct_ptr< IModel >();
    ASSERT_EQ( mock_model.get() , mock_model_2.get() );

    MockModel* mock_model_ptr = dynamic_cast< MockModel* >(mock_model.get());
    ASSERT_TRUE( !!mock_model_ptr );

    EXPECT_CALL(*mock_model_ptr, Get())
        .Times(AtLeast(1))
        .WillRepeatedly(Return("{ \"a\" : 1 , \"b\" : 2 }"));
    
    auto renderer = inj.construct_ptr< IRender >();
    ASSERT_TRUE( !!renderer );

    ASSERT_EQ( "a,b", renderer->Render() );
}
