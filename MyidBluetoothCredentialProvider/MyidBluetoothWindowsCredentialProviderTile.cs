using CredProvider.NET.Interop;
using Serilog;
using System;
//using System.Drawing;
//using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Net;
using System.Runtime.InteropServices;

namespace MyidBluetoothCredentialProvider
{
    internal class MyidBluetoothWindowsCredentialProviderTile : IMyidBluetoothWindowsCredentialProviderTile
    {
        private class CredentialDescriptor
        {
            public _CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR Descriptor { get; set; }
            public _CREDENTIAL_PROVIDER_FIELD_STATE State { get; set; }
        }

        private CredentialDescriptor[] UiElements = CreateUi();
        private Ui Ui = CreateUi2();

        private static Ui CreateUi2()
        {
            return new UiBuilder()
                .Icon(Display.InMenuAndSelection)
                .Text("MyID Bluetooth", Display.InMenuAndSelection)
                .Text("Blue it!", Display.InSelection)
                .Build();
        }

        private static CredentialDescriptor[] CreateUi()
        {
            return new[]
            {
                new CredentialDescriptor
                {
                    State = _CREDENTIAL_PROVIDER_FIELD_STATE.CPFS_DISPLAY_IN_BOTH,
                    Descriptor = new _CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR
                    {
                        cpft = _CREDENTIAL_PROVIDER_FIELD_TYPE.CPFT_SMALL_TEXT,
                        pszLabel = "MyID Bluetooth",
                    },
                },
                new CredentialDescriptor
                {
                    State = _CREDENTIAL_PROVIDER_FIELD_STATE.CPFS_DISPLAY_IN_SELECTED_TILE,
                    Descriptor = new _CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR
                    {
                        cpft = _CREDENTIAL_PROVIDER_FIELD_TYPE.CPFT_SMALL_TEXT,
                        pszLabel = "Blue it!",
                    },
                },
            }
            .Select((x, i) =>
            {
                var d = x.Descriptor;
                d.dwFieldID = (uint)i;
                x.Descriptor = d;
                return x;
            })
            .ToArray();
        }

        private ICredentialProviderCredentialEvents events;

        public MyidBluetoothWindowsCredentialProviderTile(
            MyidBluetoothCredentialProvider testWindowsCredentialProvider,
            _CREDENTIAL_PROVIDER_USAGE_SCENARIO usageScenario)
        {

        }

        public int UiElementCount => (int)Ui.ElementCount;

        public int Advise(ICredentialProviderCredentialEvents pcpce)
        {
            events = pcpce;
            var intPtr = Marshal.GetIUnknownForObject(pcpce);
            Marshal.AddRef(intPtr);
            return HResultValues.S_OK;
        }

        public int UnAdvise()
        {
            Marshal.Release(Marshal.GetIUnknownForObject(events));
            events = null;
            return HResultValues.S_OK;
        }

        public int SetSelected(out int pbAutoLogon)
        {
            pbAutoLogon = 0;
            return HResultValues.S_OK;
        }

        public int SetDeselected() => throw new NotImplementedException();

        public int GetFieldState(uint dwFieldID, out _CREDENTIAL_PROVIDER_FIELD_STATE pcpfs, out _CREDENTIAL_PROVIDER_FIELD_INTERACTIVE_STATE pcpfis)
        {
            (pcpfs, pcpfis) = Ui.GetState(dwFieldID);
            return HResultValues.S_OK;
        }

        public int GetStringValue(uint dwFieldID, out string ppsz)
        {
            ppsz = Ui.GetString(dwFieldID);
            return HResultValues.S_OK;
        }

        private static System.Drawing.Bitmap icon;

        public int GetBitmapValue(uint dwFieldID, IntPtr phbmp)
        {
            Log.Verbose("dwFieldID: {dwFieldID}");

            try
            {
                icon = TryLoadUserIcon();
                phbmp = icon?.GetHbitmap() ?? IntPtr.Zero;
                return HResultValues.S_OK;
            }
            catch (Exception ex)
            {
                Log.Verbose(ex, "Error");
                throw;
            }


        }

        private System.Drawing.Bitmap TryLoadUserIcon()
        {
            using (var web = new WebClient())
            {
                //var bytes = web.DownloadData("https://lh3.googleusercontent.com/-sKxso2c379oy6UlCtymwwPKXO8ODsNXKhYH5fiMaX5_MdWotJEiRq4aYZYZnmkQpw=w300");
                var bytes = web.DownloadData("https://syfuhs.blob.core.windows.net/images/2015/12/mad.jpg");

                var image = System.Drawing.Image.FromStream(new MemoryStream(bytes));

                var imageStream = new MemoryStream();
                image.Save(imageStream, System.Drawing.Imaging.ImageFormat.Bmp);
                imageStream.Seek(0, SeekOrigin.Begin);

                return new System.Drawing.Bitmap(imageStream);
            }
        }

