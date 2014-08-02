﻿using System;
using System.Diagnostics;
using Windows.UI.Xaml.Controls;
using Catrobat.IDE.Core.Services;
using Catrobat.IDE.Core.ViewModels;
using Catrobat.IDE.Core.ViewModels.Editor.Sprites;
using Catrobat.IDE.WindowsPhone.Controls;

namespace Catrobat.IDE.WindowsPhone.Views.Editor.Sprites
{
    public partial class SpritesView
    {
        private readonly SpritesViewModel _viewModel =
            ServiceLocator.ViewModelLocator.SpritesViewModel;

        public SpritesView()
        {
            InitializeComponent();
        }
        
        //private void ViewModelOnPropertyChanged(object sender, PropertyChangedEventArgs args)
        //{
        //    if (args.PropertyName == PropertyNameHelper.GetPropertyNameFromExpression(() => _viewModel.SelectedSprite))
        //    {
        //        var selectedSprite = _viewModel.SelectedSprite;

        //        ReorderListBoxSprites.SelectedItem = selectedSprite;
        //    }
        //}
        private void MultiModeEditorCommandBar_OnModeChanged(MultiModeEditorCommandBarMode mode)
        {
            var listView = (ListView)this.FindName("ListViewSprites"); // Hack

            Debug.Assert(listView != null, "listView != null");

            switch (mode)
            {
                case MultiModeEditorCommandBarMode.Normal:
                    listView.SelectionMode = ListViewSelectionMode.None;
                    listView.ReorderMode = ListViewReorderMode.Disabled;
                    break;
                case MultiModeEditorCommandBarMode.Reorder:
                    listView.SelectionMode = ListViewSelectionMode.None;
                    listView.ReorderMode = ListViewReorderMode.Enabled;
                    break;
                case MultiModeEditorCommandBarMode.Select:
                    listView.SelectionMode = ListViewSelectionMode.Multiple;
                    listView.ReorderMode = ListViewReorderMode.Disabled;
                    break;
                default:
                    throw new ArgumentOutOfRangeException("mode");
            }
        }
    }
}