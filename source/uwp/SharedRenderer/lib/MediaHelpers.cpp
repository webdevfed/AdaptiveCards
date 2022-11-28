// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
#include "pch.h"
#include "AdaptiveCardGetResourceStreamArgs.h"
#include "WholeItemsPanel.h"

const double c_playIconSize = 30;
const double c_playIconCornerRadius = 5;
const double c_playIconOpacity = .5;
const winrt::hstring supportedMimeTypes[] = {L"video/mp4", L"audio/mp4", L"audio/aac", L"audio/mpeg"};
const std::unordered_set<winrt::hstring> supportedCaptionTypes = {L"vtt", L"srt"};

namespace AdaptiveCards::Rendering::Xaml_Rendering::MediaHelpers
{
    winrt::xaml_controls::Image GetMediaPosterAsImage(winrt::render_xaml::AdaptiveRenderContext const& renderContext,
                                       winrt::render_xaml::AdaptiveRenderArgs const& renderArgs,
                                                      winrt::AdaptiveMedia const& adaptiveMedia)
    {
        auto posterString = adaptiveMedia.Poster();

        if (posterString.empty())
        {
            auto hostConfig = renderContext.HostConfig();
            auto mediaConfig = hostConfig.Media();

            posterString = mediaConfig.DefaultPoster();

            if (posterString.empty())
            {
                return nullptr;
            }
        }

        winrt::AdaptiveImage adaptiveImage{};
        adaptiveImage.Url(posterString);

        auto altText = adaptiveMedia.AltText();
        adaptiveImage.AltText(altText);

        auto elementRenderers = renderContext.ElementRenderers();
        auto imageRenderer = elementRenderers.Get(L"Image");

        if (const auto posterUiElement = imageRenderer.Render(adaptiveImage, renderContext, renderArgs))
        {
            return posterUiElement.as<winrt::xaml_controls::Image>();
        }
        // Not logging a warning because if we get nullptr from imageRendere - it will log warning for us.
        return nullptr;
    }

    void AddDefaultPlayIcon(winrt::xaml_controls::Panel const& posterPanel,
                            winrt::render_xaml::AdaptiveHostConfig const& hostConfig,
                            winrt::render_xaml::AdaptiveRenderArgs const& renderArgs)
    {
        // Create a rectangle
        winrt::xaml_shapes::Rectangle rectangle{};

        // Set the size
        rectangle.Height(c_playIconSize);
        rectangle.Width(c_playIconSize);

        // Round the corners
        rectangle.RadiusX(c_playIconCornerRadius);
        rectangle.RadiusY(c_playIconCornerRadius);

        // Set it's fill and opacity
        winrt::Windows::UI::Color whiteBrushColor{0xFF, 0xFF, 0xFF, 0xFF};
        rectangle.Fill(winrt::xaml_media::SolidColorBrush{whiteBrushColor});
        rectangle.Opacity(c_playIconOpacity);

        // Outline it in the Dark color
        auto containerStyle = renderArgs.ContainerStyle();

        auto darkBrushColor = GetColorFromAdaptiveColor(hostConfig, winrt::ForegroundColor::Dark, containerStyle, false, false);
        winrt::xaml_media::SolidColorBrush darkBrush{darkBrushColor};
        rectangle.Stroke(darkBrush);

        // Create a play symbol icon
        winrt::xaml_controls::SymbolIcon playIcon{winrt::xaml_controls::Symbol::Play};

        playIcon.Foreground(darkBrush);

        XamlHelpers::AppendXamlElementToPanel(rectangle, posterPanel);
        winrt::xaml_controls::RelativePanel::SetAlignVerticalCenterWithPanel(rectangle, true);
        winrt::xaml_controls::RelativePanel::SetAlignHorizontalCenterWithPanel(rectangle, true);

        XamlHelpers::AppendXamlElementToPanel(playIcon, posterPanel);
        winrt::xaml_controls::RelativePanel::SetAlignHorizontalCenterWithPanel(playIcon, true);
        winrt::xaml_controls::RelativePanel::SetAlignVerticalCenterWithPanel(playIcon, true);
    }

