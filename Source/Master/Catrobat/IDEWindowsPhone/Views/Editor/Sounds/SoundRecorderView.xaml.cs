﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Catrobat.IDECommon.Resources;
using Catrobat.IDECommon.Resources.Editor;
using Catrobat.IDEWindowsPhone.Controls.Buttons;
using Catrobat.IDEWindowsPhone.ViewModel;
using IDEWindowsPhone;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using Microsoft.Practices.ServiceLocation;

namespace Catrobat.IDEWindowsPhone.Views.Editor.Sounds
{
  public partial class SoundRecorderView : PhoneApplicationPage
  {
    private readonly SoundRecorderViewModel _soundRecorderViewModel = ServiceLocator.Current.GetInstance<SoundRecorderViewModel>();
    private ApplicationBarIconButton _buttonSave;

    public SoundRecorderView()
    {
      InitializeComponent();
      BuildApplicationBar();
      ((LocalizedStrings)Application.Current.Resources["LocalizedStrings"]).PropertyChanged += LanguageChanged;
      _soundRecorderViewModel.PropertyChanged += SoundRecorderViewModel_OnPropertyChanged;
    }

    private void SoundRecorderViewModel_OnPropertyChanged(object sender, PropertyChangedEventArgs propertyChangedEventArgs)
    {
      if (propertyChangedEventArgs.PropertyName == "IsRecording")
      {
        if(_soundRecorderViewModel.IsRecording)
          RecordingAnimation.Begin();
        else
          RecordingAnimation.Stop();
      }

      if (propertyChangedEventArgs.PropertyName == "IsPlaying")
      {
        PlayButton.State = _soundRecorderViewModel.IsPlaying ? PlayButtonState.Play : PlayButtonState.Pause;
      }

      if (propertyChangedEventArgs.PropertyName == "RecordingExists" && _buttonSave != null)
      {
        _buttonSave.IsEnabled = _soundRecorderViewModel.RecordingExists;
      }
    }

    private void PlayButton_OnClick(object sender, RoutedEventArgs e)
    {
      _soundRecorderViewModel.PlayPauseEvent();
    }

    private void LanguageChanged(object sender, PropertyChangedEventArgs e)
    {
      BuildApplicationBar();
    }

    private void BuildApplicationBar()
    {
      ApplicationBar = new ApplicationBar();

      _buttonSave = new ApplicationBarIconButton(new Uri("/Content/Images/ApplicationBar/dark/appbar.save.rest.png",
                                             UriKind.Relative));
      _buttonSave.IsEnabled = _soundRecorderViewModel.RecordingExists;
      _buttonSave.Text = EditorResources.ButtonSave;
      _buttonSave.Click += (sender, args) => _soundRecorderViewModel.SaveEvent();
      ApplicationBar.Buttons.Add(_buttonSave);

      var buttonCancel = new ApplicationBarIconButton(new Uri("/Content/Images/ApplicationBar/dark/appbar.cancel.rest.png", UriKind.Relative));
      buttonCancel.Text = EditorResources.ButtonCancel;
      buttonCancel.Click += (sender, args) => _soundRecorderViewModel.CancelEvent();
      ApplicationBar.Buttons.Add(buttonCancel);
    }
  }
}