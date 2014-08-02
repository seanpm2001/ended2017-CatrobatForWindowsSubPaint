﻿using System.Xml.Linq;

namespace Catrobat.IDE.Core.Xml.XmlObjects.Bricks.Properties
{
    public partial class XmlComeToFrontBrick : XmlBrick
    {
        public XmlComeToFrontBrick() {}

        public XmlComeToFrontBrick(XElement xElement) : base(xElement) {}

        internal override void LoadFromXml(XElement xRoot) {}

        internal override XElement CreateXml()
        {
            var xRoot = new XElement("comeToFrontBrick");

            //CreateCommonXML(xRoot);

            return xRoot;
        }
    }
}