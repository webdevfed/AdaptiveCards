// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
#pragma once

#include "AdaptiveToggleVisibilityActionRenderer.g.h"

namespace winrt::AdaptiveCards::Rendering::Xaml_Rendering::implementation
{
    struct AdaptiveToggleVisibilityActionRenderer : AdaptiveToggleVisibilityActionRendererT<AdaptiveToggleVisibilityActionRenderer>
    {
        AdaptiveToggleVisibilityActionRenderer() = default;

        winrt::xaml::UIElement Render(winrt::IAdaptiveActionElement const& actionElement,
                                                   winrt::render_xaml::AdaptiveRenderContext const& renderContext,
                                                   winrt::render_xaml::AdaptiveRenderArgs const& renderArgs);
    };
}
namespace winrt::AdaptiveCards::Rendering::Xaml_Rendering::factory_implementation
{
    struct AdaptiveToggleVisibilityActionRenderer
        : AdaptiveToggleVisibilityActionRendererT<AdaptiveToggleVisibilityActionRenderer, implementation::AdaptiveToggleVisibilityActionRenderer>
    {
    };
}