    void AddCustomPlayIcon(winrt::xaml_controls::Panel const& posterPanel,
                           winrt::hstring const& playIconString,
                           winrt::render_xaml::AdaptiveRenderContext const& renderContext,
                           winrt::render_xaml::AdaptiveRenderArgs const& renderArgs)
    {
        // Render the custom play icon using the image renderer
        winrt::AdaptiveImage playIconAdaptiveImage{};
        playIconAdaptiveImage.Url(playIconString);

        auto imageRenderer = renderContext.ElementRenderers().Get(L"Image");

        auto playIconUIElement = imageRenderer.Render(playIconAdaptiveImage, renderContext, renderArgs);

        if (const auto playIconAsFrameworkElement = playIconUIElement.try_as<winrt::xaml::FrameworkElement>())
        {
            playIconAsFrameworkElement.Height(c_playIconSize);

            // Add it to the panel and center it
            XamlHelpers::AppendXamlElementToPanel(playIconUIElement, posterPanel);
            winrt::xaml_controls::RelativePanel::SetAlignHorizontalCenterWithPanel(playIconUIElement, true);
            winrt::xaml_controls::RelativePanel::SetAlignVerticalCenterWithPanel(playIconUIElement, true);
        }
    }

    void AddPlayIcon(winrt::xaml_controls::Panel const& posterPanel,
                     winrt::render_xaml::AdaptiveRenderContext const& renderContext,
                     winrt::render_xaml::AdaptiveRenderArgs const& renderArgs)
    {
        auto hostConfig = renderContext.HostConfig();

        auto mediaConfig = hostConfig.Media();

        winrt::hstring customPlayIconString = mediaConfig.PlayButton();

        if (customPlayIconString.empty())
        {
            AddDefaultPlayIcon(posterPanel, hostConfig, renderArgs);
        }
        else
        {
            AddCustomPlayIcon(posterPanel, customPlayIconString, renderContext, renderArgs);
        }
    }

    winrt::xaml::UIElement CreatePosterContainerWithPlayButton(winrt::xaml_controls::Image const& posterImage,
                                                         winrt::render_xaml::AdaptiveRenderContext const& renderContext,
                                                         winrt::render_xaml::AdaptiveRenderArgs const& renderArgs)
    {
        winrt::xaml_controls::RelativePanel posterRelativePanel{};

        if (posterImage)
        {
            XamlHelpers::AppendXamlElementToPanel(posterImage, posterRelativePanel);
        }
        AddPlayIcon(posterRelativePanel, renderContext, renderArgs);

        return posterRelativePanel;
    }

    std::tuple<winrt::Uri, winrt::hstring> GetMediaSource(winrt::render_xaml::AdaptiveHostConfig const& hostConfig,
                                                          winrt::AdaptiveMedia const& adaptiveMedia)
    {
        winrt::Uri mediaSourceUrl{nullptr};
        winrt::hstring mimeType{};

        auto sources = adaptiveMedia.Sources();

        winrt::AdaptiveMediaSource selectedSource{nullptr};

        for (auto source : sources)
        {
            winrt::hstring currentMimeType = source.MimeType();

            for (uint32_t i = 0; i < std::size(supportedMimeTypes); i++)
            {
                if (currentMimeType == supportedMimeTypes[i])
                {
                    selectedSource = source;
                    break;
                }
            }
        }
        if (selectedSource)
        {
            mediaSourceUrl = GetUrlFromString(hostConfig, selectedSource.Url());
            mimeType = selectedSource.MimeType();
        }

        return {mediaSourceUrl, mimeType};
    }

