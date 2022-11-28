// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
#pragma once

#include "AdaptiveFontTypeDefinition.g.h"

namespace winrt::AdaptiveCards::Rendering::Xaml_Rendering::implementation
{
    struct AdaptiveFontTypeDefinition : AdaptiveFontTypeDefinitionT<AdaptiveFontTypeDefinition>
    {
        AdaptiveFontTypeDefinition(::AdaptiveCards::FontTypeDefinition const& fontTypeDefinition = {});

        property<hstring> FontFamily;
        property<winrt::render_xaml::AdaptiveFontWeightsConfig> FontWeights;
        property<winrt::render_xaml::AdaptiveFontSizesConfig> FontSizes;
    };
}
namespace winrt::AdaptiveCards::Rendering::Xaml_Rendering::factory_implementation
{
    struct AdaptiveFontTypeDefinition : AdaptiveFontTypeDefinitionT<AdaptiveFontTypeDefinition, implementation::AdaptiveFontTypeDefinition>
    {
    };
}
