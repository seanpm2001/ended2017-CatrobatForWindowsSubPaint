﻿using System;
using System.ComponentModel;
using System.Linq.Expressions;
using Catrobat.IDE.Core.UI.PortableUI;
using Catrobat.IDE.Core.Utilities.Helpers;

namespace Catrobat.IDE.Core.CatrobatObjects
{
    public enum LocalProjectState { Valid, AppUpdateRequired, Damaged, VersionOutdated }

    public class LocalProjectHeader : 
        IComparable<LocalProjectHeader>, INotifyPropertyChanged
    {
        private string _projectName;
        public string ProjectName
        {
            get { return _projectName; }
            set
            {
                _projectName = value;
                RaisePropertyChanged(() => ProjectName);
            }
        }

        private PortableImage _screenshot;
        public PortableImage Screenshot
        {
            get { return _screenshot; }
            set
            {
                _screenshot = value;
                RaisePropertyChanged(() => Screenshot);
            }
        }

        private LocalProjectState _validityState = LocalProjectState.Valid;
        public LocalProjectState ValidityState
        {
            get
            {
                return _validityState;
            }
            set
            {
                _validityState = value;
                RaisePropertyChanged(() => IsValid);
                RaisePropertyChanged(() => ValidityState);
            }
        }

        public bool IsValid
        {
            get
            {
                return _validityState == LocalProjectState.Valid;
            }
        }

 

        public int CompareTo(LocalProjectHeader other)
        {
            return System.String.Compare(ProjectName, other.ProjectName, System.StringComparison.Ordinal);
        }

        #region INotifyPropertyChanged

        public event PropertyChangedEventHandler PropertyChanged;

        public void RaisePropertyChanged<T>(Expression<Func<T>> selector)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(PropertyHelper.GetPropertyName(selector)));
            }
        }

        #endregion
    }
}