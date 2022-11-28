// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
#pragma once

#include "AdaptiveToggleInputRenderer.g.h"

namespace winrt::AdaptiveCards::Rendering::Xaml_Rendering::implementation
{
    struct AdaptiveToggleInputRenderer : AdaptiveToggleInputRendererT<AdaptiveToggleInputRenderer>
    {
        AdaptiveToggleInputRenderer() = default;

        winrt::xaml::UIElement Render(winrt::IAdaptiveCardElement const& cardElement,
                                                   winrt::render_xaml::AdaptiveRenderContext const& renderContext,
                                                   winrt::render_xaml::AdaptiveRenderArgs const& renderArgs);
    };
}
namespace winrt::AdaptiveCards::Rendering::Xaml_Rendering::factory_implementation
{
    struct AdaptiveToggleInputRenderer
        : AdaptiveToggleInputRendererT<AdaptiveToggleInputRenderer, implementation::AdaptiveToggleInputRenderer>
    {
    };
}
