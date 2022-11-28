// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
#pragma once

#include "XamlBuilder.h"
#include "AdaptiveCardRenderer.g.h"
#include "AdaptiveHostConfig.h"

namespace winrt::AdaptiveCards::Rendering::Xaml_Rendering::implementation
{
    // This class is effectively a singleton, and stays around between subsequent renders.
    struct AdaptiveCardRenderer : AdaptiveCardRendererT<AdaptiveCardRenderer>
    {
    private:
        winrt::xaml::ResourceDictionary m_overrideDictionary;
        winrt::render_xaml::AdaptiveHostConfig m_hostConfig;
        winrt::render_xaml::AdaptiveFeatureRegistration m_featureRegistration;
        winrt::com_ptr<::AdaptiveCards::Rendering::Xaml_Rendering::XamlBuilder> m_xamlBuilder;
        bool m_explicitDimensions = false;
        uint32_t m_desiredWidth = 0;
        uint32_t m_desiredHeight = 0;

        winrt::com_ptr<implementation::AdaptiveHostConfig> GetHostConfig()
        {
            return peek_innards<implementation::AdaptiveHostConfig>(m_hostConfig);
        }

    public:
        AdaptiveCardRenderer();

        // IAdaptiveCardRenderer
        void OverrideStyles(winrt::xaml::ResourceDictionary const& overrideDictionary)
        {
            m_overrideDictionary = overrideDictionary;
            SetMergedDictionary();
        }

        winrt::xaml::ResourceDictionary OverrideStyles() { return m_overrideDictionary; }

        void HostConfig(winrt::render_xaml::AdaptiveHostConfig const& hostConfig)
        {
            m_hostConfig = hostConfig;
            UpdateActionSentimentResourceDictionary();
        }

        winrt::render_xaml::AdaptiveHostConfig HostConfig() { return m_hostConfig; }

        void FeatureRegistration(winrt::render_xaml::AdaptiveFeatureRegistration const& featureRegistration)
        {
            m_featureRegistration = featureRegistration;
        }

        winrt::render_xaml::AdaptiveFeatureRegistration FeatureRegistration();

        void SetFixedDimensions(uint32_t desiredWidth, uint32_t desiredHeight);
        void ResetFixedDimensions() { m_explicitDimensions = false; };

        void OverflowMaxActions(bool overflowMaxActions);
        bool OverflowMaxActions();

        hstring OverflowButtonText() { return GetHostConfig()->OverflowButtonText; }
        void OverflowButtonText(hstring const& overflowButtonText)
        {
            return GetHostConfig()->OverflowButtonText = overflowButtonText;
        }

        hstring OverflowButtonAccessibilityText() { return GetHostConfig()->OverflowButtonAccessibilityText; }
        void OverflowButtonAccessibilityText(hstring const& text)
        {
            return GetHostConfig()->OverflowButtonAccessibilityText = text;
        }

        winrt::render_xaml::RenderedAdaptiveCard RenderAdaptiveCard(winrt::AdaptiveCard const& adaptiveCard);
        winrt::render_xaml::RenderedAdaptiveCard RenderAdaptiveCardFromJsonString(hstring const& adaptiveJson);
        winrt::render_xaml::RenderedAdaptiveCard RenderAdaptiveCardFromJson(winrt::JsonObject const& adaptiveJson);

        winrt::render_xaml::AdaptiveElementRendererRegistration ElementRenderers() { return *m_elementRendererRegistration; }
        winrt::render_xaml::AdaptiveActionRendererRegistration ActionRenderers() { return *m_actionRendererRegistration; }

        winrt::xaml::ResourceDictionary GetMergedDictionary() { return m_mergedResourceDictionary; }
        bool GetFixedDimensions(_Out_ uint32_t* width, _Out_ uint32_t* height);
        winrt::com_ptr<::AdaptiveCards::Rendering::Xaml_Rendering::XamlBuilder> GetXamlBuilder() { return m_xamlBuilder; }
        winrt::xaml::ResourceDictionary GetActionSentimentResourceDictionary()
        {
            return m_actionSentimentResourceDictionary;
        }

        auto ResourceResolvers() { return m_resourceResolvers; }

    private:
        void InitializeDefaultResourceDictionary();
        void UpdateActionSentimentResourceDictionary();
        void TryInsertResourceToSentimentResourceDictionary(std::wstring_view const& resourceName, winrt::IInspectable const& value);
        void SetMergedDictionary();

        winrt::xaml::ResourceDictionary m_defaultResourceDictionary;
        winrt::xaml::ResourceDictionary m_mergedResourceDictionary;
        winrt::xaml::ResourceDictionary m_actionSentimentResourceDictionary;
        winrt::render_xaml::AdaptiveCardResourceResolvers m_resourceResolvers;
        winrt::com_ptr<implementation::AdaptiveElementRendererRegistration> m_elementRendererRegistration;
        winrt::com_ptr<implementation::AdaptiveActionRendererRegistration> m_actionRendererRegistration;
    };
}
namespace winrt::AdaptiveCards::Rendering::Xaml_Rendering::factory_implementation
{
    struct AdaptiveCardRenderer : AdaptiveCardRendererT<AdaptiveCardRenderer, implementation::AdaptiveCardRenderer>
    {
    };
}