        public int GetCheckboxValue(uint dwFieldID, out int pbChecked, out string ppszLabel) => throw new NotImplementedException();
        public int GetSubmitButtonValue(uint dwFieldID, out uint pdwAdjacentTo)
        {
            pdwAdjacentTo = 0;
            return HResultValues.S_OK;
        }

        public int GetComboBoxValueCount(uint dwFieldID, out uint pcItems, out uint pdwSelectedItem) => throw new NotImplementedException();
        public int GetComboBoxValueAt(uint dwFieldID, uint dwItem, out string ppszItem) => throw new NotImplementedException();
        public int SetStringValue(uint dwFieldID, string psz) => throw new NotImplementedException();
        public int SetCheckboxValue(uint dwFieldID, int bChecked) => throw new NotImplementedException();
        public int SetComboBoxSelectedValue(uint dwFieldID, uint dwSelectedItem) => throw new NotImplementedException();
        public int CommandLinkClicked(uint dwFieldID) => throw new NotImplementedException();

        public int GetSerialization(out _CREDENTIAL_PROVIDER_GET_SERIALIZATION_RESPONSE pcpgsr, out _CREDENTIAL_PROVIDER_CREDENTIAL_SERIALIZATION pcpcs, out string ppszOptionalStatusText, out _CREDENTIAL_PROVIDER_STATUS_ICON pcpsiOptionalStatusIcon)
        {
            Log.Debug("GetSerialization()");

            try
            {
                pcpgsr = _CREDENTIAL_PROVIDER_GET_SERIALIZATION_RESPONSE.CPGSR_RETURN_CREDENTIAL_FINISHED;
                pcpcs = new _CREDENTIAL_PROVIDER_CREDENTIAL_SERIALIZATION();

                var username = @"intercede\ben.arnold";
                var password = "Orleans8";
                var inCredSize = 0;
                var inCredBuffer = Marshal.AllocCoTaskMem(0);

                if (!PInvoke.CredPackAuthenticationBuffer(0, username, password, inCredBuffer, ref inCredSize))
                {
                    Marshal.FreeCoTaskMem(inCredBuffer);
                    inCredBuffer = Marshal.AllocCoTaskMem(inCredSize);

                    if (PInvoke.CredPackAuthenticationBuffer(0, username, password, inCredBuffer, ref inCredSize))
                    {
                        ppszOptionalStatusText = string.Empty;
                        pcpsiOptionalStatusIcon = _CREDENTIAL_PROVIDER_STATUS_ICON.CPSI_SUCCESS;

                        pcpcs.clsidCredentialProvider = Guid.Parse("82416BBE-EDF3-4B54-9E23-2E306FB4D11E");
                        pcpcs.rgbSerialization = inCredBuffer;
                        pcpcs.cbSerialization = (uint)inCredSize;

                        RetrieveNegotiateAuthPackage(out var authPackage);
                        pcpcs.ulAuthenticationPackage = authPackage;

                        return HResultValues.S_OK;
                    }

                    ppszOptionalStatusText = "Failed to pack credentials";
                    pcpsiOptionalStatusIcon = _CREDENTIAL_PROVIDER_STATUS_ICON.CPSI_ERROR;
                    return HResultValues.E_FAIL;
                }
            }
            catch (Exception e)
            {
                // In case of any error, do not bring down winlogon
                Log.Error(e, "Ignored");
            }
            finally
            {
                //shouldAutoLogin = false; // Block auto-login from going full-retard
            }

            pcpgsr = _CREDENTIAL_PROVIDER_GET_SERIALIZATION_RESPONSE.CPGSR_NO_CREDENTIAL_NOT_FINISHED;
            pcpcs = new _CREDENTIAL_PROVIDER_CREDENTIAL_SERIALIZATION();
            ppszOptionalStatusText = string.Empty;
            pcpsiOptionalStatusIcon = _CREDENTIAL_PROVIDER_STATUS_ICON.CPSI_NONE;
            return HResultValues.E_NOTIMPL;
        }

        private int RetrieveNegotiateAuthPackage(out uint authPackage)
        {
            // TODO: better checking on the return codes

            var status = PInvoke.LsaConnectUntrusted(out var lsaHandle);

            using (var name = new PInvoke.LsaStringWrapper("Negotiate"))
            {
                status = PInvoke.LsaLookupAuthenticationPackage(lsaHandle, ref name._string, out authPackage);
            }

            PInvoke.LsaDeregisterLogonProcess(lsaHandle);

            return (int)status;
        }


        public int ReportResult(int ntsStatus, int ntsSubstatus, out string ppszOptionalStatusText, out _CREDENTIAL_PROVIDER_STATUS_ICON pcpsiOptionalStatusIcon) => throw new NotImplementedException();

        internal void DescribeUi(uint dwIndex, IntPtr ppcpfd)
        {
            Ui.DescribeInto(dwIndex, ppcpfd);
        }
    }
}