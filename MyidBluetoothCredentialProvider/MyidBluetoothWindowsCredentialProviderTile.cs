using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using CredentialProvider.Interop;
using Serilog;

namespace MyidBluetoothCredentialProvider
{
    internal class MyidBluetoothWindowsCredentialProviderTile : IMyidBluetoothWindowsCredentialProviderTile
    {
        private IReadOnlyList<_CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR> UiElements = new List<_CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR>
        {
            new _CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR
            {
                cpft = _CREDENTIAL_PROVIDER_FIELD_TYPE.CPFT_SMALL_TEXT,
                dwFieldID = 0,
                pszLabel = "MyID Bluetooth",
            },
            new _CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR
            {
                cpft = _CREDENTIAL_PROVIDER_FIELD_TYPE.CPFT_SUBMIT_BUTTON,
                dwFieldID = 1,
                pszLabel = "Login",
            }
        };
        private ICredentialProviderCredentialEvents events;

        public MyidBluetoothWindowsCredentialProviderTile(
            MyidBluetoothCredentialProvider testWindowsCredentialProvider,
            _CREDENTIAL_PROVIDER_USAGE_SCENARIO usageScenario)
        {

        }

        public int UiElementCount => UiElements.Count;

        public int Advise(ICredentialProviderCredentialEvents pcpce)
        {
            events = pcpce;
            var intPtr = Marshal.GetIUnknownForObject(pcpce);
            Marshal.AddRef(intPtr);
            return 0;
        }

        public int UnAdvise() => throw new NotImplementedException();
        public int SetSelected(out int pbAutoLogon)
        {
            pbAutoLogon = 0;
            return 0; ;
        }

        public int SetDeselected() => throw new NotImplementedException();

        public int GetFieldState(uint dwFieldID, out _CREDENTIAL_PROVIDER_FIELD_STATE pcpfs, out _CREDENTIAL_PROVIDER_FIELD_INTERACTIVE_STATE pcpfis)
        {
            pcpfis = _CREDENTIAL_PROVIDER_FIELD_INTERACTIVE_STATE.CPFIS_NONE;
            pcpfs = _CREDENTIAL_PROVIDER_FIELD_STATE.CPFS_DISPLAY_IN_BOTH;
            return 0;
        }

        public int GetStringValue(uint dwFieldID, out string ppsz)
        {
            ppsz = "Hello bob!";
            return 0;
        }

        public int GetBitmapValue(uint dwFieldID, IntPtr phbmp) => throw new NotImplementedException();
        public int GetCheckboxValue(uint dwFieldID, out int pbChecked, out string ppszLabel) => throw new NotImplementedException();
        public int GetSubmitButtonValue(uint dwFieldID, out uint pdwAdjacentTo)
        {
            pdwAdjacentTo = 0;
            return 0;
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
            if (dwIndex > UiElements.Count)
                throw new ArgumentOutOfRangeException(nameof(dwIndex));

            var listItem = UiElements[(int)dwIndex];
            var pcpfd = Marshal.AllocHGlobal(Marshal.SizeOf(listItem)); /* _CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR* */
            Marshal.StructureToPtr(listItem, pcpfd, false); /* pcpfd = &CredentialProviderFieldDescriptorList */
            Marshal.StructureToPtr(pcpfd, ppcpfd, false); /* *ppcpfd = pcpfd */
        }
    }
}