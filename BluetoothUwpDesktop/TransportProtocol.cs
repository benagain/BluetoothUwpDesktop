using System;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Threading.Tasks;
using Bluetooth.Transport;
using Google.Protobuf;
using Serilog;
using Windows.Devices.Bluetooth.GenericAttributeProfile;

namespace BluetoothUwpDesktop
{
    internal class TransportProtocol : IDisposable
    {
        private readonly GattCharacteristic characteristic;
        private readonly PacketBuilder builder;
        private readonly string deviceName;
        private readonly int mtu;

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
            this.mtu = characteristics.Service.Session.MaxPduSize - 3;
            this.mtu = 100;
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

        internal async Task SendCommandAsync(IMessage m)
        {
            var bytes = new byte[(m.CalculateSize())];
            var stream = new CodedOutputStream(bytes);
            m.WriteTo(stream);
            
            var chunker = new PacketChunker(mtu);

            //var all = chunker.Chunks(bytes).Select(x => characteristic.WriteValueAsync(x.AsBuffer(), GattWriteOption.WriteWithResponse).AsTask());
            //await Task.WhenAll(all);

            foreach (var c in chunker.Chunks(bytes))
            {
                try
                {
                    Log.Information("Sending packet");
                    await characteristic.WriteValueAsync(c.AsBuffer(), GattWriteOption.WriteWithResponse);

                }
                catch (Exception e)
                {
                    Log.Error(e, "");
                    throw;
                }
            }

            //await characteristic.WriteValueAsync(bytes.AsBuffer(), GattWriteOption.WriteWithResponse);


            Log.Information("Sent all packets");
        }
    }
}