using System;
using System.Collections.Generic;

namespace Bluetooth.Transport
{
    public class PacketChunker
    {
        private readonly int Mtu;

        public PacketChunker(int mtu)
        {
            Mtu = mtu;
        }

        public IEnumerable<byte[]> Chunks(byte[] buffer)
        {
            var dataToSend = buffer.Length;
            var sendDataIndex = 0;

            var chunks = new List<byte[]>();
            while (sendDataIndex < dataToSend)
            {
                var amountToSend = Math.Min(dataToSend - sendDataIndex, Mtu);

                var copy = new byte[amountToSend];
                Array.Copy(buffer, sendDataIndex, copy, 0, amountToSend);
                chunks.Add(copy);

                sendDataIndex += amountToSend;
            }

            chunks.Add(System.Text.Encoding.UTF8.GetBytes("EOM"));

            return chunks;
        }
    }
}
