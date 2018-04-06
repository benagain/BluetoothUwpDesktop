using System.Linq;
using Bluetooth.Transport;
using FluentAssertions;
using Xunit;

namespace BluetoothTransportProtocolTests
{
    public class TestBuilder
    {
        [Fact]
        public void Builds_multiples_into_one_buffer()
        {
            var total = Enumerable.Range(0, 200).Select(x => (byte)x).ToArray();

            var chunks = new PacketChunker(20).Chunks(total);
            chunks = chunks.Take(chunks.Count() - 1);

            var builder = new PacketBuilder();
            foreach (var block in chunks)
            {
                builder.AddBlock(block);
            }

            var fullPacket = builder.GetCompletedPacket();

            fullPacket.Should().BeEquivalentTo(total);
        }

        [Fact]
        public void Notifies_callback_of_completed_packet()
        {
            var singlePacket = new byte[] { 0x1, 0x2, 0x3 };
            var eom = System.Text.Encoding.UTF8.GetBytes("EOM");

            var builder = new PacketBuilder();

            using (var monitor = builder.Monitor())
            {
                builder.AddBlock(singlePacket);
                builder.AddBlock(eom);

                monitor.Should()
                    .Raise("OnCompletedPacket")
                    .WithArgs((PacketEventArgs args) => args.Bytes.SequenceEqual(singlePacket));
            }
        }
    }
}
