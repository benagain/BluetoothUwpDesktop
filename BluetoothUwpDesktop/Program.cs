using Google.Protobuf;
using Serilog;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Threading.Tasks;
using Windows.Devices.Bluetooth;
using Windows.Devices.Bluetooth.GenericAttributeProfile;
using Windows.Devices.Enumeration;

namespace BluetoothUwpDesktop
{
    class Program
    {
        private static readonly Guid ServiceId = Guid.Parse("0000b710-0000-1000-8000-00805f9b34fb");

        static async Task Main(string[] args)
        {
            Log.Logger = new LoggerConfiguration()
                .Enrich.WithCaller()
                .WriteTo.Console(outputTemplate: "[{Timestamp:HH:mm:ss} {Level:u3}] ({Caller}) {Message}{NewLine}{Exception}")
                .CreateLogger();

            try
            {
                await DiscoverAsync();
            }
            catch (System.Exception e)
            {
                System.Console.WriteLine(e.Message);
            }
        }

        private static List<GattCharacteristic> Subscriptions = new List<GattCharacteristic>();

        private static async System.Threading.Tasks.Task DiscoverAsync()
        {
            string[] requestedProperties =
            {
                "System.Devices.Aep.DeviceAddress",
                "System.Devices.Aep.IsConnected",
                "System.Devices.Aep.Bluetooth.Le.IsConnectable"
            };

            // BT_Code: Example showing paired and non-paired in a single query.
            // https://docs.microsoft.com/en-us/windows/uwp/devices-sensors/enumerate-devices-over-a-network
            string aqsAllBluetoothLEDevices = "(System.Devices.Aep.ProtocolId:=\"{bb7bb05e-5972-42b5-94fc-76eaa7084d49}\")";

            var deviceWatcher =
                    DeviceInformation.CreateWatcher(
                        aqsAllBluetoothLEDevices,
                        requestedProperties,
                        DeviceInformationKind.AssociationEndpoint);

            deviceWatcher.Added += DeviceWatcher_AddedAsync;
            deviceWatcher.Updated += DeviceWatcher_Updated;
            deviceWatcher.Removed += DeviceWatcher_Removed;
            deviceWatcher.EnumerationCompleted += DeviceWatcher_EnumerationCompleted;
            deviceWatcher.Stopped += DeviceWatcher_Stopped;

            deviceWatcher.Start();

            Console.ReadKey();

            deviceWatcher.Stop();

            foreach (var c in Subscriptions)
            {
                var m = new com.intercede.BluetoothSmartcard.Commands.SignatureRequest
                {
                    Plaintext = ByteString.CopyFromUtf8("Lorem ipsum dolor sit amet")
                };
                var bytes = new byte[(m.CalculateSize())];
                var stream = new CodedOutputStream(bytes);
                m.WriteTo(stream);

                Log.Logger.Information("=> {data}", bytes.AsHex());
                await c.WriteValueAsync(bytes.AsBuffer(), GattWriteOption.WriteWithResponse);
            }

            Console.ReadKey();

            foreach(var c in Subscriptions)
            {
                await c.WriteClientCharacteristicConfigurationDescriptorAsync(GattClientCharacteristicConfigurationDescriptorValue.None);
                Log.Logger.Information("Unsubscribed from {id}", c.Uuid);
            }

            Console.ReadKey();
        }

        private static async void DeviceWatcher_AddedAsync(DeviceWatcher sender, DeviceInformation args)
        {
            Log.Logger.Information("{id} {name}", args.Id, args.Name);

            var bluetoothLeDevice = await BluetoothLEDevice.FromIdAsync(args.Id);
            if(bluetoothLeDevice == null)
            {
                Log.Logger.Warning("Couldn't connect to {name}", args.Name);
                return;
            }

            Log.Logger.Information("Connected to {name}", args.Name);

            var result = await bluetoothLeDevice.GetGattServicesAsync(BluetoothCacheMode.Uncached);
            if(result.Status != GattCommunicationStatus.Success)
            {
                Log.Logger.Warning("Couldn't services on {name}", args.Name);
                return;
            }

            Log.Logger.Information("Device services: {services}", result.Services.Select(x => x.Uuid));

            var service = result.Services.FirstOrDefault(x => x.Uuid == ServiceId);
            if (service == null)
            {
                Log.Logger.Warning("Couldn't find our service on {name}", args.Name);
                return;
            }

            Log.Logger.Information("Found  service");

            var characteristics = await service.GetCharacteristicsAsync(BluetoothCacheMode.Uncached);

            Log.Logger.Information("Found {characteristics}", characteristics.Characteristics.Select(x => x.Uuid));

            foreach(var c in characteristics.Characteristics)
            {
                if (c.CharacteristicProperties.HasFlag(GattCharacteristicProperties.Notify))
                {
                    await c.WriteClientCharacteristicConfigurationDescriptorAsync(GattClientCharacteristicConfigurationDescriptorValue.Notify);

                    c.ValueChanged += (_, changedArgs) =>
                    {
                        var bytes = changedArgs.CharacteristicValue.ToArray();
                        Log.Logger.Information("Event! {data}", bytes.AsHex());
                    };

                    Subscriptions.Add(c);

                    Log.Logger.Information("Subscribed to {id}", c.Uuid);
                }
            }


        }

        private static void DeviceWatcher_Updated(DeviceWatcher sender, DeviceInformationUpdate args)
            => Log.Logger.Information("{id}, {properties}", args.Id, args.Properties);

        private static void DeviceWatcher_Removed(DeviceWatcher sender, DeviceInformationUpdate args)
            => Log.Logger.Information(args.Id);

        private static void DeviceWatcher_EnumerationCompleted(DeviceWatcher sender, object args)
            => Log.Logger.Information("");

        private static void DeviceWatcher_Stopped(DeviceWatcher sender, object args)
            => Log.Logger.Information("");
    }

    public static class Hextension
    {
        public static string AsHex(this byte[] bytes)
            => string.Join("", bytes.Select(c => ((int)c).ToString("X2")));
    }
}
