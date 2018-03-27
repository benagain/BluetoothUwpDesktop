using System;
using System.Runtime.InteropServices;
using CredProvider.NET.Interop;
using Serilog;

namespace MyidBluetoothCredentialProvider
{
    [ComVisible(true)]
    [Guid("82416BBE-EDF3-4B54-9E23-2E306FB4D11E")]
    [ClassInterface(ClassInterfaceType.None)]
    [ProgId("Myid.BluetoothCredentialProvider")]
    public class MyidBluetoothCredentialProvider : IMyidBluetoothCredentialProvider
    {
        private _CREDENTIAL_PROVIDER_USAGE_SCENARIO usageScenario;
        private readonly Lazy<MyidBluetoothWindowsCredentialProviderTile> credentialTile;
        private ulong eventsContext;
        private ICredentialProviderEvents events;

        static MyidBluetoothCredentialProvider()
        {
            BluetoothUwpDesktop.Program.ConfigureLogger();
        }

        public MyidBluetoothCredentialProvider()
        {
            credentialTile =
                new Lazy<MyidBluetoothWindowsCredentialProviderTile>(
                    () => new MyidBluetoothWindowsCredentialProviderTile(this, usageScenario));
        }

        public int SetUsageScenario(_CREDENTIAL_PROVIDER_USAGE_SCENARIO cpus, uint dwFlags)
        {
            Log.Debug("SetUsageScenario({cpus}, {dwFlags})", cpus, dwFlags);

            usageScenario = cpus;

            switch (cpus)
            {
                case _CREDENTIAL_PROVIDER_USAGE_SCENARIO.CPUS_CREDUI:
                case _CREDENTIAL_PROVIDER_USAGE_SCENARIO.CPUS_LOGON:
                case _CREDENTIAL_PROVIDER_USAGE_SCENARIO.CPUS_UNLOCK_WORKSTATION:
                    return 0;

                default:
                    throw new NotImplementedException($"MyidBluetoothCredentialProvider does not support {cpus})");
            }
        }

        public int SetSerialization(ref _CREDENTIAL_PROVIDER_CREDENTIAL_SERIALIZATION pcpcs)
        {
            Log.Debug("SetSerialization({pcpcs})", pcpcs);
            throw new NotImplementedException();
        }

        public int Advise(ICredentialProviderEvents pcpe, ulong upAdviseContext)
        {
            Log.Debug("Advise({pcpe}, {upAdviseContext})", pcpe, upAdviseContext);

            if (pcpe != null)
            {
                events = pcpe;
                eventsContext = upAdviseContext;
                Marshal.AddRef(Marshal.GetIUnknownForObject(events));
            }

            return 0;
        }

        public int UnAdvise()
        {
            Log.Debug("UnAdvise()");

            if (events != null)
            {
                Marshal.Release(Marshal.GetIUnknownForObject(events));
                events = null;
                eventsContext = 0;
            }

            return 0;
        }

        public int GetFieldDescriptorCount(out uint pdwCount)
        {
            Log.Debug("GetFieldDescriptorCount()");
            pdwCount = (uint)credentialTile.Value.UiElementCount;
            return 0;
        }

        public int GetFieldDescriptorAt(uint dwIndex, IntPtr ppcpfd)
        {
            Log.Debug("GetFieldDescriptorAt({dwIndex}, {ppcpfd})", dwIndex, ppcpfd);

            credentialTile.Value.DescribeUi(dwIndex, ppcpfd);

            return 0;

        }

        public int GetCredentialCount(out uint pdwCount, out uint pdwDefault, out int pbAutoLogonWithDefault)
        {
            Log.Debug("GetCredentialCount()");
            pdwCount = 1;
            pdwDefault = 0;
            pbAutoLogonWithDefault = 0;
            return 0;

        }

        public int GetCredentialAt(uint dwIndex, out ICredentialProviderCredential ppcpc)
        {
            Log.Debug("GetCredentialAt({dwIndex})", dwIndex);

            ppcpc = credentialTile.Value;

            return 0;
        }
    }
}