    void SetMediaSourceHelper(winrt::xaml_controls::MediaElement const& mediaElement,
                              winrt::AdaptiveMedia const& adaptiveMedia,
                              winrt::render_xaml::AdaptiveRenderContext const& renderContext,
                              winrt::MediaSource const& mediaSrc)
    {
        if (adaptiveMedia.CaptionSources().Size() > 0)
        {
            for (const auto captionSource : adaptiveMedia.CaptionSources())
            {
                if (const auto search = supportedCaptionTypes.find(captionSource.MimeType());
                    search != supportedCaptionTypes.end())
                {
                    const auto timedTextURL = GetUrlFromString(renderContext.HostConfig(), captionSource.Url());

                    winrt::render_xaml::IAdaptiveCardResourceResolver resourceResolver{nullptr};
                    if (const auto resourceResolvers = renderContext.ResourceResolvers())
                    {
                        resourceResolver = resourceResolvers.Get(timedTextURL.SchemeName());
                    }

                    const auto timedTextSrcResolvedHelper =
                        [label = captionSource.Label()](winrt::TimedTextSource const& /*sender*/,
                                                        winrt::TimedTextSourceResolveResultEventArgs const& args)
                    {
                        if (!args.Error())
                        {
                            args.Tracks().GetAt(0).Label(label);
                        }
                    };
                    if (!resourceResolver)
                    {
                        const auto timedTextSrc = winrt::TimedTextSource::CreateFromUri(timedTextURL);
                        timedTextSrc.Resolved(timedTextSrcResolvedHelper);
                        mediaSrc.ExternalTimedTextSources().Append(timedTextSrc);
                    }
                    else
                    {
                        auto args = winrt::make<winrt::implementation::AdaptiveCardGetResourceStreamArgs>(timedTextURL);
                        const auto randomAccessStream = resourceResolver.GetResourceStreamAsync(args);
                        auto timedTextSrc = winrt::TimedTextSource::CreateFromStream(randomAccessStream.get());
                        timedTextSrc.Resolved(timedTextSrcResolvedHelper);
                        mediaSrc.ExternalTimedTextSources().Append(timedTextSrc);
                    }
                }
            }
        }
        winrt::MediaPlaybackItem playbackItem{mediaSrc};
        playbackItem.TimedMetadataTracksChanged(
            [playbackItem](winrt::IInspectable const& /*sender*/, winrt::IInspectable const& /*args*/)
            {
                playbackItem.TimedMetadataTracks().SetPresentationMode(0, winrt::TimedMetadataTrackPresentationMode::PlatformPresented);
            });
        mediaElement.SetPlaybackSource(playbackItem);
    }

    void HandleMediaResourceResolverCompleted(winrt::IAsyncOperation<winrt::IRandomAccessStream> const& operation,
                                              winrt::AsyncStatus status,
                                              winrt::xaml_controls::MediaElement const& mediaElement,
                                              winrt::hstring const& mimeType,
                                              winrt::AdaptiveMedia const& adaptiveMedia,
                                              winrt::render_xaml::AdaptiveRenderContext const& renderContext)
    {
        if (status == winrt::AsyncStatus::Completed)
        {
            // Get the random access stream
            if (const auto randomAccessStream = operation.GetResults())
            {
                auto mediaSrc = winrt::MediaSource::CreateFromStream(randomAccessStream, mimeType);
                SetMediaSourceHelper(mediaElement, adaptiveMedia, renderContext, mediaSrc);
            }
        }
    }

    void HandleMediaClick(winrt::render_xaml::AdaptiveRenderContext const& renderContext,
                          winrt::AdaptiveMedia const& adaptiveMedia,
                          winrt::xaml_controls::MediaElement const& mediaElement,
                          winrt::xaml::UIElement const& posterContainer,
                          winrt::Uri const& mediaSourceUrl,
                          winrt::hstring const& mimeType,
                          winrt::render_xaml::AdaptiveMediaEventInvoker const& mediaInvoker)
    {
        if (mediaElement)
        {
            posterContainer.Visibility(winrt::xaml::Visibility::Collapsed);
            mediaElement.Visibility(winrt::xaml::Visibility::Visible);

            winrt::render_xaml::IAdaptiveCardResourceResolver resourceResolver{nullptr};
            if (const auto resourceResolvers = renderContext.ResourceResolvers())
            {
                resourceResolver = resourceResolvers.Get(mediaSourceUrl.SchemeName());
            }

            if (resourceResolver == nullptr)
            {
                auto mediaSrc = winrt::MediaSource::CreateFromUri(mediaSourceUrl);
                SetMediaSourceHelper(mediaElement, adaptiveMedia, renderContext, mediaSrc);
            }
            else
            {
                auto args = winrt::make<winrt::implementation::AdaptiveCardGetResourceStreamArgs>(mediaSourceUrl);

                auto getResourceStreamOperation = resourceResolver.GetResourceStreamAsync(args);

                getResourceStreamOperation.Completed(
                    [mediaElement, mimeType, adaptiveMedia, renderContext](winrt::IAsyncOperation<winrt::IRandomAccessStream> operation,
                                                                           winrt::AsyncStatus status) -> void
                    {
                        return HandleMediaResourceResolverCompleted(operation, status, mediaElement, mimeType, adaptiveMedia, renderContext);
                    });
            }

            mediaElement.MediaOpened(
                [](winrt::IInspectable const& sender, winrt::xaml::RoutedEventArgs const& /*args*/) -> void
                {
                    if (const auto mediaElement = sender.try_as<winrt::xaml_controls::MediaElement>())
                    {
                        mediaElement.Play();
                    }
                });
        }
        else
        {
            mediaInvoker.SendMediaClickedEvent(adaptiveMedia);
        }
    }
}
