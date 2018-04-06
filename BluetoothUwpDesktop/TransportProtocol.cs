using System;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Threading.Tasks;
using Bluetooth.Transport;
using Serilog;
using Windows.Devices.Bluetooth.GenericAttributeProfile;

namespace BluetoothUwpDesktop
{
    internal class TransportProtocol : IDisposable
    {
        private readonly GattCharacteristic characteristic;
        private readonly PacketBuilder builder;
        private readonly string deviceName;

        public static async Task<TransportProtocol> Create(GattCharacteristic characteristics)
        {
            var o = new TransportProtocol(characteristics);
            await o.Initialise();
            return o;
        }

        private TransportProtocol(GattCharacteristic characteristics)
        {
            this.characteristic = characteristics;
            this.builder = new PacketBuilder();
            this.deviceName = characteristics.Service.Session.DeviceId.Id;
            builder.OnCompletedPacket += Builder_OnCompletedPacket;
        }

        private void Builder_OnCompletedPacket(object sender, PacketEventArgs e)
        {
            Log.Logger.Information("{name} Received a completed packet of {count} bytes", deviceName, e.Bytes.Length);
        }

        private async Task Initialise()
        {
            if (characteristic.CharacteristicProperties.HasFlag(GattCharacteristicProperties.Notify))
            {
                await characteristic.WriteClientCharacteristicConfigurationDescriptorAsync(GattClientCharacteristicConfigurationDescriptorValue.Notify);

                characteristic.ValueChanged += (_, changedArgs) =>
                {
                    var bytes = changedArgs.CharacteristicValue.ToArray();
                    builder.AddBlock(bytes);
                    Log.Logger.Information("{name} Received some data", deviceName);
                };

                Log.Logger.Information("{name} Subscribed to {id}", deviceName, characteristic.Uuid);
            }
        }


        public void Dispose()
        {
            //await c.WriteClientCharacteristicConfigurationDescriptorAsync(GattClientCharacteristicConfigurationDescriptorValue.None);
        }
    }
}