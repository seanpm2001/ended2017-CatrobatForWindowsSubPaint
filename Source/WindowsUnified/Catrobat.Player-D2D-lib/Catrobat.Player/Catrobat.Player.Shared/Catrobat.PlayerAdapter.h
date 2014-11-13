#pragma once
#include "Catrobat.PlayerMain.h"
#include "Common\DeviceResources.h"
#include "Content\Basic2DRenderer.h"

namespace Catrobat_Player
{
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class Catrobat_PlayerAdapter sealed
    {
    public:
        Catrobat_PlayerAdapter();
        virtual ~Catrobat_PlayerAdapter();

        void InitPlayer(Windows::UI::Xaml::Controls::SwapChainPanel^ swapChainPanel,
            Windows::UI::Xaml::Controls::CommandBar^ playerAppBar);

        // Independent input handling functions.
        void PointerPressed(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
        bool HardwareBackButtonPressed();

        // Bottom CommandBar handlers
        void RestartButtonClicked(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ args);
        void PlayButtonClicked(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ args);
        void ThumbnailButtonClicked(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ args);
        void EnableAxisButtonClicked(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ args);
        void ScreenshotButtonClicked(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ args);


    private:
        // Other event handlers.
        void OnCompositionScaleChanged(Windows::UI::Xaml::Controls::SwapChainPanel^ sender, Object^ args);
        //void OnSwapChainPanelSizeChanged(Object^ sender, SizeChangedEventArgs^ e);

        // Independent input handling functions.
        void OnPointerPressed(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);

        // Track our independent input on a background worker thread.
        Windows::Foundation::IAsyncAction^ m_inputLoopWorker;
        Windows::UI::Core::CoreIndependentInputSource^ m_coreInput;

        // Resources used to render the DirectX content in the XAML page background.
        std::shared_ptr<DX::DeviceResources> m_deviceResources;
        std::unique_ptr<Catrobat_PlayerMain> m_main;
        bool m_windowVisible;
    };
};