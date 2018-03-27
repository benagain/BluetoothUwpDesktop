using CredProvider.NET.Interop;
using System.Runtime.InteropServices;

namespace MyidBluetoothCredentialProvider
{
    [ComVisible(true)]
    [Guid("D5C28749-B93C-4BC9-B5AC-113462F66026")]
    [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    public interface IMyidBluetoothWindowsCredentialProviderTile : ICredentialProviderCredential2
    {
    }
}
