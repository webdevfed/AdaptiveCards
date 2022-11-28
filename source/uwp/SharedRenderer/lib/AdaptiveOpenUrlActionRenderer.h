// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
#pragma once

#include "AdaptiveOpenUrlActionRenderer.g.h"

namespace winrt::AdaptiveCards::Rendering::Xaml_Rendering::implementation
{
    struct AdaptiveOpenUrlActionRenderer : AdaptiveOpenUrlActionRendererT<AdaptiveOpenUrlActionRenderer>
    {
        AdaptiveOpenUrlActionRenderer() = default;

        winrt::xaml::UIElement Render(winrt::IAdaptiveActionElement const& action,
                                                   winrt::render_xaml::AdaptiveRenderContext const& renderContext,
                                                   winrt::render_xaml::AdaptiveRenderArgs const& renderArgs);
    };
}
namespace winrt::AdaptiveCards::Rendering::Xaml_Rendering::factory_implementation
{
    struct AdaptiveOpenUrlActionRenderer
        : AdaptiveOpenUrlActionRendererT<AdaptiveOpenUrlActionRenderer, implementation::AdaptiveOpenUrlActionRenderer>
    {
    };
}
