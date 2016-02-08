﻿using System.Xml.Linq;

namespace Catrobat.IDE.Core.Xml.XmlObjects.Formulas
{
    public partial class XmlFormula : XmlObjectNode
    {
        public XmlFormulaTree FormulaTree { get; set; }

        public XmlFormula()
        {
        }

        public XmlFormula(XElement xElement)
        {
            LoadFromXml(xElement);
        }

        internal override void LoadFromXml(XElement xRoot)
        {
            if (xRoot.Element("formulaTree") != null)
            {
                FormulaTree = new XmlFormulaTree(xRoot.Element("formulaTree"));
            }
        }

        internal override XElement CreateXml()
        {
            return (FormulaTree ?? new XmlFormulaTree()).CreateXml();
        }
    }
}