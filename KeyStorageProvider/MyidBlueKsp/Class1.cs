using System;
using System.Linq;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Security.Cryptography.X509Certificates;
using System.Text;
//using BlueKspInterface;

namespace MyidBlueKsp
{
    [ComVisible(true)]
    [StructLayout(LayoutKind.Sequential, Pack = 4)]
    public struct CryptKeyName
    {
        [MarshalAs(UnmanagedType.LPWStr)]
        public string pszName;

        [MarshalAs(UnmanagedType.LPWStr)]
        public string pszAlgid;

        public uint dwLegacyKeySpec;

        public uint dwFlags;
    }

    [ComVisible(true)]
    public enum HashAlgorithm
    {
        Pkcs1,
        Pss,
    }

    [ComVisible(true)]
    [Guid("C18D0816-CFEC-4C54-B2B1-0AD4788F8975")]
    [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    public interface IBlueKsp
    {
        IBlueKey[] EnumerateKeys2();

        //void EnumerateKeys(IntPtr key);
    }

    [ComVisible(true)]
    [Guid("9F8E7040-D33B-49CE-B0D5-C44E04BE7CCB")]
    [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    public interface IBlueKey
    {
        void KeyProperties(IntPtr key);

        byte[] Certificate { get; }

        long NumCertificateBytes { get; }

        string Name { get; }

        long CalculateSignatureSize(byte[] data, HashAlgorithm hashAlgorithm);

        byte[] Sign(byte[] data);

        void Hello();

        void GetCryptName(out CryptKeyName name);
    }

    [ComVisible(true)]
    [Guid("577936D2-F166-40EE-A734-2F69F88E4FAC")]
    [ClassInterface(ClassInterfaceType.None)]
    [ProgId("Myid.BlueKsp")]
    public class BlueKsp : IBlueKsp
    {
        private readonly CspParameters csp = new CspParameters(1, "Microsoft Base Smart Card Crypto Provider", "cfc502be-3b4c-3b4d-88af-7295e45fc105");

        public IBlueKey[] EnumerateKeys2()
        {
            using (var store = new X509Store("MyID Phone", StoreLocation.LocalMachine))
            {
                store.Open(OpenFlags.ReadOnly);

                return store.Certificates
                    .Cast<X509Certificate2>()
                    .Select(c => new BlueKey(c))
                    .ToArray();
            }
        }

        //public void EnumerateKeys(IntPtr key)
        //{
        //    using (var store = new X509Store("MyID Phone", StoreLocation.LocalMachine))
        //    {
        //        store.Open(OpenFlags.ReadOnly);
        //        var first = store.Certificates.Cast<X509Certificate2>().FirstOrDefault();

        //        if (first != null)
        //        {
        //            var size = first.RawData.Length;

        //            var keysa = new CryptKeyName
        //            {
        //                pszName = first.Subject,
        //                pszAlgid = first.SignatureAlgorithm.FriendlyName
        //            };

        //            Marshal.StructureToPtr(keysa, key, false);
        //        }
        //    }
        //    //using (var rsa = new RSACryptoServiceProvider(csp))
        //    //{
        //    //    var keysa = new CryptKeyName
        //    //    {
        //    //        Name = rsa.CspKeyContainerInfo.KeyContainerName,
        //    //        AlgorithmId = rsa.SignatureAlgorithm,
        //    //    };

        //    //}
        //}

        public byte[] Sign(byte[] data) => throw new System.NotImplementedException();
    }


    [ComVisible(true)]
    [Guid("4B13196F-9418-449E-A9BD-B02AF7EAB9E6")]
    [ClassInterface(ClassInterfaceType.None)]
    [ProgId("Myid.BlueKey")]
    public class BlueKey : IBlueKey
    {
        private X509Certificate2 certificate;

        public BlueKey(X509Certificate2 cert)
        {
            certificate = cert;
        }

        public void KeyProperties(IntPtr key)
        {
            var keysa = new CryptKeyName
            {
                pszName = Name,
                pszAlgid = certificate.SignatureAlgorithm.FriendlyName
            };

            Marshal.StructureToPtr(keysa, key, false);
        }

        public string Name => certificate.Subject;

        public long NumCertificateBytes => certificate.RawData.Length;

        public byte[] Certificate => certificate.RawData;

        private byte[] Signature = Enumerable.Repeat(0xb0, 50).Select(x => (byte)x).ToArray();
        public long CalculateSignatureSize(byte[] data, HashAlgorithm hashAlgorithm) => Signature.Length;

        public byte[] Sign(byte[] data)
        {
            //new RSACng(new CngKey())
            //var sha = new SHA1Managed();
            //var hash = sha.ComputeHash(data);
            //var sig = ((RSACryptoServiceProvider)certificate.PrivateKey).SignHash(hash, CryptoConfig.MapNameToOID("SHA1"));
            return Signature;
        }

        public void GetCryptName(out CryptKeyName name)
        {
            name = new CryptKeyName
            {
                pszName = Name,
                pszAlgid = certificate.SignatureAlgorithm.FriendlyName
            };
        }

        public void Hello() => throw new NotImplementedException();
    }


    public class Class1
    {
        public static void Main(string[] argv)
        {
            try
            {
                var csp = new CspParameters(
                     1,
                     "Microsoft Base Smart Card Crypto Provider",
                     "cfc502be-3b4c-3b4d-88af-7295e45fc105");

                using (var rsa = new RSACryptoServiceProvider(csp))
                {
                    var data = System.Convert.FromBase64String("Abcd");
                    var sig = rsa.SignData(data, new SHA1CryptoServiceProvider());
                    var bsig = System.Convert.ToBase64String(sig);
                    System.Console.WriteLine(bsig);
                }

            }
            catch (System.Exception e)
            {
                System.Console.WriteLine(e.Message);
            }
        }
    }

    //public class X : BlueKspInterface.IBlueKey
    //{
    //    public void GetCryptKeyName(out NCryptKeyName keyName) => throw new NotImplementedException();
    //}
}