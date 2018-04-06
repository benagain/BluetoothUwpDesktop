using System.Linq;
using Bluetooth.Transport;
using FluentAssertions;
using Xunit;

namespace BluetoothTransportProtocolTests
{
    public class TestBuilder
    {
        private readonly PacketBuilder sut = new PacketBuilder();
        private readonly byte[] EOM = System.Text.Encoding.UTF8.GetBytes("EOM");

        [Fact]
        public void Builds_multiples_into_one_buffer()
        {
            var total = Enumerable.Range(0, 200).Select(x => (byte)x).ToArray();

            var chunks = new PacketChunker(20).Chunks(total);
            chunks = chunks.Take(chunks.Count() - 1);

            foreach (var block in chunks)
            {
                sut.AddBlock(block);
            }

            var fullPacket = sut.GetCompletedPacket();

            fullPacket.Should().BeEquivalentTo(total);
        }

        [Fact]
        public void Notifies_callback_of_completed_packet()
        {
            var singlePacket = new byte[] { 0x1, 0x2, 0x3 };

            using (var monitor = sut.Monitor())
            {
                sut.AddBlock(singlePacket);
                sut.AddBlock(EOM);

                monitor.Should()
                    .Raise("OnCompletedPacket")
                    .WithArgs((PacketEventArgs args) => args.Bytes.SequenceEqual(singlePacket));
            }
        }

        [Fact]
        public void Resets_buffer_after_completed_packet()
        {
            var singlePacket = new byte[] { 0x1, 0x2, 0x3 };

            sut.AddBlock(singlePacket);
            sut.AddBlock(EOM);

            using (var monitor = sut.Monitor())
            {
                sut.AddBlock(singlePacket);
                sut.AddBlock(EOM);

                monitor.Should()
                    .Raise("OnCompletedPacket")
                    .WithArgs((PacketEventArgs args) => args.Bytes.SequenceEqual(singlePacket));
            }
        }
    }
}
