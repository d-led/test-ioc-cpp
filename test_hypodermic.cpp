#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "common/interfaces/irender.h"
#include "common/interfaces/imodel.h"
#include "common/interfaces/igetkeyvalue.h"

 #include <Hypodermic/ContainerBuilder.h>

 using Hypodermic::ContainerBuilder;

TEST(hypodermic_hello_world,example) {
	 ContainerBuilder builder;

	// inject_renderer(builder);
	// inject_decoder(builder);

	 auto container = builder.build();

	 auto renderer = container->resolve< IRender >();

	// ASSERT_EQ( "a,b" , renderer->Render() );
}
