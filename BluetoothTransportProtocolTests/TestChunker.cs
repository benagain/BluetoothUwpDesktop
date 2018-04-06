using System.Linq;
using Bluetooth.Transport;
using FluentAssertions;
using Xunit;

namespace BluetoothTransportProtocolTests
{
    public class TestPacketChunker
    {
        [Fact]
        public void Chunks200BytesOver20Mtu()
        {
            byte[] total = Enumerable.Range(0, 200).Select(x => (byte)x).ToArray();

            var chunks = new PacketChunker(20).Chunks(total);

            chunks.Should().HaveCountGreaterOrEqualTo(10);
        }

        [Fact]
        public void Chunks200BytesOver20MtuContent()
        {
            byte[] total = Enumerable.Range(0, 200).Select(x => (byte)x).ToArray();

            var chunks = new PacketChunker(20).Chunks(total).ToArray();

            chunks[0].Should().BeEquivalentTo(Enumerable.Range(0, 20));
            chunks[1].Should().BeEquivalentTo(Enumerable.Range(20, 20));
            chunks[2].Should().BeEquivalentTo(Enumerable.Range(40, 20));
            chunks[3].Should().BeEquivalentTo(Enumerable.Range(60, 20));
            chunks[4].Should().BeEquivalentTo(Enumerable.Range(80, 20));
        }

        [Fact]
        public void ChunkAddsEOMPacket()
        {
            byte[] total = Enumerable.Range(0, 10).Select(x => (byte)x).ToArray();

            var chunks = new PacketChunker(20).Chunks(total).ToArray();

            chunks.Should().HaveCount(2);
        }

        [Fact]
        public void ChunkAddsEOMValue()
        {
            byte[] total = Enumerable.Range(0, 10).Select(x => (byte)x).ToArray();

            var chunks = new PacketChunker(20).Chunks(total).ToArray();

            chunks[1].Should().BeEquivalentTo("EOM");
        }
    }
}
