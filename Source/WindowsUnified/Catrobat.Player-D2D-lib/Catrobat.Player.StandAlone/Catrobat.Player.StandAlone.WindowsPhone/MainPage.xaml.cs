﻿using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using Windows.Phone.UI.Input;




// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

namespace Catrobat.Player.StandAlone
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();

            this.NavigationCacheMode = NavigationCacheMode.Required;
        }

        //private readonly Catrobat_Player::Catrobat_PlayerMain _testMain = new Catrobat_PlayerMain;

        /// <summary>
        /// Invoked when this page is about to be displayed in a Frame.
        /// </summary>
        /// <param name="e">Event data that describes how this page was reached.
        /// This parameter is typically used to configure the page.</param>

        //var playerObject = null;
        private readonly Catrobat_Player.Catrobat_PlayerAdapter playerObject = new Catrobat_Player.Catrobat_PlayerAdapter();
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            // register hardware back button event
            HardwareButtons.BackPressed += OnHardwareBackButtonPressed;

            playerObject.InitPlayer(swapChainPanel, PlayerAppBar);
        }

        private void OnRestartButtonClicked(object sender, RoutedEventArgs e)
        {
            playerObject.RestartButtonClicked(sender, e);            
        }

        private void OnPlayButtonClicked(object sender, RoutedEventArgs e)
        {
            playerObject.PlayButtonClicked(sender, e);
        }
        private void OnThumbnailButtonClicked(object sender, RoutedEventArgs e)
        {
            playerObject.ThumbnailButtonClicked(sender, e);
        }
        private void OnEnableAxisButtonClicked(object sender, RoutedEventArgs e)
        {
            playerObject.EnableAxisButtonClicked(sender, e);
        }
        private void OnScreenshotButtonClicked(object sender, RoutedEventArgs e)
        {
            playerObject.ScreenshotButtonClicked(sender, e);
        }


        private void OnHardwareBackButtonPressed(object sender, BackPressedEventArgs args)
        {
            args.Handled = true;
            if (playerObject.HardwareBackButtonPressed() == true)
            {
                Application.Current.Exit();
            }
        }
    }
}
