/*
 * PROJECT:   Mouri Internal Library Essentials
 * FILE:      CoreWindowHost.cs
 * PURPOSE:   Definition for Mile.Xaml.CoreWindowHost
 *
 * LICENSE:   The MIT License
 *
 * MAINTAINER: MouriNaruto (Kenji.Mouri@outlook.com)
 */

using System.Windows.Forms;

namespace Mile.Xaml
{
    internal class CoreWindowHost : Form
    {
        private const int WS_EX_TRANSPARENT = 0x00000020;
        private const int WS_EX_LAYERED = 0x00080000;

        public CoreWindowHost()
        {
            Controls.Add(new WindowsXamlHost());
            ShowInTaskbar = false;
            Show();
            Hide();
        }

        protected override CreateParams CreateParams
        {
            get
            {
                CreateParams CreateParameters = base.CreateParams;
                CreateParameters.ExStyle = WS_EX_TRANSPARENT | WS_EX_LAYERED;
                CreateParameters.Style = 0;
                return CreateParameters;
            }
        }
    }
}
