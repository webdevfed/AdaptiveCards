// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
#pragma once

#include "AdaptiveActionsConfig.g.h"

namespace winrt::AdaptiveCards::Rendering::Xaml_Rendering::implementation
{
    struct AdaptiveActionsConfig : AdaptiveActionsConfigT<AdaptiveActionsConfig>
    {
        AdaptiveActionsConfig(::AdaptiveCards::ActionsConfig const& actionsConfig = {});

        property<winrt::render_xaml::AdaptiveShowCardActionConfig> ShowCard;
        property<winrt::render_xaml::ActionsOrientation> ActionsOrientation;
        property<winrt::render_xaml::ActionAlignment> ActionAlignment;
        property<uint32_t> ButtonSpacing;
        property<uint32_t> MaxActions;
        property<winrt::Spacing> Spacing;
        property<winrt::render_xaml::IconPlacement> IconPlacement;
        property<uint32_t> IconSize;
    };
}
namespace winrt::AdaptiveCards::Rendering::Xaml_Rendering::factory_implementation
{
    struct AdaptiveActionsConfig : AdaptiveActionsConfigT<AdaptiveActionsConfig, implementation::AdaptiveActionsConfig>
    {
    };
}
