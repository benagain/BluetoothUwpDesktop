using System.Runtime.InteropServices;
using CredentialProvider.Interop;

namespace MyidBluetoothCredentialProvider
{
    [ComVisible(true)]
    [Guid("E05A7912-CA63-4731-BAED-928A65B71E21")]
    [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    public interface IMyidBluetoothCredentialProvider : ICredentialProvider
    {
    }
}
